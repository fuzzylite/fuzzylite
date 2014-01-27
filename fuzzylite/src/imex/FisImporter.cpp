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
 * File:   FisImporter.cpp
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:54 PM
 */

#include "fl/imex/FisImporter.h"

#include "fl/Headers.h"

#include <sstream>
#include <iostream>
#include <cctype>

namespace fl {

    FisImporter::FisImporter() {
    }

    FisImporter::~FisImporter() {
    }

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
            std::vector<std::string> comments;
            comments = Op::split(line, "//");
            if (comments.size() > 1) {
                line = comments.front();
            }
            comments = Op::split(line, "#");
            if (comments.size() > 1) {
                line = comments.front();
            }
            line = Op::trim(line);
            line = fl::Op::findReplace(line, "'", "");
            if (line.empty() or line.at(0) == '%')
                continue;

            if ("[System]" == line.substr(0, std::string("[System]").size())
                    or "[Input" == line.substr(0, std::string("[Input").size())
                    or "[Output" == line.substr(0, std::string("[Output").size())
                    or "[Rules]" == line.substr(0, std::string("[Rules]").size())) {
                sections.push_back(line);
            } else {
                if (not sections.empty()) {
                    sections.at(sections.size() - 1) += "\n" + line;
                } else {
                    throw fl::Exception("[import error] line  <" + line + "> "
                            "does not belong to any section", FL_AT);
                }
            }
        }
        std::string andMethod, orMethod, impMethod, aggMethod, defuzzMethod;
        try {
            for (std::size_t i = 0; i < sections.size(); ++i) {
                if ("[System]" == sections.at(i).substr(0, std::string("[System]").size()))
                    importSystem(sections.at(i), engine,
                        andMethod, orMethod, impMethod, aggMethod, defuzzMethod);
                else if ("[Input" == sections.at(i).substr(0, std::string("[Input").size()))
                    importInput(sections.at(i), engine);
                else if ("[Output" == sections.at(i).substr(0, std::string("[Output").size()))
                    importOutput(sections.at(i), engine);
                else if ("[Rules]" == sections.at(i).substr(0, std::string("[Rules]").size()))
                    importRules(sections.at(i), engine);
                else
                    throw fl::Exception("[import error] section <"
                        + sections.at(i) + "> not recognized", FL_AT);
            }
            engine->configure(tnorm(andMethod), snorm(orMethod),
                    tnorm(impMethod), snorm(aggMethod),
                    defuzzifier(defuzzMethod));
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

            std::string key = fl::Op::trim(keyValue.at(0));
            std::string value;
            for (std::size_t i = 1; i < keyValue.size(); ++i) {
                value += keyValue.at(i);
            }
            value = fl::Op::trim(value);
            if (key == "Name") engine->setName(value);
            else if (key == "AndMethod") andMethod = value;
            else if (key == "OrMethod") orMethod = value;
            else if (key == "ImpMethod") impMethod = value;
            else if (key == "AggMethod") aggMethod = value;
            else if (key == "DefuzzMethod") defuzzMethod = value;
            else if (key == "Type" or key == "Version"
                    or key == "NumInputs" or key == "NumOutputs"
                    or key == "NumRules" or key == "NumMFs") {
                //ignore because are redundant.
            } else throw fl::Exception("[import error] token <" + key + "> not recognized", FL_AT);
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
            std::string key = fl::Op::trim(keyValue.at(0));
            std::string value = fl::Op::trim(keyValue.at(1));

            if (key == "Name") input->setName(fl::Op::makeValidId(value));
            else if (key == "Enabled") {
                input->setEnabled(Op::isEq(Op::toScalar(value), 1.0));
            } else if (key == "Range") {
                std::pair<scalar, scalar> minmax = extractRange(value);
                input->setMinimum(minmax.first);
                input->setMaximum(minmax.second);
            } else if (key.substr(0, 2) == "MF") {
                input->addTerm(prepareTerm(extractTerm(value), engine));
            } else if (key == "NumMFs") {
                //ignore
            } else {
                throw fl::Exception("[import error] token <" + key + "> not recognized", FL_AT);
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
            std::string key = fl::Op::trim(keyValue.at(0));
            std::string value = fl::Op::trim(keyValue.at(1));

            if (key == "Name") output->setName(fl::Op::makeValidId(value));
            else if (key == "Enabled") {
                output->setEnabled(Op::isEq(Op::toScalar(value), 1.0));
            } else if (key == "Range") {
                std::pair<scalar, scalar> minmax = extractRange(value);
                output->setMinimum(minmax.first);
                output->setMaximum(minmax.second);
            } else if (key.substr(0, 2) == "MF") {
                output->addTerm(prepareTerm(extractTerm(value), engine));
            } else if (key == "Default") {
                output->setDefaultValue(fl::Op::toScalar(value));
            } else if (key == "LockValid") {
                output->setLockValidOutput((int) fl::Op::toScalar(value) == 1);
            } else if (key == "LockRange") {
                output->setLockOutputRange((int) fl::Op::toScalar(value) == 1);
            } else if (key == "NumMFs") {
                //ignore
            } else {
                throw fl::Exception("[import error] token <" + key + "> not recognized", FL_AT);
            }
        }
    }

    void FisImporter::importRules(const std::string& section, Engine* engine) const {
        std::istringstream reader(section);
        std::string line;
        std::getline(reader, line); //ignore first line [Rules]

        RuleBlock* ruleblock = new RuleBlock;
        engine->addRuleBlock(ruleblock);

        while (std::getline(reader, line)) {
            std::vector<std::string> inputsAndRest = fl::Op::split(line, ",");
            if (inputsAndRest.size() != 2)
                throw fl::Exception("[syntax error] expected rule to match pattern "
                    "<'i '+, 'o '+ (w) : '1|2'>, but found instead <" + line + ">", FL_AT);

            std::vector <std::string> outputsAndRest = fl::Op::split(inputsAndRest.at(1), ":");
            if (outputsAndRest.size() != 2)
                throw fl::Exception("[syntax error] expected rule to match pattern "
                    "<'i '+, 'o '+ (w) : '1|2'>, but found instead <" + line + ">", FL_AT);

            std::vector<std::string> inputs = fl::Op::split(inputsAndRest.at(0), " ");
            std::vector<std::string> outputs = fl::Op::split(outputsAndRest.at(0), " ");
            std::string weightInParenthesis = outputs.at(outputs.size() - 1);
            outputs.erase(outputs.begin() + outputs.size() - 1);
            std::string connector = fl::Op::trim(outputsAndRest.at(1));

            if ((int) inputs.size() != engine->numberOfInputVariables()) {
                std::ostringstream ss;
                ss << "[syntax error] expected <" << engine->numberOfInputVariables() << ">"
                        " input variables, but found <" << inputs.size() << ">"
                        " input variables in rule <" << line << ">";
                throw fl::Exception(ss.str(), FL_AT);
            }
            if ((int) outputs.size() != engine->numberOfOutputVariables()) {
                std::ostringstream ss;
                ss << "[syntax error] expected <" << engine->numberOfOutputVariables() << ">"
                        " output variables, but found <" << outputs.size() << ">"
                        " output variables in rule <" << line << ">";
                throw fl::Exception(ss.str(), FL_AT);
            }

            std::vector<std::string> antecedent, consequent;

            for (std::size_t i = 0; i < inputs.size(); ++i) {
                scalar inputCode = fl::Op::toScalar(inputs.at(i));
                if (fl::Op::isEq(inputCode, 0.0)) continue;
                std::ostringstream ss;
                ss << engine->getInputVariable(i)->getName() << " "
                        << fl::Rule::FL_IS << " "
                        << translateProposition(inputCode, engine->getInputVariable(i));
                antecedent.push_back(ss.str());
            }

            for (std::size_t i = 0; i < outputs.size(); ++i) {
                scalar outputCode = fl::Op::toScalar(outputs.at(i));
                if (fl::Op::isEq(outputCode, 0.0)) continue;
                std::ostringstream ss;
                ss << engine->getOutputVariable(i)->getName() << " "
                        << fl::Rule::FL_IS << " "
                        << translateProposition(outputCode, engine->getOutputVariable(i));
                consequent.push_back(ss.str());
            }

            std::ostringstream rule;

            rule << fl::Rule::FL_IF << " ";
            for (std::size_t i = 0; i < antecedent.size(); ++i) {
                rule << antecedent.at(i);
                if (i + 1 < antecedent.size()) {
                    rule << " ";
                    if (connector == "1") rule << fl::Rule::FL_AND << " ";
                    else if (connector == "2") rule << fl::Rule::FL_OR << " ";
                    else throw fl::Exception("[syntax error] connector <"
                            + connector + "> not recognized", FL_AT);
                }
            }

            rule << " " << fl::Rule::FL_THEN << " ";
            for (std::size_t i = 0; i < consequent.size(); ++i) {
                rule << consequent.at(i);
                if (i + 1 < consequent.size()) {
                    rule << " " << fl::Rule::FL_AND << " ";
                }
            }

            std::ostringstream ss;
            for (std::size_t i = 0; i < weightInParenthesis.size(); ++i) {
                if (weightInParenthesis.at(i) == '('
                        or weightInParenthesis.at(i) == ')'
                        or weightInParenthesis.at(i) == ' ') continue;
                ss << weightInParenthesis.at(i);
            }

            scalar weight = fl::Op::toScalar(ss.str());
            if (not fl::Op::isEq(weight, 1.0))
                rule << " " << fl::Rule::FL_WITH << " " << Op::str(weight);

            ruleblock->addRule(fl::Rule::parse(rule.str(), engine));
        }
    }

    std::string FisImporter::translateProposition(scalar code, Variable* variable) const {
        int intPart = (int) std::floor(std::fabs(code)) - 1;
        scalar fracPart = std::fmod(std::fabs(code), 1.0);
        if (intPart >= variable->numberOfTerms()) {
            std::ostringstream ex;
            ex << "[syntax error] the code <" << code << "> refers to a term "
                    "out of range from variable <" << variable->toString() << ">";
            throw fl::Exception(ex.str(), FL_AT);
        }

        bool isAny = intPart < 0;
        std::ostringstream ss;
        if (code < 0) ss << Not().name() << " ";
        if (fl::Op::isEq(fracPart, 0.01)) ss << Seldom().name() << " ";
        else if (fl::Op::isEq(fracPart, 0.05)) ss << Somewhat().name() << " ";
        else if (fl::Op::isEq(fracPart, 0.2)) ss << Very().name() << " ";
        else if (fl::Op::isEq(fracPart, 0.3)) ss << Extremely().name() << " ";
        else if (fl::Op::isEq(fracPart, 0.4)) ss << Very().name() << " " << Very().name() << " ";
        else if (fl::Op::isEq(fracPart, 0.99)) ss << Any().name() << " ";
        else if (not fl::Op::isEq(fracPart, 0))
            throw fl::Exception("[syntax error] no hedge defined in FIS format for <"
                + fl::Op::str(fracPart) + ">", FL_AT);
        if (not isAny) {
            ss << variable->getTerm(intPart)->getName();
        }
        return ss.str();
    }

    std::string FisImporter::tnorm(const std::string & name) const {
        if (name == "min") return Minimum().className();
        else if (name == "prod") return AlgebraicProduct().className();
        else if (name == "bounded_difference") return BoundedDifference().className();
        else if (name == "drastic_product") return DrasticProduct().className();
        else if (name == "einstein_product") return EinsteinProduct().className();
        else if (name == "hamacher_product") return HamacherProduct().className();
        return name;
    }

    std::string FisImporter::snorm(const std::string & name) const {
        if (name == "max") return Maximum().className();
        else if (name == "sum" or name == "probor") return AlgebraicSum().className();
        else if (name == "bounded_sum") return BoundedSum().className();
        else if (name == "normalized_sum") return NormalizedSum().className();
        else if (name == "drastic_sum") return DrasticSum().className();
        else if (name == "einstein_sum") return EinsteinSum().className();
        else if (name == "hamacher_sum") return HamacherSum().className();
        return name;
    }

    std::string FisImporter::defuzzifier(const std::string & name) const {
        if (name == "centroid") return Centroid().className();
        if (name == "bisector") return Bisector().className();
        if (name == "lom") return LargestOfMaximum().className();
        if (name == "mom") return MeanOfMaximum().className();
        if (name == "som") return SmallestOfMaximum().className();
        if (name == "wtaver") return WeightedAverage().className();
        if (name == "wtsum") return WeightedSum().className();
        return name;
    }

    std::pair<scalar, scalar> FisImporter::extractRange(const std::string& range) const {
        std::vector<std::string> parts = fl::Op::split(range, " ");
        if (parts.size() != 2)
            throw fl::Exception("[syntax error] expected range in format '[begin end]',"
                " but found <" + range + ">", FL_AT);
        std::string begin = parts.at(0), end = parts.at(1);
        if (begin.at(0) != '[' or end.at(end.size() - 1) != ']')
            throw fl::Exception("[syntax error] expected range in format '[begin end]',"
                " but found <" + range + ">", FL_AT);
        std::pair<scalar, scalar> result;
        result.first = fl::Op::toScalar(begin.substr(1));
        result.second = fl::Op::toScalar(end.substr(0, end.size() - 1));
        return result;
    }

    Term * FisImporter::extractTerm(const std::string & fis) const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < fis.size(); ++i) {
            if (not (fis.at(i) == '[' or fis.at(i) == ']')) {
                ss << fis.at(i);
            }
        }
        std::string line = ss.str();

        std::vector<std::string> nameTerm = fl::Op::split(line, ":");
        if (nameTerm.size() != 2) {
            throw fl::Exception("[syntax error] expected term in format 'name':'class',[params], "
                    "but found <" + line + ">", FL_AT);
        }
        std::vector<std::string> termParams = fl::Op::split(nameTerm.at(1), ",");
        if (termParams.size() != 2) {
            throw fl::Exception("[syntax error] expected term in format 'name':'class',[params], "
                    "but found " + line, FL_AT);
        }

        std::vector<std::string> parameters = fl::Op::split(termParams.at(1), " ");
        for (std::size_t i = 0; i < parameters.size(); ++i) {
            parameters.at(i) = fl::Op::trim(parameters.at(i));
        }
        return createInstance(
                fl::Op::trim(termParams.at(0)),
                fl::Op::trim(nameTerm.at(0)),
                parameters);
    }

    Term* FisImporter::prepareTerm(Term* term, const Engine* engine) const {
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

    Term * FisImporter::createInstance(const std::string& mClass,
            const std::string& name, const std::vector<std::string>& params) const {
        std::map<std::string, std::string> mapping;
        mapping["discretemf"] = Discrete().className();
        mapping["constant"] = Constant().className();
        mapping["function"] = Function().className();
        mapping["gbellmf"] = Bell().className();
        mapping["gaussmf"] = Gaussian().className();
        mapping["gauss2mf"] = GaussianProduct().className();
        mapping["linear"] = Linear().className();
        mapping["pimf"] = PiShape().className();
        mapping["rampmf"] = Ramp().className();
        mapping["rectmf"] = Rectangle().className();
        mapping["smf"] = SShape().className();
        mapping["sigmf"] = Sigmoid().className();
        mapping["dsigmf"] = SigmoidDifference().className();
        mapping["psigmf"] = SigmoidProduct().className();
        mapping["trapmf"] = Trapezoid().className();
        mapping["trimf"] = Triangle().className();
        mapping["zmf"] = ZShape().className();

        std::vector<scalar> sortedParams;
        if (mClass != "function") {
            for (std::size_t i = 0; i < params.size(); ++i) {
                sortedParams.push_back(fl::Op::toScalar(params.at(i)));
            }
        }

        if (mClass == "gbellmf" and params.size() >= 3) {
            sortedParams.at(0) = fl::Op::toScalar(params.at(2));
            sortedParams.at(1) = fl::Op::toScalar(params.at(0));
            sortedParams.at(2) = fl::Op::toScalar(params.at(1));
        } else if (mClass == "gaussmf" and params.size() >= 2) {
            sortedParams.at(0) = fl::Op::toScalar(params.at(1));
            sortedParams.at(1) = fl::Op::toScalar(params.at(0));
        } else if (mClass == "gauss2mf" and params.size() >= 4) {
            sortedParams.at(0) = fl::Op::toScalar(params.at(1));
            sortedParams.at(1) = fl::Op::toScalar(params.at(0));
            sortedParams.at(2) = fl::Op::toScalar(params.at(3));
            sortedParams.at(3) = fl::Op::toScalar(params.at(2));
        } else if (mClass == "sigmf" and params.size() >= 2) {
            sortedParams.at(0) = fl::Op::toScalar(params.at(1));
            sortedParams.at(1) = fl::Op::toScalar(params.at(0));
        } else if (mClass == "dsigmf" and params.size() >= 4) {
            sortedParams.at(0) = fl::Op::toScalar(params.at(1));
            sortedParams.at(1) = fl::Op::toScalar(params.at(0));
            sortedParams.at(2) = fl::Op::toScalar(params.at(2));
            sortedParams.at(3) = fl::Op::toScalar(params.at(3));
        } else if (mClass == "psigmf" and params.size() >= 4) {
            sortedParams.at(0) = fl::Op::toScalar(params.at(1));
            sortedParams.at(1) = fl::Op::toScalar(params.at(0));
            sortedParams.at(2) = fl::Op::toScalar(params.at(2));
            sortedParams.at(3) = fl::Op::toScalar(params.at(3));
        }

        std::string flClass;
        std::map<std::string, std::string>::const_iterator it = mapping.find(mClass);
        if (it != mapping.end()) flClass = it->second;
        else flClass = mClass;

        try {
            Term* result = FactoryManager::instance()->term()->createInstance(flClass);
            result->setName(Op::makeValidId(name));
            if (dynamic_cast<Function*> (result)) {
                result->configure(Op::join(params, ""));
            } else {
                result->configure(Op::join(sortedParams, " "));
            }
            return result;
        } catch (fl::Exception& ex) {
            ex.append(FL_AT);
            throw ex;
        }
    }


}