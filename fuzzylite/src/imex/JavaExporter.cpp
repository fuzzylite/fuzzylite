/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/imex/JavaExporter.h"

#include "fl/Headers.h"

#include <algorithm>
namespace fl {

    JavaExporter::JavaExporter(bool exportVariableName) : Exporter(),
    _exportVariableName(exportVariableName) {
    }

    JavaExporter::~JavaExporter() {

    }

    std::string JavaExporter::name() const {
        return "JavaExporter";
    }

    void JavaExporter::setExportVariableName(bool exportVariableName) {
        this->_exportVariableName = exportVariableName;
    }

    bool JavaExporter::exportVariableName() const {
        return this->_exportVariableName;
    }

    std::string JavaExporter::toString(const Engine* engine) const {
        std::ostringstream ss;
        ss << "Engine engine = new Engine();\n";
        ss << "engine.setName(\"" << engine->getName() << "\");\n";
        ss << "\n";

        for (std::size_t i = 0; i < engine->numberOfInputVariables(); ++i) {
            ss << toString(engine->getInputVariable(i), engine) << "\n";
        }

        for (std::size_t i = 0; i < engine->numberOfOutputVariables(); ++i) {
            ss << toString(engine->getOutputVariable(i), engine) << "\n";
        }

        for (std::size_t i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            ss << toString(engine->getRuleBlock(i), engine) << "\n";
        }

        return ss.str();
    }

    std::string JavaExporter::toString(const InputVariable* inputVariable, const Engine* engine) const {
        std::ostringstream ss;
        std::string name;
        if (exportVariableName()) {
            name = fl::Op::validName(inputVariable->getName());
        } else {
            name = "inputVariable";
            if (engine->numberOfInputVariables() > 1) {
                std::size_t index = std::distance(engine->inputVariables().begin(),
                        std::find(engine->inputVariables().begin(),
                        engine->inputVariables().end(), inputVariable));
                name += Op::str(index + 1);
            }
        }
        ss << "InputVariable " << name << " = new InputVariable();\n";
        ss << name << ".setEnabled(" << (inputVariable->isEnabled() ? "true" : "false") << ");\n";
        ss << name << ".setName(\"" << inputVariable->getName() << "\");\n";
        ss << name << ".setRange("
                << toString(inputVariable->getMinimum()) << ", "
                << toString(inputVariable->getMaximum()) << ");\n";
        ss << name << ".setLockValueInRange("
                << (inputVariable->isLockValueInRange() ? "true" : "false") << ");\n";
        for (std::size_t i = 0; i < inputVariable->numberOfTerms(); ++i) {
            ss << name << ".addTerm(" <<
                    toString(inputVariable->getTerm(i)) << ");\n";
        }
        ss << "engine.addInputVariable(" << name << ");\n";
        return ss.str();
    }

    std::string JavaExporter::toString(const OutputVariable* outputVariable, const Engine* engine) const {
        std::ostringstream ss;
        std::string name;
        if (exportVariableName()) {
            name = fl::Op::validName(outputVariable->getName());
        } else {
            name = "outputVariable";
            if (engine->numberOfOutputVariables() > 1) {
                std::size_t index = std::distance(engine->outputVariables().begin(),
                        std::find(engine->outputVariables().begin(),
                        engine->outputVariables().end(), outputVariable));
                name += Op::str(index + 1);
            }
        }
        ss << "OutputVariable " << name << " = new OutputVariable();\n";
        ss << name << ".setEnabled(" << (outputVariable->isEnabled() ? "true" : "false") << ");\n";
        ss << name << ".setName(\"" << outputVariable->getName() << "\");\n";
        ss << name << ".setRange("
                << toString(outputVariable->getMinimum()) << ", "
                << toString(outputVariable->getMaximum()) << ");\n";
        ss << name << ".setLockValueInRange(" <<
                (outputVariable->isLockValueInRange() ? "true" : "false") << ");\n";
        ss << name << ".fuzzyOutput().setAccumulation(" <<
                toString(outputVariable->fuzzyOutput()->getAccumulation()) << ");\n";
        ss << name << ".setDefuzzifier(" <<
                toString(outputVariable->getDefuzzifier()) << ");\n";
        ss << name << ".setDefaultValue(" <<
                toString(outputVariable->getDefaultValue()) << ");\n";
        ss << name << ".setLockPreviousValue(" <<
                (outputVariable->isLockPreviousValue() ? "true" : "false") << ");\n";
        for (std::size_t i = 0; i < outputVariable->numberOfTerms(); ++i) {
            ss << name << ".addTerm(" <<
                    toString(outputVariable->getTerm(i)) << ");\n";
        }
        ss << "engine.addOutputVariable(" << name << ");\n";
        return ss.str();
    }

    std::string JavaExporter::toString(const RuleBlock* ruleBlock, const Engine* engine) const {
        std::ostringstream ss;
        std::string name = "ruleBlock";
        if (engine->numberOfRuleBlocks() > 1) {
            std::size_t index = std::distance(engine->ruleBlocks().begin(),
                    std::find(engine->ruleBlocks().begin(),
                    engine->ruleBlocks().end(), ruleBlock));
            name += Op::str(index + 1);
        }
        ss << "RuleBlock " << name << " = new RuleBlock();\n";
        ss << name << ".setEnabled(" << (ruleBlock->isEnabled() ? "true" : "false") << ");\n";
        ss << name << ".setName(\"" << ruleBlock->getName() << "\");\n";
        ss << name << ".setConjunction("
                << toString(ruleBlock->getConjunction()) << ");\n";
        ss << name << ".setDisjunction("
                << toString(ruleBlock->getDisjunction()) << ");\n";
        ss << name << ".setImplication("
                << toString(ruleBlock->getImplication()) << ");\n";
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            ss << name << ".addRule(Rule.parse(\"" << rule->getText() << "\", engine));\n";
        }
        ss << "engine.addRuleBlock(" << name << ");\n";
        return ss.str();
    }

    std::string JavaExporter::toString(const Term* term) const {
        if (not term) {
            return "null";
        }

        if (const Discrete * discrete = dynamic_cast<const Discrete*> (term)) {
            std::ostringstream ss;
            std::vector<scalar> xy;
            ss << term->className() << ".create(\"" << term->getName() << "\", "
                    << Op::join(Discrete::toVector(discrete->xy()), ", ") << ")";
            return ss.str();
        }

        if (const Function * function = dynamic_cast<const Function*> (term)) {
            std::ostringstream ss;
            ss << term->className() << ".create(\"" << term->getName() << "\", "
                    << "\"" << function->getFormula() << "\", engine)";
            return ss.str();
        }

        if (const Linear * linear = dynamic_cast<const Linear*> (term)) {
            std::ostringstream ss;
            ss << term->className() << ".create(\"" << term->getName() << "\", "
                    << "engine, " << Op::join(linear->coefficients(), ", ") << ")";
            return ss.str();
        }

        std::ostringstream ss;
        ss << "new " << term->className() << "(\"" << term->getName() << "\", "
                << Op::findReplace(term->parameters(), " ", ", ") << ")";
        return ss.str();
    }

    std::string JavaExporter::toString(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "null";

        if (const IntegralDefuzzifier * integralDefuzzifier =
                dynamic_cast<const IntegralDefuzzifier*> (defuzzifier)) {
            return "new " + integralDefuzzifier->className() + "("
                    + fl::Op::str(integralDefuzzifier->getResolution()) + ")";
        }
        if (const WeightedDefuzzifier * weightedDefuzzifier =
                dynamic_cast<const WeightedDefuzzifier*> (defuzzifier)) {
            return "new " + weightedDefuzzifier->className() +
                    "(\"" + weightedDefuzzifier->getTypeName() + "\")";
        }
        return "new " + defuzzifier->className() + "()";
    }

    std::string JavaExporter::toString(const Norm* norm) const {
        if (not norm) return "null";
        return "new " + norm->className() + "()";
    }

    std::string JavaExporter::toString(scalar value) const {
        if (Op::isNaN(value)) {
            return "Double.NaN";
        } else if (Op::isInf(value)) {
            return (value > 0
                    ? "Double.POSITIVE_INFINITY"
                    : "Double.NEGATIVE_INFINITY");
        }
        return Op::str(value);
    }

    JavaExporter* JavaExporter::clone() const {
        return new JavaExporter(*this);
    }


}

