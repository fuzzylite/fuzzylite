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
        Configuration* configuration = new Configuration;

        std::istringstream fisReader(fis);
        std::string line;
        int lineNumber = 0;

        std::vector<std::string> sections;
        while (std::getline(fisReader, line)) {
            ++lineNumber;
            line = Op::Trim(line);
            line = fl::Op::FindReplace(line, "'", "");
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
        try {
            for (std::size_t i = 0; i < sections.size(); ++i) {
                if ("[System]" == sections[i].substr(0, std::string("[System]").size()))
                    loadSystem(sections[i], engine);
                else if ("[Input" == sections[i].substr(0, std::string("[Input").size()))
                    loadInput(sections[i], engine);
                else if ("[Output" == sections[i].substr(0, std::string("[Output").size()))
                    loadOutput(sections[i], engine);
                else if ("[Rules]" == sections[i].substr(0, std::string("[Rules]").size()))
                    loadRules(sections[i], engine);
                else
                    throw fl::Exception("[internal error] no rule to parse section: "
                        << sections[i]);
            }
        } catch (fl::Exception& ex) {
            delete engine;
            delete configuration;
            throw ex;
        }

        return engine;
    }

    void FisImporter::loadSystem(const std::string& section, Engine * engine) const {
        std::istringstream reader(section);
        std::string line;
        std::getline(reader, line); //ignore first line [System]
        while (std::getline(reader, line)) {
            std::vector<std::string> keyValue = fl::Op::Split(line, "=");
            if (keyValue.size() != 2)
                throw fl::Exception("[syntax error] expected a property of type "
                    "'key=value', but found < " + line + ">");
            std::string key = keyValue[0], value = keyValue[1];

            if (key == "Name") engine->setName(value);
            else if (key == "Type") {
                if (value != "mamdani")
                    throw fl::Exception("[implementation error] only mamdani-type "
                        "engines are supported at the moment");

            } else if (key == "AndMethod") {
                engine->getConfiguration()->setTnorm(extractTNorm(value));
            } else if (key == "OrMethod") {
                engine->getConfiguration()->setSnorm(extractSNorm(value));
            } else if (key == "ImpMethod") {
                engine->getConfiguration()->setActivation(extractTNorm(value));
            } else if (key == "AggMethod") {
                engine->getConfiguration()->setAccumulation(extractSNorm(value));
            } else if (key == "DefuzzMethod") {
                engine->getConfiguration()->setDefuzzifier(extractDefuzzifier(value));
            } else {
                FL_LOG("[info] ignoring redundant or non-relevant information from line: " << line);
            }
        }
    }

    void FisImporter::loadInput(const std::string& section, Engine* engine) const {
        std::istringstream reader(section);

        InputVariable* input = new InputVariable;
        engine->addInputVariable(input);

        std::string line;
        while (std::getline(reader, line)) {
            std::vector<std::string> keyValue = fl::Op::Split(line, "=");
            if (keyValue.size() != 2)
                throw fl::Exception("[syntax error] expected a property of type "
                    "'key=value', but found < " + line + ">");
            std::string key = keyValue[0], value = keyValue[1];

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

    void FisImporter::loadOutput(const std::string& section, Engine* engine) const {
        std::istringstream reader(section);

        OutputVariable* output = new OutputVariable;
        engine->addOutputVariable(output);

        std::string line;
        while (std::getline(reader, line)) {
            std::vector<std::string> keyValue = fl::Op::Split(line, "=");
            if (keyValue.size() != 2)
                throw fl::Exception("[syntax error] expected a property of type "
                    "'key=value', but found < " + line + ">");
            std::string key = keyValue[0], value = keyValue[1];

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

    TNorm* FisImporter::extractTNorm(const std::string& name) const {
        if (name == "min") return new Minimum;
        if (name == "algebraic_product") return new AlgebraicProduct;
        if (name == "bounded_difference") return new BoundedDifference;
        if (name == "drastic_product") return new DrasticProduct;
        if (name == "einstein_product") return new EinsteinProduct;
        if (name == "hamacher_product") return new HamacherProduct;

        throw fl::Exception("[syntax error] T-Norm <" + name + "> not recognized");
    }

    SNorm* FisImporter::extractSNorm(const std::string& name) const {
        if (name == "max") return new Maximum;
        if (name == "algebraic_sum") return new AlgebraicSum;
        if (name == "bounded_sum") return new BoundedSum;
        if (name == "normalized_sum") return new NormalizedSum;
        if (name == "drastic_sum") return new DrasticSum;
        if (name == "einstein_sum") return new EinsteinSum;
        if (name == "hamacher_sum") return new HamacherSum;

        throw fl::Exception("[syntax error] S-Norm <" + name + "> not recognized");
    }

    Term* FisImporter::extractTerm(const std::string& fis) const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < fis.size(); ++i){
            if (not (fis[i] == '[' or fis[i] == ']')){
                ss << fis[i];
            }
        }
        std::string line = ss.str();
        
        std::vector<std::string> nameTerm = fl::Op::Split(line, ":");
        if (nameTerm.size() != 2){
            throw fl::Exception("[syntax error] expected term in format 'name':'class',[params], "
                "but found " + line);
        }
        std::vector<std::string> termParams = fl::Op::Split( nameTerm[1], ",");
        if (termParams.size() != 2){
            throw fl::Exception("[syntax error] expected term in format 'name':'class',[params], "
                "but found " + line);
        }
        
        std::vector<std::string> strParams = fl::Op::Split(termParams[1], " ");
        std::vector<scalar> params;
        for (std::size_t i = 0 ; i < strParams.size(); ++i){
            params.push_back(fl::Op::toScalar(strParams[i]));
        }
        
        return createInstance(termParams[0], nameTerm[0], params);
    }
    
    Term* FisImporter::createInstance(const std::string& termClass,
            const std::string& name, const std::vector<scalar>& params) const {
        int requiredParams = 0;

        if (termClass == "discretemf") {
            if (params.size() % 2 == 0) {
                Discrete* term = new Discrete(name);
                for (std::size_t i = 0; i < params.size() - 1; i += 2) {
                    term->x.push_back(params[i]);
                    term->y.push_back(params[i + 1]);
                }
            } else {
                std::ostringstream ex;
                ex << "[syntax error] a discrete term requires an even list of values, "
                        "but found <" << params.size() << ">";
                throw fl::Exception(ex.str());
            }
        }

        if (termClass == "gbellmf") {
            if (params.size() == (requiredParams = 3)) {
                return new Bell(name, params[0], params[1], params[2]);
            }
        }

        if (termClass == "gaussmf") {
            if (params.size() == (requiredParams = 2)) {
                return new Gaussian(name, params[0], params[1]);
            }
        }

        if (termClass == "gauss2mf") {
            if (params.size() == (requiredParams = 4)) {
                return new GaussianProduct(name, params[0], params[1], params[2], params[3]);
            }
        }

        if (termClass == "pimf") {
            if (params.size() == (requiredParams = 4)) {
                return new PiShape(name, params[0], params[1]);
            }
        }

        if (termClass == "rampmf") {
            if (params.size() == (requiredParams = 2)) {
                return new Ramp(name, params[0], params[1]);
            }
        }


        if (termClass == "rectmf") {
            if (params.size() == (requiredParams = 2)) {
                return new Rectangle(name, params[0], params[1]);
            }
        }

        if (termClass == "smf") {
            if (params.size() == (requiredParams = 2)) {
                return new SShape(name, params[0], params[1]);
            }
        }


        if (termClass == "sigmf") {
            if (params.size() == (requiredParams = 2)) {
                return new Sigmoid(name, params[0], params[1]);
            }
        }
        if (termClass == "dsigmf") {
            if (params.size() == (requiredParams = 4)) {
                return new SigmoidDifference(name, params[0], params[1], params[2], params[3]);
            }
        }
        if (termClass == "psigmf") {
            if (params.size() == (requiredParams = 4)) {
                return new SigmoidProduct(name, params[0], params[1], params[2], params[3]);
            }
        }

        if (termClass == "trapmf") {
            if (params.size() == (requiredParams = 4))
                return new Trapezoid(name, params[0], params[1], params[2], params[3]);
        }

        if (termClass == "trimf") {
            if (params.size() == (requiredParams = 3))
                return new Triangle(name, params[0], params[1], params[2]);
        }

        if (termClass == "zmf") {
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
    

}