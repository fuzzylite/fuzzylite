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

    FclImporter::FclImporter() {
    }

    FclImporter::~FclImporter() {
    }

    std::string FclImporter::name() const {
        return "FclImporter";
    }

    Engine* FclImporter::fromString(const std::string& fcl) const {
        Engine* engine = new Engine;

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
                std::vector<std::string> comments;
                comments = Op::split(line, "//");
                if ((int) comments.size() > 1) {
                    line = comments.front();
                }
                line = Op::trim(line);
                if (line.empty() or line.at(0) == '#')
                    continue;
                line = fl::Op::findReplace(line, ";", "");
                std::istringstream tokenizer(line);
                std::string firstToken;
                tokenizer >> firstToken;

                if (firstToken == "FUNCTION_BLOCK") {
                    if (tokenizer.rdbuf()->in_avail() > 0) {
                        std::ostringstream name;
                        std::string token;
                        tokenizer >> token;
                        name << token;
                        while (tokenizer >> token) {
                            name << " " << token;
                        }
                        engine->setName(name.str());
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
                    block.str("");
                    block.clear();
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
        while (std::getline(blockReader, line)) {
            std::vector<std::string> token = Op::split(line, ":");
            if (token.size() != 2) {
                std::ostringstream ex;
                ex << "[syntax error] expected property of type (key : value) in line: " << line;
                throw fl::Exception(ex.str(), FL_AT);
            }
            std::string name = fl::Op::makeValidId(token.at(0));
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
            name = fl::Op::makeValidId(line.substr(index + 1));
        } else {
            std::ostringstream ex;
            ex << "[syntax error] expected name of input variable in line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        if (not engine->hasInputVariable(name)) {
            std::ostringstream ex;
            ex << "[syntax error] engine does not contain "
                    "input variable <" << name << "> from line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        InputVariable* inputVariable = engine->getInputVariable(name);
        while (std::getline(blockReader, line)) {
            std::istringstream ss(line);
            std::string firstToken;
            ss >> firstToken;
            try {
                if (firstToken == "RANGE") {
                    std::pair<scalar, scalar> minmax = extractRange(line);
                    inputVariable->setMinimum(minmax.first);
                    inputVariable->setMaximum(minmax.second);
                } else if (firstToken == "ENABLED") {
                    inputVariable->setEnabled(extractEnabled(line));
                } else if (firstToken == "TERM") {
                    inputVariable->addTerm(prepareTerm(extractTerm(line), engine));
                } else throw fl::Exception("[syntax error] unexpected token "
                        "<" + firstToken + ">" + line, FL_AT);
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
            name = fl::Op::makeValidId(line.substr(index + 1));
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
            std::istringstream tokenizer(line);
            std::string firstToken;
            tokenizer >> firstToken;
            if (firstToken == "TERM") {
                outputVariable->addTerm(prepareTerm(extractTerm(line), engine));
            } else if (firstToken == "METHOD") {
                outputVariable->setDefuzzifier(extractDefuzzifier(line));
            } else if (firstToken == "ACCU") {
                outputVariable->fuzzyOutput()->setAccumulation(extractSNorm(line));
            } else if (firstToken == "DEFAULT") {
                std::pair<scalar, bool> defaultAndLock = extractDefaultValue(line);
                outputVariable->setDefaultValue(defaultAndLock.first);
                outputVariable->setLockValidOutput(defaultAndLock.second or
                        outputVariable->isLockingValidOutput());
            } else if (firstToken == "RANGE") {
                std::pair<scalar, scalar> minmax = extractRange(line);
                outputVariable->setMinimum(minmax.first);
                outputVariable->setMaximum(minmax.second);
            } else if (firstToken == "LOCK") {
                std::pair<bool, bool> output_range = extractLocksOutputRange(line);
                outputVariable->setLockValidOutput(output_range.first);
                outputVariable->setLockOutputRange(output_range.second);
            } else if (firstToken == "ENABLED") {
                outputVariable->setEnabled(extractEnabled(line));
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

        while (std::getline(blockReader, line)) {
            std::string firstToken = line.substr(0, line.find_first_of(' '));
            if (firstToken == "AND") {
                ruleblock->setConjunction(extractTNorm(line));
            } else if (firstToken == "OR") {
                ruleblock->setDisjunction(extractSNorm(line));
            } else if (firstToken == "ACT") {
                ruleblock->setActivation(extractTNorm(line));
            } else if (firstToken == "ENABLED") {
                ruleblock->setEnabled(extractEnabled(line));
            } else if (firstToken == "RULE") {
                std::size_t ruleStart = line.find_first_of(':');
                if (ruleStart == std::string::npos) ruleStart = 4; // "RULE".size()
                std::string rule = line.substr(ruleStart + 1);
                rule = fl::Op::trim(rule);
                ruleblock->addRule(Rule::parse(rule, engine));
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
            return FactoryManager::instance()->tnorm()->createInstance(className);
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
            return FactoryManager::instance()->snorm()->createInstance(className);
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
        std::vector<std::string> parameters;
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
                    parameters.push_back(token);
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
                parameters.push_back(fl::Op::trim(token));
            }
        }
        if (state <= S_ASSIGN)
            throw fl::Exception("[syntax error] malformed term in line: " + line, FL_AT);

        try {
            Term * result = FactoryManager::instance()->term()->createInstance(termClass);
            result->setName(fl::Op::makeValidId(name));
            if (dynamic_cast<Function*> (result)) {
                result->configure(Op::join(parameters, "")); //remove spaces for text of function
            } else {
                result->configure(Op::join(parameters, " "));
            }

            return result;
        } catch (fl::Exception& ex) {
            ex.append(FL_AT);
            throw ex;
        }
    }

    Term* FclImporter::prepareTerm(Term* term, const Engine* engine) const {
        Linear* linear = NULL;
        Function* function = NULL;
        if ((linear = dynamic_cast<Linear*> (term))) {
            linear->inputVariables = std::vector<const InputVariable*>
                    (engine->inputVariables().begin(),
                    engine->inputVariables().end());
        } else if ((function = dynamic_cast<Function*> (term))) {
            function->setEngine(engine);
            //builtin functions are loaded from TermFactory calling Function::create
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

        std::string name = fl::Op::trim(token.at(1));
        std::string className = name;
        if (name == "COG") className = Centroid().className();
        if (name == "COA") className = Bisector().className();
        if (name == "LM") className = SmallestOfMaximum().className();
        if (name == "RM") className = LargestOfMaximum().className();
        if (name == "MM") className = MeanOfMaximum().className();
        if (name == "COGS") className = WeightedAverage().className();
        if (name == "COGSS") className = WeightedSum().className();

        try {
            return FactoryManager::instance()->defuzzifier()->createInstance(className);
        } catch (fl::Exception& ex) {
            ex.append("[syntax error] defuzzifier <" + name +
                    "> not recognized in line:\n" + line, FL_AT);
            throw ex;
        }
    }

    std::pair<scalar, bool> FclImporter::extractDefaultValue(const std::string& line) const {
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

        defaultValue = fl::Op::trim(defaultValue);
        nc = fl::Op::trim(nc);

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

        bool lockValidOutput = (nc == "NC");

        if (not (lockValidOutput or nc.empty())) {
            throw fl::Exception("[syntax error] expected keyword <NC>, "
                    "but found <" + nc + "> in line: " + line, FL_AT);
        }

        return std::pair<scalar, bool>(value, lockValidOutput);
    }

    std::pair<scalar, scalar> FclImporter::extractRange(const std::string& line) const {
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
            ex << "[syntax error] expected property of type 'start .. end', "
                    << "but found <" << range << "> in line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        scalar minimum, maximum;
        int index;
        try {
            minimum = Op::toScalar(token.at(index = 0));
            maximum = Op::toScalar(token.at(index = 1));
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, but found <" << token.at(index) << "> in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        return std::pair<scalar, scalar>(minimum, maximum);
    }

    std::pair<bool, bool> FclImporter::extractLocksOutputRange(const std::string& line) const {
        std::size_t index = line.find_first_of(":");
        if (index == std::string::npos) {
            throw fl::Exception("[syntax error] expected property of type "
                    "'key : value' in line: " + line, FL_AT);
        }
        bool output, range;
        std::string value = line.substr(index + 1);
        std::vector<std::string> flags = fl::Op::split(value, "|");
        if (flags.size() == 1) {
            std::string flag = fl::Op::trim(flags.front());
            output = (flag == "VALID");
            range = (flag == "RANGE");
            if (not (output or range)) {
                throw fl::Exception("[syntax error] expected locking flags "
                        "<VALID|RANGE>, but found <" + flag + "> in line: " + line, FL_AT);
            }
        } else if (flags.size() == 2) {
            std::string flagA = fl::Op::trim(flags.front());
            std::string flagB = fl::Op::trim(flags.back());
            output = (flagA == "VALID" or flagB == "VALID");
            range = (flagA == "RANGE" or flagB == "RANGE");
            if (not (output and range)) {
                throw fl::Exception("[syntax error] expected locking flags "
                        "<VALID|RANGE>, but found "
                        "<" + flags.front() + "|" + flags.back() + "> in line: " + line, FL_AT);
            }
        } else {
            throw fl::Exception("[syntax error] expected locking flags "
                    "<VALID|RANGE>, but found "
                    "<" + value + "> in line: " + line, FL_AT);
        }
        return std::pair<bool, bool>(output, range);
    }

    bool FclImporter::extractEnabled(const std::string& line) const {
        std::vector<std::string> tokens = Op::split(line, ":");
        if (tokens.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        std::string boolean = fl::Op::trim(tokens.at(1));
        if (boolean == "TRUE") return true;
        if (boolean == "FALSE") return false;
        throw fl::Exception("[syntax error] expected boolean <TRUE|FALSE>, but found <" + line + ">", FL_AT);
    }

}
