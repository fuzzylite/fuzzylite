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
                if (line.empty() or line[0] == '#')
                    continue;

                std::istringstream tokenizer(line);
                std::string firstToken;
                tokenizer >> firstToken;

                if (firstToken == "FUNCTION_BLOCK") {
                    if (tokenizer.rdbuf()->in_avail() > 0) {
                        std::string name;
                        tokenizer >> name;
                        engine->setName(name);
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
            std::string name = Op::trim(token[0]);
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
            name = line.substr(index + 1);
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
                    inputVariable->addTerm(extractTerm(line));
                } else throw fl::Exception("[syntax error] token <" + firstToken + " not recognized", FL_AT);
            } catch (fl::Exception& ex) {
                ex.addDetail("At line: <" + line + ">");
                throw ex;
            }
        }

    }

    void FclImporter::processDefuzzify(const std::string& block, Engine* engine)const {
        std::istringstream blockReader(block);
        std::string line;

        std::getline(blockReader, line);
        std::string name;
        std::size_t index = line.find_first_of(' ');
        if (index != std::string::npos) {
            name = line.substr(index + 1);
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
            std::string firstToken = line.substr(0, line.find_first_of(' '));
            if (firstToken == "TERM") {
                outputVariable->addTerm(extractTerm(line));
            } else if (firstToken == "METHOD") {
                outputVariable->setDefuzzifier(extractDefuzzifier(line));
            } else if (firstToken == "ACCU") {
                outputVariable->output()->setAccumulation(extractSNorm(line));
            } else if (firstToken == "DEFAULT") {
                bool lockDefuzzifiedValue;
                outputVariable->setDefaultValue(extractDefaultValue(line, lockDefuzzifiedValue));
                outputVariable->setLockDefuzzifiedValue(lockDefuzzifiedValue);
            } else if (firstToken == "RANGE") {
                scalar minimum, maximum;
                extractRange(line, minimum, maximum);
                outputVariable->setMinimum(minimum);
                outputVariable->setMaximum(maximum);
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
        RuleBlock* ruleblock = new RuleBlock(name);
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
                ruleblock->addRule(MamdaniRule::parse(rule, engine));
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
        std::string name = Op::findReplace(Op::trim(token[1]), ";", "");
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
            ex.addDetail("[syntax error] T-Norm <" + name + "> not recognized in line:\n"
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
        std::string name = Op::findReplace(Op::trim(token[1]), ";", "");
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
            ex.addDetail("[syntax error] S-Norm <" + name + "> not recognized in line:\n"
                    + line, FL_AT);
            throw ex;
        }
    }

    Term* FclImporter::extractTerm(const std::string& line) const {
        std::ostringstream spacer;
        for (std::size_t i = 0; i < line.size(); ++i) {
            if (line[i] == '(' or line[i] == ')' or line[i] == ',') {
                spacer << " " << line[i] << " ";
            } else if (line[i] == ':') {
                spacer << " :";
            } else if (line[i] == '=') {
                spacer << "= ";
            } else
                spacer << line[i];
        }
        std::string spacedLine = spacer.str();

        enum FSM {
            S_KWTERM, S_NAME, S_ASSIGN, S_TERMCLASS, S_PARAMETERS
        };
        FSM state = S_KWTERM;
        std::istringstream tokenizer(spacedLine);
        std::string token;
        std::string name, termClass;
        std::vector<scalar> parameters;
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
                if (token != "(") termClass = token;
                state = S_PARAMETERS;
                continue;
            }
            if (state == S_PARAMETERS) {
                if (token == "(" or token == ")" or token == ",")
                    continue;
                if (token == ";") break;
                scalar parameter;
                try {
                    parameter = Op::toScalar(token);
                } catch (...) {
                    std::ostringstream ex;
                    ex << "[syntax error] expected numeric value, but found <"
                            << token << "> in line: " << line;
                    throw fl::Exception(ex.str(), FL_AT);
                }
                parameters.push_back(parameter);
            }
        }
        try {
            if (termClass.empty()) termClass = Discrete().className();
            Term* result = Factory::instance()->term()->create(termClass, parameters);
            result->setName(name);
            return result;
        } catch (fl::Exception& ex) {
            ex.addCall(FL_AT);
            throw ex;
        }
    }

    Defuzzifier* FclImporter::extractDefuzzifier(const std::string& line) const {
        std::vector<std::string> token = Op::split(line, ":");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        std::string name = Op::trim(Op::findReplace(token[1], ";", ""));
        std::string className = name;
        if (name == "COG") className = Centroid().className();
        if (name == "SOM") className = SmallestOfMaximum().className();
        if (name == "LOM") className = LargestOfMaximum().className();
        if (name == "MOM") className = MeanOfMaximum().className();

        try {
            return Factory::instance()->defuzzifier()->create(className);
        } catch (fl::Exception& ex) {
            ex.addDetail("[syntax error] defuzzifier <" + name +
                    "> not recognized in line:\n" + line, FL_AT);
            throw ex;
        }
    }

    scalar FclImporter::extractDefaultValue(const std::string& line, bool& lockDefuzzifiedValue) const {
        std::vector<std::string> token = Op::split(line, ":=");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key := value) in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        std::string defaultValue = Op::findReplace(token[1], " ", "");
        token = Op::split(defaultValue, "|");

        scalar value;
        try {
            value = Op::toScalar(token[0]);
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, "
                    << "but found <" << token[0] << "> in line: "
                    << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

        lockDefuzzifiedValue = false;

        if (token.size() == 2) {
            std::string noChangeFlag = Op::findReplace(Op::trim(token[1]), ";", "");
            if (noChangeFlag == "NC")
                lockDefuzzifiedValue = true;
            else {
                std::ostringstream ex;
                ex << "expected keyword <NC>, but found<" << noChangeFlag << "> in "
                        << "line: " << line;
                throw fl::Exception(ex.str(), FL_AT);
            }
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

        std::ostringstream range;
        for (std::size_t i = 0; i < token[1].size(); ++i) {
            char character = token[1][i];
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
            minimum = Op::toScalar(token[0]);
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, but found <" << token[0] << "> in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }
        try {
            maximum = Op::toScalar(token[1]);
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, but found <" << token[1] << "> in "
                    << "line: " << line;
            throw fl::Exception(ex.str(), FL_AT);
        }

    }

    //    Term* FclImporter::createInstance(const std::string& termClass,
    //            const std::string& name, const std::vector<scalar>& params) const {
    //        int requiredParams = 0;
    //
    //        if (termClass.empty() or termClass == Discrete().className()) {
    //            if (params.size() % 2 == 0) {
    //                Discrete* term = new Discrete(name);
    //                for (std::size_t i = 0; i < params.size() - 1; i += 2) {
    //                    term->x.push_back(params[i]);
    //                    term->y.push_back(params[i + 1]);
    //                }
    //                return term;
    //            } else {
    //                std::ostringstream ex;
    //                ex << "[syntax error] a discrete term requires an even list of values, "
    //                        "but found <" << params.size() << "> values";
    //                throw fl::Exception(ex.str(), FL_AT);
    //            }
    //        }
    //
    //        if (termClass == Bell().className()) {
    //            if (params.size() == (requiredParams = 3)) {
    //                return new Bell(name, params[0], params[1], params[2]);
    //            }
    //        }
    //
    //        if (termClass == Gaussian().className()) {
    //            if (params.size() == (requiredParams = 2)) {
    //                return new Gaussian(name, params[0], params[1]);
    //            }
    //        }
    //
    //        if (termClass == GaussianProduct().className()) {
    //            if (params.size() == (requiredParams = 4)) {
    //                return new GaussianProduct(name, params[0], params[1], params[2], params[3]);
    //            }
    //        }
    //
    //        if (termClass == PiShape().className()) {
    //            if (params.size() == (requiredParams = 4)) {
    //                return new PiShape(name, params[0], params[1], params[2], params[3]);
    //            }
    //        }
    //
    //        if (termClass == Ramp().className()) {
    //            if (params.size() == (requiredParams = 2)) {
    //                return new Ramp(name, params[0], params[1]);
    //            }
    //        }
    //
    //
    //        if (termClass == Rectangle().className()) {
    //            if (params.size() == (requiredParams = 2)) {
    //                return new Rectangle(name, params[0], params[1]);
    //            }
    //        }
    //
    //        if (termClass == SShape().className()) {
    //            if (params.size() == (requiredParams = 2)) {
    //                return new SShape(name, params[0], params[1]);
    //            }
    //        }
    //
    //
    //        if (termClass == Sigmoid().className()) {
    //            if (params.size() == (requiredParams = 2)) {
    //                return new Sigmoid(name, params[0], params[1]);
    //            }
    //        }
    //        if (termClass == SigmoidDifference().className()) {
    //            if (params.size() == (requiredParams = 4)) {
    //                return new SigmoidDifference(name, params[0], params[1], params[2], params[3]);
    //            }
    //        }
    //        if (termClass == SigmoidProduct().className()) {
    //            if (params.size() == (requiredParams = 4)) {
    //                return new SigmoidProduct(name, params[0], params[1], params[2], params[3]);
    //            }
    //        }
    //
    //        if (termClass == Trapezoid().className()) {
    //            if (params.size() == (requiredParams = 4))
    //                return new Trapezoid(name, params[0], params[1], params[2], params[3]);
    //        }
    //
    //        if (termClass == Triangle().className()) {
    //            if (params.size() == (requiredParams = 3))
    //                return new Triangle(name, params[0], params[1], params[2]);
    //        }
    //
    //        if (termClass == ZShape().className()) {
    //            if (params.size() == (requiredParams = 2)) {
    //                return new ZShape(name, params[0], params[1]);
    //            }
    //        }
    //
    //
    //        std::ostringstream ex;
    //        if (requiredParams != 0) {
    //            ex << "[syntax error] " << termClass << " requires "
    //                    << requiredParams << " parameters";
    //        } else {
    //            ex << "[syntax error] term of class <" << termClass << "> not recognized";
    //        }
    //        throw fl::Exception(ex.str(), FL_AT);
    //    }



}
