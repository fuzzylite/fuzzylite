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

    std::string FclImporter::name() const{
        return "FclImporter";
    }
    
    Engine* FclImporter::fromString(const std::string& fcl) {
        _engine = new Engine;
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
        while (std::getline(fclReader, line)) {
            ++lineNumber;
            line = Op::Trim(line);
            if (line.empty() or line[0] == '#')
                continue;

            std::istringstream tokenizer(line);
            std::string firstToken;
            tokenizer >> firstToken;

            if (firstToken == "FUNCTION_BLOCK") {
                if (tokenizer.rdbuf()->in_avail() > 0) {
                    std::string name;
                    tokenizer >> name;
                    this->_engine->setName(name);
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
                            << "> " << " in line " << lineNumber << ": "
                            << std::endl << line;
                    throw fl::Exception(ex.str());
                }
                currentTag = tagFinder->first;
                closingTag = tagFinder->second;
                block.clear();
                block.str("");
                block << line << std::endl;
                continue;
            }

            if (not currentTag.empty()) {
                if (firstToken == closingTag) {
                    processBlock(currentTag, block.str());
                    currentTag = "";
                    closingTag = "";
                } else if (tags.find(firstToken) != tags.end()) {
                    //if opening new block without closing the previous one
                    std::ostringstream ex;
                    ex << "[syntax error] expected <" << closingTag << "> before <"
                            << firstToken << "> in line: " << std::endl << line;
                    throw fl::Exception(ex.str());
                } else {
                    block << line << std::endl;
                }
                continue;
            }
        }

        if (not currentTag.empty()) {
            std::ostringstream ex;
            ex << "[syntax error] ";
            if (block.rdbuf()->in_avail() > 0) {
                ex << "expected <" << closingTag << "> for block:" << std::endl
                        << block.str();
            } else {
                ex << "expected <" << closingTag << ">, but not found";
            }
            throw fl::Exception(ex.str());
        }
        return _engine;
    }

    void FclImporter::processBlock(const std::string& tag, const std::string& block) {
        if (tag == "VAR_INPUT" or tag == "VAR_OUTPUT") {
            processVar(tag, block);
        } else if (tag == "FUZZIFY") {
            processFuzzify(block);
        } else if (tag == "DEFUZZIFY") {
            processDefuzzify(block);
        } else if (tag == "RULEBLOCK") {
            processRuleBlock(block);
        } else {
            std::ostringstream ex;
            ex << "[syntax error] unexpected tag <" << tag << "> for block:"
                    << std::endl << block;
            throw fl::Exception(ex.str());
        }
    }

    void FclImporter::processVar(const std::string& tag, const std::string& block) {
        std::istringstream blockReader(block);
        std::string line;

        std::getline(blockReader, line); //discard first line as it is VAR_INPUT
        while (getline(blockReader, line)) {
            std::vector<std::string> token = Op::Split(line, ":");
            if (token.size() != 2) {
                std::ostringstream ex;
                ex << "[syntax error] expected property of type (key : value) in line:"
                        << std::endl << line;
                throw fl::Exception(ex.str());
            }
            std::string name = Op::Trim(token[0]);
            if (tag == "VAR_INPUT")
                this->_engine->addInputVariable(new InputVariable(name));
            else if (tag == "VAR_OUTPUT")
                this->_engine->addOutputVariable(new OutputVariable(name));
            else {
                std::ostringstream ex;
                ex << "[syntax error] unexpected tag <" << tag << "> in line:"
                        << std::endl << line;
                throw fl::Exception(ex.str());
            }
        }
    }

    void FclImporter::processFuzzify(const std::string& block) {
        std::istringstream blockReader(block);
        std::string line;

        std::getline(blockReader, line);
        std::string name;
        std::size_t index = line.find_first_of(' ');
        if (index != std::string::npos) {
            name = line.substr(index + 1);
        } else {
            std::ostringstream ex;
            ex << "[syntax error] expected an input variable name in line:"
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }
        if (not this->_engine->hasInputVariable(name)) {
            std::ostringstream ex;
            ex << "[syntax error] input variable <" << name
                    << "> not registered in engine. "
                    << "Line: " << std::endl << line;
            throw fl::Exception(ex.str());
        }

        InputVariable* inputVariable = this->_engine->getInputVariable(name);
        while (std::getline(blockReader, line)) {
            inputVariable->addTerm(extractTerm(line));
        }

    }

    void FclImporter::processDefuzzify(const std::string& block) {
        std::istringstream blockReader(block);
        std::string line;

        std::getline(blockReader, line);
        std::string name;
        std::size_t index = line.find_first_of(' ');
        if (index != std::string::npos) {
            name = line.substr(index + 1);
        } else {
            std::ostringstream ex;
            ex << "[syntax error] expected an output variable name in line: "
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }
        if (not this->_engine->hasOutputVariable(name)) {
            std::ostringstream ex;
            ex << "[syntax error] output variable <" << name
                    << "> not registered in engine. "
                    << "Line: " << std::endl << line;
            throw fl::Exception(ex.str());
        }

        OutputVariable* outputVariable = this->_engine->getOutputVariable(name);
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
                        "> in line: " << std::endl << line;
                throw fl::Exception(ex.str());
            }
        }

    }

    void FclImporter::processRuleBlock(const std::string& block) {
        std::istringstream blockReader(block);
        std::string line;

        std::string name;
        std::getline(blockReader, line);
        std::size_t index = line.find_last_of(' ');
        if (index != std::string::npos) name = line.substr(index);
        RuleBlock* ruleblock = new RuleBlock(name);
        this->_engine->addRuleBlock(ruleblock);

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
                ruleblock->addRule(MamdaniRule::parse(rule, this->_engine));
            } else {
                std::ostringstream ex;
                ex << "[syntax error] keyword <" << firstToken
                        << "> not recognized in line: " << std::endl << line;
                throw fl::Exception(ex.str());
            }

        }
    }

    TNorm* FclImporter::extractTNorm(const std::string& line) const {
        std::vector<std::string> token = Op::Split(line, ":");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in line: "
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }
        std::string name = Op::FindReplace(Op::Trim(token[1]), ";", "");
        if (name == "MIN") return new Minimum;
        if (name == "PROD") return new AlgebraicProduct;
        if (name == "BDIF") return new BoundedDifference;
        if (name == "DPROD") return new DrasticProduct;
        if (name == "EPROD") return new EinsteinProduct;
        if (name == "HPROD") return new HamacherProduct;

        std::ostringstream ex;
        ex << "[syntax error] T-Norm <" << name << "> not recognized in line:"
                << std::endl << line;
        throw fl::Exception(ex.str());
    }

    SNorm* FclImporter::extractSNorm(const std::string& line) const {
        std::vector<std::string> token = Op::Split(line, ":");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in line: "
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }
        std::string name = Op::FindReplace(Op::Trim(token[1]), ";", "");
        if (name == "MAX") return new Maximum;
        if (name == "ASUM") return new AlgebraicSum;
        if (name == "BSUM") return new BoundedSum;
        if (name == "NSUM") return new NormalizedSum;
        if (name == "DSUM") return new DrasticSum;
        if (name == "ESUM") return new EinsteinSum;
        if (name == "HSUM") return new HamacherSum;

        std::ostringstream ex;
        ex << "[syntax error] S-Norm <" << name << "> not recognized in line:"
                << std::endl << line;
        throw fl::Exception(ex.str());
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
                termClass = token;
                state = S_PARAMETERS;
                continue;
            }
            if (state == S_PARAMETERS) {
                if (token == "(" or token == ")" or token == ",")
                    continue;
                if (token == ";") break;
                scalar parameter;
                try {
                    parameter = Op::Scalar(token);
                } catch (...) {
                    std::ostringstream ex;
                    ex << "[syntax error] expected numeric value, but found <"
                            << token << "> in line: " << std::endl << line;
                    throw fl::Exception(ex.str());
                }
                parameters.push_back(parameter);
            }

        }
        return createInstance(termClass, name, parameters);
    }

    Term* FclImporter::createInstance(const std::string& termClass,
            const std::string& name, const std::vector<scalar>& params) const {
        int requiredParams = 0;

        if (termClass == Bell().className()) {
            if (params.size() == (requiredParams = 3)) {
                return new Bell(name, params[0], params[1], params[2]);
            }
        }

        if (termClass == Discrete().className()) {
            if (params.size() % 2 == 0) {
                Discrete* term = new Discrete(name);
                for (std::size_t i = 0; i < params.size() - 1; i += 2) {
                    term->x.push_back(params[i]);
                    term->y.push_back(params[i + 1]);
                }
            }
            requiredParams = 2;
        }

        if (termClass == Gaussian().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Gaussian(name, params[0], params[1]);
            }
        }


        if (termClass == PiShape().className()) {
            if (params.size() == (requiredParams = 4)) {
                return new PiShape(name, params[0], params[1]);
            }
        }

        if (termClass == Ramp().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Ramp(name, params[0], params[1]);
            }
        }


        if (termClass == Rectangle().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Rectangle(name, params[0], params[1]);
            }
        }

        if (termClass == SShape().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new SShape(name, params[0], params[1]);
            }
        }


        if (termClass == Sigmoid().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new Sigmoid(name, params[0], params[1]);
            }
        }

        if (termClass == Trapezoid().className()) {
            if (params.size() == (requiredParams = 4))
                return new Trapezoid(name, params[0], params[1], params[2], params[3]);
        }

        if (termClass == Triangle().className()) {
            if (params.size() == (requiredParams = 3))
                return new Triangle(name, params[0], params[1], params[2]);
        }

        if (termClass == ZShape().className()) {
            if (params.size() == (requiredParams = 2)) {
                return new ZShape(name, params[0], params[1]);
            }
        }


        std::ostringstream ex;
        if (requiredParams != 0) {
            ex << "[syntax error] " << termClass << " requires "
                    << requiredParams << " parameters";
        } else {
            ex << "[syntax error] term of class <" << termClass << "> not recognized";
        }
        throw fl::Exception(ex.str());
    }

    Defuzzifier* FclImporter::extractDefuzzifier(const std::string& line) const {
        std::vector<std::string> token = Op::Split(line, ":");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key : value) in "
                    << "line: " << std::endl << line;
            throw fl::Exception(ex.str());
        }

        std::string name = Op::Trim(Op::FindReplace(token[1], ";", ""));
        if (name == "COG") return new CenterOfGravity;
        if (name == "SOM") return new SmallestOfMaximum;
        if (name == "LOM") return new LargestOfMaximum;
        if (name == "MOM") return new MeanOfMaximum;

        std::ostringstream ex;
        ex << "[syntax error] defuzzifier <" << name << "> not recognized in line:"
                << std::endl << line;
        throw fl::Exception(ex.str());
    }

    scalar FclImporter::extractDefaultValue(const std::string& line, bool& lockDefuzzifiedValue) const {
        std::vector<std::string> token = Op::Split(line, ":=");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key := value) in line: "
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }

        std::string defaultValue = Op::FindReplace(token[1], " ", "");
        token = Op::Split(defaultValue, "|");

        scalar value;
        try {
            value = Op::Scalar(token[0]);
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, "
                    << "but found <" << token[0] << "> in line: "
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }

        lockDefuzzifiedValue = false;

        if (token.size() == 2) {
            std::string noChangeFlag = Op::FindReplace(Op::Trim(token[1]), ";", "");
            if (noChangeFlag == "NC")
                lockDefuzzifiedValue = true;
            else {
                std::ostringstream ex;
                ex << "expected keyword <NC>, but found<" << noChangeFlag << "> in "
                        << "line: " << std::endl << line;
                throw fl::Exception(ex.str());
            }
        }
        return value;
    }

    void FclImporter::extractRange(const std::string& line, scalar& minimum, scalar& maximum) const {
        std::vector<std::string> token = Op::Split(line, ":=");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key := value) in line: "
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }

        std::ostringstream range;
        for (std::size_t i = 0; i < token[1].size(); ++i) {
            char character = token[1][i];
            if (character == '(' or character == ')' or character == ' ' or character == ';')
                continue;
            range << character;
        }

        token = Op::Split(range.str(), "..");
        if (token.size() != 2) {
            std::ostringstream ex;
            ex << "[syntax error] expected property of type (key .. value) in line: "
                    << std::endl << line;
            throw fl::Exception(ex.str());
        }

        try {
            minimum = Op::Scalar(token[0]);
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, but found <" << token[0] << "> in "
                    << "line: " << std::endl << line;
            throw fl::Exception(ex.str());
        }
        try {
            maximum = Op::Scalar(token[1]);
        } catch (...) {
            std::ostringstream ex;
            ex << "[syntax error] expected numeric value, but found <" << token[1] << "> in "
                    << "line: " << std::endl << line;
            throw fl::Exception(ex.str());
        }

    }


}
