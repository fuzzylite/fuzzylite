// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

/* 
 * File:   CppExporter.cpp
 * Author: jcrada
 * 
 * Created on 1 January 2013, 2:41 PM
 */

#include "fl/imex/CppExporter.h"

#include "fl/Headers.h"

#include <algorithm>

namespace fl {

    CppExporter::CppExporter() {
    }

    CppExporter::~CppExporter() {
    }

    std::string CppExporter::name() const {
        return "CppExporter";
    }

    std::string CppExporter::toString(const Engine* engine) const {
        std::ostringstream cpp;
        cpp << "fl::Engine* engine = new fl::Engine;\n";
        cpp << "engine->setName(\"" << engine->getName() << "\");\n";

        cpp << "\n";

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            cpp << toString(engine->getInputVariable(i), engine) << "\n";
        }

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            cpp << toString(engine->getOutputVariable(i), engine) << "\n";
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            cpp << toString(engine->getRuleBlock(i), engine) << "\n";
        }

        return cpp.str();
    }

    std::string CppExporter::toString(const InputVariable* inputVariable, const Engine* engine) const {
        std::ostringstream ss;
        std::string name = "inputVariable";
        if (engine->numberOfInputVariables() > 1) {
            int index = std::distance(engine->constInputVariables().begin(),
                    std::find(engine->constInputVariables().begin(),
                    engine->constInputVariables().end(), inputVariable));
            name += Op::str<int>(index + 1);
        }
        ss << "fl::InputVariable* " << name << " = new fl::InputVariable;\n";
        ss << name << "->setEnabled(" << (inputVariable->isEnabled() ? "true" : "false") << ");\n";
        ss << name << "->setName(\"" << inputVariable->getName() << "\");\n";
        ss << name << "->setRange(" <<
                toString(inputVariable->getMinimum()) << ", " <<
                toString(inputVariable->getMaximum()) << ");\n";
        for (int t = 0; t < inputVariable->numberOfTerms(); ++t) {
            ss << name << "->addTerm(" << toString(inputVariable->getTerm(t)) << ");\n";
        }
        ss << "engine->addInputVariable(" << name << ");\n";
        return ss.str();
    }

    std::string CppExporter::toString(const OutputVariable* outputVariable, const Engine* engine) const {
        std::ostringstream ss;
        std::string name = "outputVariable";
        if (engine->numberOfOutputVariables() > 1) {
            int index = std::distance(engine->constOutputVariables().begin(),
                    std::find(engine->constOutputVariables().begin(),
                    engine->constOutputVariables().end(), outputVariable));
            name += Op::str<int>(index + 1);
        }
        ss << "fl::OutputVariable* " << name << " = new fl::OutputVariable;\n";
        ss << name << "->setEnabled(" << (outputVariable->isEnabled() ? "true" : "false") << ");\n";
        ss << name << "->setName(\"" << outputVariable->getName() << "\");\n";
        ss << name << "->setRange(" <<
                toString(outputVariable->getMinimum()) << ", " <<
                toString(outputVariable->getMaximum()) << ");\n";
        ss << name << "->fuzzyOutput()->setAccumulation(" <<
                toString(outputVariable->fuzzyOutput()->getAccumulation()) << ");\n";
        ss << name << "->setDefuzzifier(" <<
                toString(outputVariable->getDefuzzifier()) << ");\n";
        ss << name << "->setDefaultValue(" <<
                toString(outputVariable->getDefaultValue()) << ");\n";
        ss << name << "->setLockValidOutput(" <<
                (outputVariable->isLockingValidOutput() ? "true" : "false") << ");\n";
        ss << name << "->setLockOutputRange(" <<
                (outputVariable->isLockingOutputRange() ? "true" : "false") << ");\n";
        for (int t = 0; t < outputVariable->numberOfTerms(); ++t) {
            ss << name << "->addTerm(" << toString(outputVariable->getTerm(t)) << ");\n";
        }
        ss << "engine->addOutputVariable(" << name << ");\n";
        return ss.str();
    }

    std::string CppExporter::toString(const RuleBlock* ruleBlock, const Engine* engine) const {
        std::ostringstream ss;
        std::string name = "ruleBlock";
        if (engine->numberOfRuleBlocks() > 1) {
            int index = std::distance(engine->constRuleBlocks().begin(),
                    std::find(engine->constRuleBlocks().begin(),
                    engine->constRuleBlocks().end(), ruleBlock));
            name += Op::str<int>(index + 1);
        }
        ss << "fl::RuleBlock* " << name << " = new fl::RuleBlock;\n";
        ss << name << "->setEnabled(" << (ruleBlock->isEnabled() ? "true" : "false") << ");\n";
        ss << name << "->setName(\"" << ruleBlock->getName() << "\");\n";
        ss << name << "->setConjunction(" <<
                toString(ruleBlock->getConjunction()) << ");\n";
        ss << name << "->setDisjunction("
                << toString(ruleBlock->getDisjunction()) << ");\n";
        ss << name << "->setActivation("
                << toString(ruleBlock->getActivation()) << ");\n";
        for (int r = 0; r < ruleBlock->numberOfRules(); ++r) {
            ss << name << "->addRule(fl::Rule::parse(\"" <<
                    ruleBlock->getRule(r)->getText() << "\", engine));\n";
        }
        ss << "engine->addRuleBlock(" << name << ");\n";
        return ss.str();
    }

    std::string CppExporter::toString(scalar value) const {
        std::ostringstream ss;
        if (fl::Op::isNaN(value))
            ss << "fl::nan";
        else if (fl::Op::isInf(value))
            ss << (Op::isGE(value, 0.0) ? "" : "-") << "fl::inf";
        else ss << fl::Op::str(value);
        return ss.str();
    }

    std::string CppExporter::toString(const Term* term) const {
        if (not term) return "NULL";

        if (term->className() == Discrete().className()) {
            const Discrete* discrete = dynamic_cast<const Discrete*> (term);
            std::ostringstream ss;
            ss << "fl::" << term->className() << "::create(\"" << term->getName() << "\", ";
            ss << discrete->x.size() + discrete->y.size() << ", ";
            for (std::size_t i = 0; i < discrete->x.size(); ++i) {
                ss << fl::Op::str(discrete->x.at(i)) << ", "
                        << fl::Op::str(discrete->y.at(i));
                if (i + 1 < discrete->x.size()) ss << ", ";
            }
            ss << ")";
            return ss.str();
        }

        if (term->className() == Function().className()) {
            const Function* function = dynamic_cast<const Function*> (term);
            std::ostringstream ss;
            ss << "fl::" << term->className() << "::create(\"" << term->getName() << "\", "
                    << "\"" << function->getFormula() << "\", engine)";
            return ss.str();
        }

        if (term->className() == Linear().className()) {
            const Linear* linear = dynamic_cast<const Linear*> (term);
            std::ostringstream ss;
            ss << "fl::" << term->className() << "::create(\"" << term->getName() << "\", "
                    << "engine, " << fl::Op::join(linear->constCoefficients(), ", ") << ")";
            return ss.str();
        }

        std::ostringstream ss;
        ss << "new fl::" << term->className() << "(\"" << term->getName() << "\", "
                << Op::findReplace(term->parameters(), " ", ", ") << ")";
        return ss.str();
    }

    std::string CppExporter::toString(const Hedge * hedge) const {
        if (hedge->name() == Any().name()) return "new fl::Any";
        if (hedge->name() == Extremely().name()) return "new fl::Extremely";
        if (hedge->name() == Not().name()) return "new fl::Not";
        if (hedge->name() == Seldom().name()) return "new fl::Seldom";
        if (hedge->name() == Somewhat().name()) return "new fl::Somewhat";
        if (hedge->name() == Very().name()) return "new fl::Very";
        return "new " + hedge->name();
    }

    std::string CppExporter::toString(const Norm* op) const {
        if (not op) return "NULL";
        return "new fl::" + op->className();
    }

    std::string CppExporter::toString(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "NULL";
        const IntegralDefuzzifier* integralDefuzzifier =
                dynamic_cast<const IntegralDefuzzifier*> (defuzzifier);
        if (integralDefuzzifier) {
            return "new fl::" + integralDefuzzifier->className() + "("
                    + fl::Op::str(integralDefuzzifier->getResolution()) + ")";
        }
        return "new fl::" + defuzzifier->className();
    }

    CppExporter* CppExporter::clone() const {
        return new CppExporter(*this);
    }

}
