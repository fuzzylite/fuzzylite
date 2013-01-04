/* 
 * File:   FisImporter.cpp
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:54 PM
 */

#include "fl/imex/FisImporter.h"

#include "fl/Headers.h"

#include <sstream>
#include <iostream>


namespace fl {

    FisImporter::FisImporter() { }

    FisImporter::~FisImporter() { }

    std::string FisImporter::name() const {
        return "FisImporter";
    }

    Engine* FisImporter::fromString(const std::string& fis) const {
        Engine* engine = new Engine;

        std::istringstream fisReader(fis);
        std::string line;
        int lineNumber = 0;

        std::vector<std::string> sections;
        while (std::getline(fisReader, line)) {
            ++lineNumber;
            line = Op::trim(line);
            line = fl::Op::findReplace(line, "'", "");
            if (line.empty() or line[0] == '#')
                continue;

            if ("[System]" == line.substr(0, std::string("[System]").size())
                    or "[Input" == line.substr(0, std::string("[Input").size())
                    or "[Output" == line.substr(0, std::string("[Output").size())
                    or "[Rules]" == line.substr(0, std::string("[Rules]").size())) {
                sections.push_back(line);
            } else {
                if (sections.size() == 0) {
                    FL_LOG("[importer warning] ignoring line because it does not "
                            "belong to any section: " << line);
                } else {
                    sections[sections.size() - 1] += "\n" + line;
                }
            }
        }
        std::string andMethod, orMethod, impMethod, aggMethod, defuzzMethod;
        try {
            for (std::size_t i = 0; i < sections.size(); ++i) {
                if ("[System]" == sections[i].substr(0, std::string("[System]").size()))
                    importSystem(sections[i], engine,
                        andMethod, orMethod, impMethod, aggMethod, defuzzMethod);
                else if ("[Input" == sections[i].substr(0, std::string("[Input").size()))
                    importInput(sections[i], engine);
                else if ("[Output" == sections[i].substr(0, std::string("[Output").size()))
                    importOutput(sections[i], engine);
                else if ("[Rules]" == sections[i].substr(0, std::string("[Rules]").size()))
                    importRules(sections[i], engine);
                else
                    throw fl::Exception("[internal error] unable to parse section: "
                        + sections[i], FL_AT);
            }
            engine->configure(flTnorm(andMethod), flSnorm(orMethod),
                    flTnorm(impMethod), flSnorm(aggMethod),
                    flDefuzzifier(defuzzMethod));
        } catch (fl::Exception& ex) {
            delete engine;
            throw ex;
        }

        return engine;
    }

    void FisImporter::importSystem(const std::string& section, Engine * engine,
            std::string& andMethod, std::string& orMethod,
            std::string& impMethod, std::string& aggMethod,
            std::string& defuzzMethod) const {
        std::istringstream reader(section);
        std::string line;
        std::getline(reader, line); //ignore first line [System]
        while (std::getline(reader, line)) {
            std::vector<std::string> keyValue = fl::Op::split(line, "=");
            if (keyValue.size() != 2)
                throw fl::Exception("[syntax error] expected a property of type "
                    "'key=value', but found < " + line + ">", FL_AT);
            std::string key = fl::Op::trim(keyValue[0]);
            std::string value = fl::Op::trim(keyValue[1]);

            if (key == "Name") engine->setName(value);
            else if (key == "Type") {
                if (value != "mamdani")
                    throw fl::Exception("[error] fuzzylite supports only mamdani-type "
                        "engines at the moment", FL_AT);

            } else if (key == "AndMethod") andMethod = value;
            else if (key == "OrMethod") orMethod = value;
            else if (key == "ImpMethod") impMethod = value;
            else if (key == "AggMethod") aggMethod = value;
            else if (key == "DefuzzMethod") defuzzMethod = value;
            else FL_LOG("[info] ignoring redundant or non-relevant information "
                    "from line: " << line);
        }
    }

    void FisImporter::importInput(const std::string& section, Engine* engine) const {
        std::istringstream reader(section);
        std::string line;
        std::getline(reader, line); //ignore first line [Input#]

        InputVariable* input = new InputVariable;
        engine->addInputVariable(input);

        while (std::getline(reader, line)) {
            std::vector<std::string> keyValue = fl::Op::split(line, "=");
            if (keyValue.size() != 2)
                throw fl::Exception("[syntax error] expected a property of type "
                    "'key=value', but found < " + line + ">", FL_AT);
            std::string key = fl::Op::trim(keyValue[0]);
            std::string value = fl::Op::trim(keyValue[1]);

            if (key == "Name") input->setName(value);
            else if (key == "Range") {
                scalar minimum, maximum;
                extractRange(value, minimum, maximum);
                input->setMinimum(minimum);
                input->setMaximum(maximum);
            } else if (key.substr(0, 2) == "MF") {
                input->addTerm(extractTerm(value));
            } else {
                FL_LOG("[info] ignoring redundant or non-relevant information from line: " << line);
            }
        }
    }

    void FisImporter::importOutput(const std::string& section, Engine* engine) const {
        std::istringstream reader(section);
        std::string line;
        std::getline(reader, line); //ignore first line [Output#]

        OutputVariable* output = new OutputVariable;
        engine->addOutputVariable(output);


        while (std::getline(reader, line)) {
            std::vector<std::string> keyValue = fl::Op::split(line, "=");
            if (keyValue.size() != 2)
                throw fl::Exception("[syntax error] expected a property of type "
                    "'key=value', but found < " + line + ">", FL_AT);
            std::string key = fl::Op::trim(keyValue[0]);
            std::string value = fl::Op::trim(keyValue[1]);

            if (key == "Name") output->setName(value);
            else if (key == "Range") {
                scalar minimum, maximum;
                extractRange(value, minimum, maximum);
                output->setMinimum(minimum);
                output->setMaximum(maximum);
            } else if (key.substr(0, 2) == "MF") {
                output->addTerm(extractTerm(value));
            } else if (key == "Default") {
                output->setDefaultValue(fl::Op::toScalar(value));
            } else if (key == "Lock") {
                output->setLockDefuzzifiedValue((int) fl::Op::toScalar(value) == 1);
            } else {
                FL_LOG("[info] ignoring redundant or non-relevant information from line: " << line);
            }
        }
    }

    void FisImporter::importRules(const std::string& section, Engine* engine) const {
        std::istringstream reader(section);
        std::string line;
        std::getline(reader, line); //ignore first line [Rules]

        engine->addHedge(new Any);
        engine->addHedge(new Extremely);
        engine->addHedge(new Not);
        engine->addHedge(new Seldom);
        engine->addHedge(new Somewhat);
        engine->addHedge(new Very);

        RuleBlock* ruleblock = new RuleBlock;
        engine->addRuleBlock(ruleblock);

        while (std::getline(reader, line)) {
            std::vector<std::string> inputsAndRest = fl::Op::split(line, ",");
            if (inputsAndRest.size() != 2)
                throw fl::Exception("[syntax error] expected rule to match pattern "
                    "<'i '+, 'o '+ (w) : '1|2'>, but found instead <" + line + ">", FL_AT);

            std::vector <std::string> outputsAndRest = fl::Op::split(inputsAndRest[1], ":");
            if (outputsAndRest.size() != 2)
                throw fl::Exception("[syntax error] expected rule to match pattern "
                    "<'i '+, 'o '+ (w) : '1|2'>, but found instead <" + line + ">", FL_AT);

            std::vector<std::string> inputs = fl::Op::split(inputsAndRest[0], " ");
            std::vector<std::string> outputs = fl::Op::split(outputsAndRest[0], " ");
            std::string weightInParenthesis = outputs[outputs.size() - 1];
            outputs.erase(outputs.begin() + outputs.size() - 1);
            std::string connector = fl::Op::trim(outputsAndRest[1]);

            if ((int) inputs.size() != engine->numberOfInputVariables())
                throw fl::Exception("[syntax error] missing input variables in rule <"
                    + line + ">", FL_AT);

            if ((int) outputs.size() != engine->numberOfOutputVariables())
                throw fl::Exception("[syntax error] missing output variables in rule <"
                    + line + ">", FL_AT);

            std::vector<std::string> antecedent, consequent;

            for (std::size_t i = 0; i < inputs.size(); ++i) {
                std::ostringstream ss;
                scalar inputCode = fl::Op::toScalar(inputs[i]);
                if (fl::Op::isEq(inputCode, 0.0)) continue;
                ss << engine->getInputVariable(i)->getName() << " "
                        << fl::Rule::FL_IS << " "
                        << translateProposition(inputCode, engine->getInputVariable(i));
                antecedent.push_back(ss.str());
            }

            for (std::size_t i = 0; i < outputs.size(); ++i) {
                std::ostringstream ss;
                scalar outputCode = fl::Op::toScalar(outputs[i]);
                if (fl::Op::isEq(outputCode, 0.0)) continue;
                ss << engine->getOutputVariable(i)->getName() << " "
                        << fl::Rule::FL_IS << " "
                        << translateProposition(outputCode, engine->getOutputVariable(i));
                consequent.push_back(ss.str());
            }

            std::ostringstream rule;

            rule << fl::Rule::FL_IF << " ";
            for (std::size_t i = 0; i < antecedent.size(); ++i) {
                rule << antecedent[i];
                if (i < antecedent.size() - 1) {
                    rule << " ";
                    if (connector == "1") rule << fl::Rule::FL_AND << " ";
                    else if (connector == "2") rule << fl::Rule::FL_OR << " ";
                    else throw fl::Exception("[syntax error] connector <"
                            + connector + "> not recognized", FL_AT);
                }
            }

            rule << " " << fl::Rule::FL_THEN << " ";
            for (std::size_t i = 0; i < consequent.size(); ++i) {
                rule << consequent[i];
                if (i < consequent.size() - 1) {
                    rule << " " << fl::Rule::FL_AND << " ";
                }
            }

            std::ostringstream ss;
            for (std::size_t i = 0; i < weightInParenthesis.size(); ++i) {
                if (weightInParenthesis[i] == '('
                        or weightInParenthesis[i] == ')'
                        or weightInParenthesis[i] == ' ') continue;
                ss << weightInParenthesis[i];
            }

            scalar weight = fl::Op::toScalar(ss.str());
            if (not fl::Op::isEq(weight, 1.0))
                rule << " " << fl::Rule::FL_WITH << " " << weight;

            FL_LOG("Rule: " << rule.str());

            ruleblock->addRule(fl::MamdaniRule::parse(rule.str(), engine));
        }
    }

    std::string FisImporter::translateProposition(scalar code, Variable* variable) const {
        scalar intPartScalar;
        scalar fracPartScalar = std::modf(code, &intPartScalar);

        int intPart = (int) std::abs(intPartScalar) - 1;
        int fracPart = (int) std::abs(fracPartScalar);

        if (intPart < 0 or intPart >= variable->numberOfTerms()) {
            std::ostringstream ex;
            ex << "[syntax error] the code <" << code << "> refers to a term "
                    "out of range from variable <" << variable->toString() << ">";
            throw fl::Exception(ex.str(), FL_AT);
        }

        std::ostringstream ss;
        if (code < 0) ss << Not().name() << " ";
        if (fracPart == 5) ss << Somewhat().name() << " ";
        else if (fracPart == 20) ss << Very().name() << " ";
        else if (fracPart == 30) ss << Extremely().name() << " ";
        else if (fracPart == 40) ss << Very().name() << " " << Very().name() << " ";
        else if (fracPart != 0)
            throw fl::Exception("[syntax error] no hedge defined in fis format for <"
                + fl::Op::str(fracPartScalar) + ">", FL_AT);

        ss << variable->getTerm(intPart)->getName();
        return ss.str();
    }

    std::string FisImporter::flTnorm(const std::string & name) const {
        if (name == "min") return "Minimum";
        if (name == "prod") return "AlgebraicProduct";
        if (name == "bounded_difference") return "BoundedDifference";
        if (name == "drastic_product") return "DrasticProduct";
        if (name == "einstein_product") return "EinsteinProduct";
        if (name == "hamacher_product") return "HamacherProduct";

        throw fl::Exception("[syntax error] T-Norm <" + name + "> not recognized", FL_AT);
    }

    std::string FisImporter::flSnorm(const std::string & name) const {
        if (name == "max") return "Maximum";
        if (name == "sum") return "AlgebraicSum";
        if (name == "bounded_sum") return "BoundedSum";
        if (name == "normalized_sum") return "NormalizedSum";
        if (name == "drastic_sum") return "DrasticSum";
        if (name == "einstein_sum") return "EinsteinSum";
        if (name == "hamacher_sum") return "HamacherSum";

        throw fl::Exception("[syntax error] S-Norm <" + name + "> not recognized", FL_AT);
    }

    std::string FisImporter::flDefuzzifier(const std::string & name) const {
        if (name == "centroid") return "CenterOfGravity";
        if (name == "som") return "SmallestOfMaximum";
        if (name == "lom") return "LargestOfMaximum";
        if (name == "mom") return "MeanOfMaximum";
        throw fl::Exception("[syntax error] defuzzifier <" + name + "> not recognized", FL_AT);
    }

    Term * FisImporter::extractTerm(const std::string & fis) const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < fis.size(); ++i) {
            if (not (fis[i] == '[' or fis[i] == ']')) {
                ss << fis[i];
            }
        }
        std::string line = ss.str();

        std::vector<std::string> nameTerm = fl::Op::split(line, ":");
        if (nameTerm.size() != 2) {
            throw fl::Exception("[syntax error] expected term in format 'name':'class',[params], "
                    "but found " + line, FL_AT);
        }
        std::vector<std::string> termParams = fl::Op::split(nameTerm[1], ",");
        if (termParams.size() != 2) {
            throw fl::Exception("[syntax error] expected term in format 'name':'class',[params], "
                    "but found " + line, FL_AT);
        }

        std::vector<std::string> strParams = fl::Op::split(termParams[1], " ");
        std::vector<scalar> params;
        for (std::size_t i = 0; i < strParams.size(); ++i) {
            params.push_back(fl::Op::toScalar(strParams[i]));
        }

        return createInstance(termParams[0], nameTerm[0], params);
    }

    Term * FisImporter::createInstance(const std::string& termClass,
            const std::string& name, const std::vector<scalar>& params) const {
        int requiredParams = 0;

        if (termClass == "discretemf") {
            if (params.size() % 2 == 0) {
                Discrete* term = new Discrete(name);
                for (std::size_t i = 0; i < params.size() - 1; i += 2) {
                    term->x.push_back(params[i]);
                    term->y.push_back(params[i + 1]);
                }
                return term;
            } else {
                std::ostringstream ex;
                ex << "[syntax error] a discrete term requires an even list of values, "
                        "but found <" << params.size() << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

        if (termClass == "gbellmf") {
            if (params.size() >= (requiredParams = 3)) {
                return new Bell(name, params[2], params[0], params[1]);
            }
        }

        if (termClass == "gaussmf") {
            if (params.size() >= (requiredParams = 2)) {
                return new Gaussian(name, params[1], params[0]);
            }
        }

        if (termClass == "gauss2mf") {
            if (params.size() >= (requiredParams = 4)) {
                return new GaussianProduct(name, params[1], params[0], params[3], params[2]);
            }
        }

        if (termClass == "pimf") {
            if (params.size() >= (requiredParams = 4)) {
                return new PiShape(name, params[0], params[1], params[2], params[3]);
            }
        }

        if (termClass == "rampmf") {
            if (params.size() >= (requiredParams = 2)) {
                return new Ramp(name, params[0], params[1]);
            }
        }


        if (termClass == "rectmf") {
            if (params.size() >= (requiredParams = 2)) {
                return new Rectangle(name, params[0], params[1]);
            }
        }

        if (termClass == "smf") {
            if (params.size() >= (requiredParams = 2)) {
                return new SShape(name, params[0], params[1]);
            }
        }


        if (termClass == "sigmf") {
            if (params.size() >= (requiredParams = 2)) {
                return new Sigmoid(name, params[1], params[0]);
            }
        }
        if (termClass == "dsigmf") {
            if (params.size() >= (requiredParams = 4)) {
                return new SigmoidDifference(name, params[1], params[0], params[3], params[2]);
            }
        }
        if (termClass == "psigmf") {
            if (params.size() >= (requiredParams = 4)) {
                return new SigmoidProduct(name, params[1], params[0], params[3], params[2]);
            }
        }

        if (termClass == "trapmf") {
            if (params.size() >= (requiredParams = 4))
                return new Trapezoid(name, params[0], params[1], params[2], params[3]);
        }

        if (termClass == "trimf") {
            if (params.size() >= (requiredParams = 3))
                return new Triangle(name, params[0], params[1], params[2]);
        }

        if (termClass == "zmf") {
            if (params.size() >= (requiredParams = 2)) {
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
        throw fl::Exception(ex.str(), FL_AT);
    }

    void FisImporter::extractRange(const std::string& range, scalar& minimum, scalar & maximum) const {
        std::vector<std::string> parts = fl::Op::split(range, " ");
        if (parts.size() != 2)
            throw fl::Exception("[syntax error] expected range in format '[begin end]',"
                " but found <" + range + ">", FL_AT);
        std::string begin = parts[0], end = parts[1];
        if (begin[0] != '[' or end[end.size() - 1] != ']')
            throw fl::Exception("[syntax error] expected range in format '[begin end]',"
                " but found <" + range + ">", FL_AT);
        minimum = fl::Op::toScalar(begin.substr(1));
        maximum = fl::Op::toScalar(end.substr(0, end.size() - 1));
    }




}