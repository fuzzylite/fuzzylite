/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * FclImporter.cpp
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#include "fl/imex/FclImporter.h"

#include "fl/Headers.h"

#include <sstream>
#include <iostream>

namespace fl {

    FclImporter::FclImporter() { }

    FclImporter::~FclImporter() { }

    std::string FclImporter::name() const {
        return "FclImporter";
    }

    Engine* FclImporter::fromString(const std::string& fcl) const {
        Engine* engine = new Engine;
        engine->addHedge(new Any);
        engine->addHedge(new Extremely);
        engine->addHedge(new Not);
        engine->addHedge(new Seldom);
        engine->addHedge(new Somewhat);
        engine->addHedge(new Very);

        std::map<std::string, std::string> tags;
        tags["VAR_INPUT"] = "END_VAR";
        tags["VAR_OUTPUT"] = "END_VAR";
        tags["FUZZIFY"] = "END_FUZZIFY";
        tags["DEFUZZIFY"] = "END_DEFUZZIFY";
        tags["RULEBLOCK"] = "END_RULEBLOCK";
        std::map<std::string, std::string>::const_iterator tagFinder;

        std::string currentTag = "", closingTag = "";
        std::ostringstream block;
        std::istringstream fclReader(fcl);
        std::string line;
        int lineNumber = 0;
        try {
            while (std::getline(fclReader, line)) {
                ++lineNumber;
                line = Op::trim(line);
                if (line.empty() or line.at(0) == '#')
                    continue;
                line = fl::Op::findReplace(line, ";", "");
                std::istringstream tokenizer(line);
                std::string firstToken;
                tokenizer >> firstToken;

                if (firstToken == "FUNCTION_BLOCK") {
                    if (tokenizer.rdbuf()->in_avail() > 0) {
                        std::ostringstream ss;
                        std::string token;
                        tokenizer >> token;
                        ss << token;
                        while (tokenizer >> token) {
                            ss << " " << token;
                        }
                        engine->setName(ss.str());
                    }
                    continue;
                }
                if (firstToken == "END_FUNCTION_BLOCK") {
                    break;
                }

                if (currentTag.empty()) {
                    tagFinder = tags.find(firstToken);
                    if (tagFinder == tags.end()) {
                        std::ostringstream ex;
                        ex << "[syntax error] unknown block definition <" << firstToken
                                << "> " << " in line " << lineNumber << ": " << line;
                        throw fl::Exception(ex.str(), FL_AT);
                    }
                    currentTag = tagFinder->first;
                    closingTag = tagFinder->second;
                    block.clear();
                    block.str("");
                    block << line << "\n";
                    continue;
                }

                if (not currentTag.empty()) {
                    if (firstToken == closingTag) {
                        processBlock(currentTag, block.str(), engine);
                        currentTag = "";
                        closingTag = "";
                    } else if (tags.find(firstToken) != tags.end()) {
                        //if opening new block without closing the previous one
                        std::ostringstream ex;
                        ex << "[syntax error] expected <" << closingTag << "> before <"
                                << firstToken << "> in line: " << line;
                        throw fl::Exception(ex.str(), FL_AT);
                    } else {
                        block << line << "\n";
                    }
                    continue;
                }
            }

            if (not currentTag.empty()) {
                std::ostringstream ex;
                ex << "[syntax error] ";
                if (block.rdbuf()->in_avail() > 0) {
                    ex << "expected <" << closingTag << "> for block:\n" << block.str();
                } else {
                    ex << "expected <" << closingTag << ">, but not found";
                }
                throw fl::Exception(ex.str(), FL_AT);
            }
            //            if (engine->numberOfInputVariables() == 0
            //                    and engine->numberOfOutputVariables() == 0
            //                    and (engine->numberOfRuleBlocks() == 0
            //                    or engine->getRuleBlock(0)->numberOfRules() == 0)) {
            //                std::ostringstream ex;
            //                ex << "[importer error] the FCL code introduced produces an empty engine";
            //                throw fl::Exception(ex.str(), FL_AT);
            //            }
        } catch (fl::Exception& ex) {
            delete engine;
            throw ex;
        }
        return engine;
    }

    void FclImporter::processBlock(const std::string& tag, const std::string& block, Engine* engine) const {
        if (tag == "VAR_INPUT" or tag == "VAR_OUTPUT") {
            processVar(tag, block, engine);
        } else if (tag == "FUZZIFY") {
            processFuzzify(block, engine);
        } else if (tag == "DEFUZZIFY") {
            processDefuzzify(block, engine);
        } else if (tag == "RULEBLOCK") {
            processRuleBlock(block, engine);
        } else {
            std::ostringstream ex;
            ex << "[syntax error] unexpected tag <" << tag << "> for block:\n" << block;
            throw fl::Exception(ex.str(), FL_AT);
        }
    }

    void FclImporter::processVar(const std::string& tag, const std::string& block, Engine* engine)const {
        std::istringstream blockReader(block);
        std::string line;

        std::getline(blockReader, line); //discard first line as it is VAR_INPUT
        while (getline(blockReader, line)) {
            std::vector<std::string> token = Op::split(line, ":");
            if (token.size() != 2) {
                std::ostringstream ex;
                ex << "[syntax error] expected property of type (key : value) in line: " << line;
                throw fl::Exception(ex.str(), FL_AT);
            }
            std::string name = fl::Op::format(token.at(0), fl::Op::isValidForName);
            if (tag == "VAR_INPUT")
                engine->addInputVariable(new InputVariable(name));
            else if (tag == "VAR_OUTPUT")
                engine->addOutputVariable(new OutputVariable(name));
            else {
                std::ostringstream ex;
                ex << "[syntax error] unexpected tag <" << tag << "> in line: " << line;
                throw fl::Exception(ex.str(), FL_AT);
            }
        }
    }

    void FclImporter::processFuzzify(const std::string& block, Engine* engine)const {
        std::istringstream blockReader(block);
        std::string line;

        std::getline(blockReader, line);
        std::string name;
        std::size_t index = line.find_first_of(' ');
        if (index != std::string::npos) {
            name = fl::Op::format(line.substr(index + 1), fl::Op::isValidForName);
        } else {
            std::ostringstream ex;
            ex << "[syntax error] expected an input variable name in line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        if (not engine->hasInputVariable(name)) {
            std::ostringstream ex;
            ex << "[syntax error] input variable <" << name
                    << "> not registered in engine. "
                    << "Line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        InputVariable* inputVariable = engine->getInputVariable(name);
        while (std::getline(blockReader, line)) {
            std::istringstream ss(line);
            std::string firstToken;
            ss >> firstToken;
            try {
                if (firstToken == "RANGE") {
                    scalar minimum, maximum;
                    extractRange(line, minimum, maximum);
                    inputVariable->setMinimum(minimum);
                    inputVariable->setMaximum(maximum);
                } else if (firstToken == "TERM") {
                    inputVariable->addTerm(prepareTerm(extractTerm(line), engine));
                } else throw fl::Exception("[syntax error] token <" + firstToken + " not recognized", FL_AT);
            } catch (fl::Exception& ex) {
                ex.append("At line: <" + line + ">");
                throw ex;
            }
        }

    }

    void FclImporter::processDefuzzify(const std::string& block, Engine* engine) const {
        std::istringstream blockReader(block);
        std::string line;

        std::getline(blockReader, line);
        std::string name;
        std::size_t index = line.find_first_of(' ');
        if (index != std::string::npos) {
            name = fl::Op::format(line.substr(index + 1), fl::Op::isValidForName);
        } else {
            std::ostringstream ex;
            ex << "[syntax error] expected an output variable name in line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        if (not engine->hasOutputVariable(name)) {
            std::ostringstream ex;
            ex << "[syntax error] output variable <" << name
                    << "> not registered in engine. "
                    << "Line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        OutputVariable* outputVariable = engine->getOutputVariable(name);
        while (std::getline(blockReader, line)) {
            line = fl::Op::trim(line);
            std::string firstToken = line.substr(0, line.find_first_of(' '));
            if (firstToken == "TERM") {
                outputVariable->addTerm(prepareTerm(extractTerm(line), engine));
            } else if (firstToken == "METHOD") {
                outputVariable->setDefuzzifier(extractDefuzzifier(line));
            } else if (firstToken == "ACCU") {
                outputVariable->output()->setAccumulation(extractSNorm(line));
            } else if (firstToken == "DEFAULT") {
                bool lockValidOutput;
                outputVariable->setDefaultValue(extractDefaultValue(line, lockValidOutput));
                outputVariable->setLockValidOutput(lockValidOutput);
            } else if (firstToken == "RANGE") {
                scalar minimum, maximum;
                extractRange(line, minimum, maximum);
                outputVariable->setMinimum(minimum);
                outputVariable->setMaximum(maximum);
            } else if (firstToken == "LOCK") {
                bool valid, range;
                extractLock(line, valid, range);
                outputVariable->setLockValidOutput(valid);
                outputVariable->setLockOutputRange(range);
            } else {
                std::ostringstream ex;
                ex << "[syntax error] unexpected token <" << firstToken <<
                        "> in line: " << line;
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

    }

    void FclImporter::processRuleBlock(const std::string& block, Engine* engine)const {
        std::istringstream blockReader(block);
        std::string line;

        std::string name;
        std::getline(blockReader, line);
        std::size_t index = line.find_last_of(' ');
        if (index != std::string::npos) name = line.substr(index);
        RuleBlock * ruleblock = new RuleBlock(name);
        engine->addRuleBlock(ruleblock);

        while (getline(blockReader, line)) {
            std::string firstToken = line.substr(0, line.find_first_of(' '));
            if (firstToken == "AND") {
                ruleblock->setTnorm(extractTNorm(line));
            } else if (firstToken == "OR") {
                ruleblock->setSnorm(extractSNorm(line));
            } else if (firstToken == "ACT") {
                ruleblock->setActivation(extractTNorm(line));
            } else if (firstToken == "RULE") {
                std::string rule = line.substr(line.find_first_of(':') + 1);
                rule = fl::Op::trim(rule);
                ruleblock->addRule(FuzzyRule::parse(rule, engine));
            } else {
                std::ostringstream ex;
                ex << "[syntax error] keyword <" << firstToken
                        << "> not recognized in line: " << line;
                throw fl::Exception(ex.str(), FL_AT);
            }

        }
    }

    TNorm* FclImporter::extractTNorm(const std::string& line) const {
        std::vector<std::string> token = Op::split(line, ":");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        std::string name = Op::trim(token.at(1));
        std::string className = name;
        if (name == "MIN") className = Minimum().className();
        else if (name == "PROD") className = AlgebraicProduct().className();
        else if (name == "BDIF") className = BoundedDifference().className();
        else if (name == "DPROD") className = DrasticProduct().className();
        else if (name == "EPROD") className = EinsteinProduct().className();
        else if (name == "HPROD") className = HamacherProduct().className();

        try {
            return Factory::instance()->tnorm()->create(className);
        } catch (fl::Exception& ex) {
            ex.append("[syntax error] T-Norm <" + name + "> not recognized in line:\n"
                    + line, FL_AT);
            throw ex;
        }
    }

    SNorm* FclImporter::extractSNorm(const std::string& line) const {
        std::vector<std::string> token = Op::split(line, ":");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        std::string name = Op::trim(token.at(1));
        std::string className = name;
        if (name == "MAX") className = Maximum().className();
        else if (name == "ASUM") className = AlgebraicSum().className();
        else if (name == "BSUM") className = BoundedSum().className();
        else if (name == "NSUM") className = NormalizedSum().className();
        else if (name == "DSUM") className = DrasticSum().className();
        else if (name == "ESUM") className = EinsteinSum().className();
        else if (name == "HSUM") className = HamacherSum().className();

        try {
            return Factory::instance()->snorm()->create(className);
        } catch (fl::Exception& ex) {
            ex.append("[syntax error] S-Norm <" + name + "> not recognized in line:\n"
                    + line, FL_AT);
            throw ex;
        }
    }

    Term* FclImporter::extractTerm(const std::string& line) const {
        std::ostringstream spacer;
        for (std::size_t i = 0; i < line.size(); ++i) {
            if (line.at(i) == '(' or line.at(i) == ')' or line.at(i) == ',') {
                spacer << " " << line.at(i) << " ";
            } else if (line.at(i) == ':') {
                spacer << " :";
            } else if (line.at(i) == '=') {
                spacer << "= ";
            } else
                spacer << line.at(i);
        }
        std::string spacedLine = spacer.str();

        enum FSM {
            S_KWTERM, S_NAME, S_ASSIGN, S_TERMCLASS, S_PARAMETERS
        };
        FSM state = S_KWTERM;
        std::istringstream tokenizer(spacedLine);
        std::string token;
        std::string name, termClass;
        std::vector<std::string> strParams;
        while (tokenizer >> token) {
            if (state == S_KWTERM and token == "TERM") {
                state = S_NAME;
                continue;
            }
            if (state == S_NAME) {
                name = token;
                state = S_ASSIGN;
                continue;
            }
            if (state == S_ASSIGN and token == ":=") {
                state = S_TERMCLASS;
                continue;
            }
            if (state == S_TERMCLASS) {
                if (fl::Op::isNumeric(token)) {
                    termClass = Constant().className();
                    strParams.push_back(token);
                } else if (token == "(") {
                    termClass = Discrete().className();
                } else {
                    termClass = token;
                }
                state = S_PARAMETERS;
                continue;
            }
            if (state == S_PARAMETERS) {
                if (termClass != Function().className() and
                        (token == "(" or token == ")" or token == ",")) {
                    continue;
                }
                if (token == ";") break;
                strParams.push_back(fl::Op::trim(token));
            }
        }
        if (state <= S_ASSIGN)
            throw fl::Exception("[syntax error] malformed term in line: " + line, FL_AT);

        std::vector<scalar> params;
        if (termClass != Function().className()) {
            try {
                for (std::size_t i = 0; i < strParams.size(); ++i) {
                    params.push_back(fl::Op::toScalar(strParams.at(i)));
                }
            } catch (...) {
                std::ostringstream ex;
                ex << "[syntax error] expected numeric value, but found <"
                        << token << "> in line: " << line;
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

        try {
            Term * result = Factory::instance()->term()->create(termClass, params);
            result->setName(fl::Op::format(name, fl::Op::isValidForName));

            if (termClass == Function().className() and not strParams.empty()) {
                std::ostringstream ss;
                for (std::size_t i = 0; i < strParams.size(); ++i) {
                    ss << strParams.at(i);
                }
                std::string infix = ss.str();
                if (infix.size() > 1 and infix.at(0) == '(' and infix.at(infix.size() - 1) == ')') {
                    infix = infix.substr(1, infix.size() - 2);
                }
                dynamic_cast<Function*> (result)->setInfix(infix);
            }

            return result;
        } catch (fl::Exception& ex) {
            ex.append(FL_AT);
            throw ex;
        }
    }

    Term* FclImporter::prepareTerm(Term* term, const Engine* engine) const {
        if (term->className() == Linear().className()) {
            Linear* linear = dynamic_cast<Linear*> (term);
            linear->inputVariables = std::vector<const InputVariable*>
                    (engine->inputVariables().begin(),
                    engine->inputVariables().end());
        } else if (term->className() == Function().className()) {
            Function* function = dynamic_cast<Function*> (term);
            function->setEngine(engine);
            function->load();
        }
        return term;
    }

    Defuzzifier* FclImporter::extractDefuzzifier(const std::string& line) const {
        std::vector<std::string> token = Op::split(line, ":");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        std::string name = fl::Op::trim(fl::Op::findReplace(token.at(1), ";", ""));
        std::string className = name;
        if (name == "COG") className = Centroid().className();
        if (name == "COA") className = Bisector().className();
        if (name == "LM") className = SmallestOfMaximum().className();
        if (name == "RM") className = LargestOfMaximum().className();
        if (name == "MM") className = MeanOfMaximum().className();
        if (name == "COGS") className = WeightedAverage().className();
        if (name == "COGSS") className = WeightedSum().className();

        try {
            return Factory::instance()->defuzzifier()->create(className);
        } catch (fl::Exception& ex) {
            ex.append("[syntax error] defuzzifier <" + name +
                    "> not recognized in line:\n" + line, FL_AT);
            throw ex;
        }
    }

    scalar FclImporter::extractDefaultValue(const std::string& line, bool& lockValidOutput) const {
        std::vector<std::string> token = Op::split(line, ":=");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key := value) in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        std::vector<std::string> values = Op::split(token.at(1), "|");

        std::string defaultValue = values.front();
        std::string nc;
        if (values.size() == 2) nc = values.back();

        defaultValue = fl::Op::trim(fl::Op::findReplace(defaultValue, ";", ""));
        nc = fl::Op::trim(fl::Op::findReplace(nc, ";", ""));

        scalar value;
        try {
            value = fl::Op::toScalar(defaultValue);
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, "
                    << "but found <" << defaultValue << "> in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        lockValidOutput = (nc == "NC");

        if (not (lockValidOutput or nc.empty())) {
            throw fl::Exception("[syntax error] expected keyword <NC>, "
                    "but found <" + nc + "> in line: " + line, FL_AT);
        }

        return value;
    }

    void FclImporter::extractRange(const std::string& line, scalar& minimum, scalar& maximum) const {
        std::vector<std::string> token = Op::split(line, ":=");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key := value) in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        std::string rangeToken = token.at(1);

        std::ostringstream range;
        for (std::size_t i = 0; i < rangeToken.size(); ++i) {
            char character = rangeToken.at(i);
            if (character == '(' or character == ')' or character == ' ' or character == ';')
                continue;
            range << character;
        }
        token = Op::split(range.str(), "..");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type 'start .. end' in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        try {
            minimum = Op::toScalar(token.at(0));
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, but found <" << token.at(0) << "> in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        try {
            maximum = Op::toScalar(token.at(1));
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, but found <" << token.at(1) << "> in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
    }

    void FclImporter::extractLock(const std::string& line, bool& valid, bool& range) const {
        std::size_t index = line.find_first_of(":");
        if (index == std::string::npos) {
            throw fl::Exception("[syntax error] expected property of type "
                    "'key : value' in line: " + line, FL_AT);
        }
        std::string value = fl::Op::findReplace(line.substr(index + 1), ";", "");
        std::vector<std::string> flags = fl::Op::split(value, "|");
        if (flags.size() == 1) {
            std::string flag = fl::Op::trim(flags.front());
            valid = (flag == "VALID");
            range = (flag == "RANGE");
            if (not (valid or range)) {
                throw fl::Exception("[syntax error] expected locking flags "
                        "<VALID|RANGE>, but found <" + flag + "> in line: " + line, FL_AT);
            }
        } else if (flags.size() == 2) {
            std::string flagA = fl::Op::trim(flags.front());
            std::string flagB = fl::Op::trim(flags.back());
            valid = (flagA == "VALID" or flagB == "VALID");
            range = (flagA == "RANGE" or flagB == "RANGE");
            if (not (valid and range)) {
                throw fl::Exception("[syntax error] expected locking flags "
                        "<VALID|RANGE>, but found "
                        "<" + flags.front() + "|" + flags.back() + "> in line: " + line, FL_AT);
            }
        } else throw fl::Exception("[syntax error] expected locking flags "
                "<VALID|RANGE>, but found "
                "<" + value + "> in line: " + line, FL_AT);
    }


}
