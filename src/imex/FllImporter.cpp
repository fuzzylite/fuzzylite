/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/imex/FllImporter.h"

#include "fl/Headers.h"

#include <queue>

namespace fl {

    FllImporter::FllImporter(const std::string& separator) : Importer(),
    _separator(separator) { }

    FllImporter::~FllImporter() { }

    std::string FllImporter::name() const {
        return "FllImporter";
    }

    void FllImporter::setSeparator(const std::string& separator) {
        this->_separator = separator;
    }

    std::string FllImporter::getSeparator() const {
        return this->_separator;
    }

    Engine* FllImporter::fromString(const std::string& code) const {
        FL_unique_ptr<Engine> engine(new Engine);

        const std::string fll = Op::join(Op::split(code, _separator), "\n");
        std::string tag;
        std::vector<std::string> block;
        std::istringstream fllReader(fll);
        std::string line;

        while (std::getline(fllReader, line)) {
            line = Op::trim(Op::split(line, "#", false).front()); //remove comments
            if (line.empty()) continue;
            std::size_t colon = line.find_first_of(':');
            if (colon == std::string::npos) {
                throw Exception("[import error] expected a colon here: " + line, FL_AT);
            }
            std::string key = Op::trim(line.substr(0, colon));
            std::string value = Op::trim(line.substr(colon + 1));
            if ("Engine" == key) {
                engine->setName(value);
                continue;
            } else if (key == "description" and block.empty()){
                engine->setDescription(value);
                continue;
            } else if (key == "InputVariable"
                    or key == "OutputVariable"
                    or key == "RuleBlock") {
                process(tag, Op::join(block, "\n"), engine.get());
                block.clear(); //clear error flags
                tag = key;
            } else if (tag.empty()) {
                throw Exception("[import error] unexpected block: " + line, FL_AT);
            }
            block.push_back(key + ":" + value);
        }
        process(tag, Op::join(block, "\n"), engine.get());
        return engine.release();
    }

    void FllImporter::process(const std::string& tag, const std::string& block, Engine* engine) const {
        if (tag.empty()) return;
        //        FL_LOG("Processing " << tag << "\n" << block);
        if ("InputVariable" == tag) {
            processInputVariable(block, engine);
        } else if ("OutputVariable" == tag) {
            processOutputVariable(block, engine);
        } else if ("RuleBlock" == tag) {
            processRuleBlock(block, engine);
        } else {
            throw Exception("[import error] block tag <" + tag + "> not recognized", FL_AT);
        }
    }

    void FllImporter::processInputVariable(const std::string& block, Engine* engine) const {
        std::istringstream reader(block);
        std::string line;
        FL_unique_ptr<InputVariable> inputVariable(new InputVariable);
        while (std::getline(reader, line)) {
            std::pair<std::string, std::string> keyValue = parseKeyValue(line, ':');
            if ("InputVariable" == keyValue.first) {
                inputVariable->setName(Op::validName(keyValue.second));
            } else if ("description" == keyValue.first) {
                inputVariable->setDescription(keyValue.second);
            } else if ("enabled" == keyValue.first) {
                inputVariable->setEnabled(parseBoolean(keyValue.second));
            } else if ("range" == keyValue.first) {
                std::pair<scalar, scalar> range = parseRange(keyValue.second);
                inputVariable->setRange(range.first, range.second);
            } else if ("lock-range" == keyValue.first) {
                inputVariable->setLockValueInRange(parseBoolean(keyValue.second));
            } else if ("term" == keyValue.first) {
                inputVariable->addTerm(parseTerm(keyValue.second, engine));
            } else {
                throw Exception("[import error] key <" + keyValue.first + "> not "
                        "recognized in pair <" + keyValue.first + ":" + keyValue.second + ">", FL_AT);
            }
        }
        engine->addInputVariable(inputVariable.release());
    }

    void FllImporter::processOutputVariable(const std::string& block, Engine* engine) const {
        std::istringstream reader(block);
        std::string line;
        FL_unique_ptr<OutputVariable> outputVariable(new OutputVariable);
        while (std::getline(reader, line)) {
            std::pair<std::string, std::string> keyValue = parseKeyValue(line, ':');
            if ("OutputVariable" == keyValue.first) {
                outputVariable->setName(Op::validName(keyValue.second));
            } else if ("description" == keyValue.first) {
                outputVariable->setDescription(keyValue.second);
            } else if ("enabled" == keyValue.first) {
                outputVariable->setEnabled(parseBoolean(keyValue.second));
            } else if ("range" == keyValue.first) {
                std::pair<scalar, scalar> range = parseRange(keyValue.second);
                outputVariable->setRange(range.first, range.second);
            } else if ("default" == keyValue.first) {
                outputVariable->setDefaultValue(Op::toScalar(keyValue.second));
            } else if ("lock-previous" == keyValue.first or "lock-valid" == keyValue.first) {
                outputVariable->setLockPreviousValue(parseBoolean(keyValue.second));
            } else if ("lock-range" == keyValue.first) {
                outputVariable->setLockValueInRange(parseBoolean(keyValue.second));
            } else if ("defuzzifier" == keyValue.first) {
                outputVariable->setDefuzzifier(parseDefuzzifier(keyValue.second));
            } else if ("aggregation" == keyValue.first) {
                outputVariable->fuzzyOutput()->setAggregation(parseSNorm(keyValue.second));
            } else if ("accumulation" == keyValue.first) {
                outputVariable->fuzzyOutput()->setAggregation(parseSNorm(keyValue.second));
                FL_LOG("[warning] obsolete usage of identifier <accumulation: SNorm> in OutputVariable");
                FL_LOG("[information] from version 6.0, the identifier <aggregation: SNorm> should be used");
                FL_LOG("[backward compatibility] assumed "
                        "<aggregation: " << keyValue.second << "> "
                        "instead of <accumulation: " << keyValue.second << ">");
            } else if ("term" == keyValue.first) {
                outputVariable->addTerm(parseTerm(keyValue.second, engine));
            } else {
                throw Exception("[import error] key <" + keyValue.first + "> not "
                        "recognized in pair <" + keyValue.first + ":" + keyValue.second + ">", FL_AT);
            }
        }
        engine->addOutputVariable(outputVariable.release());
    }

    void FllImporter::processRuleBlock(const std::string& block, Engine* engine) const {
        std::istringstream reader(block);
        std::string line;
        FL_unique_ptr<RuleBlock> ruleBlock(new RuleBlock);
        while (std::getline(reader, line)) {
            std::pair<std::string, std::string> keyValue = parseKeyValue(line, ':');
            if ("RuleBlock" == keyValue.first) {
                ruleBlock->setName(keyValue.second);
            } else if ("description" == keyValue.first) {
                ruleBlock->setDescription(keyValue.second);
            } else if ("enabled" == keyValue.first) {
                ruleBlock->setEnabled(parseBoolean(keyValue.second));
            } else if ("conjunction" == keyValue.first) {
                ruleBlock->setConjunction(parseTNorm(keyValue.second));
            } else if ("disjunction" == keyValue.first) {
                ruleBlock->setDisjunction(parseSNorm(keyValue.second));
            } else if ("implication" == keyValue.first) {
                ruleBlock->setImplication(parseTNorm(keyValue.second));
            } else if ("activation" == keyValue.first) {
                TNormFactory* tnorm = FactoryManager::instance()->tnorm();
                //@todo remove backwards compatibility in version 7.0
                if (tnorm->hasConstructor(keyValue.second)) {
                    ruleBlock->setImplication(parseTNorm(keyValue.second));
                    FL_LOG("[warning] obsolete usage of identifier <activation: TNorm> "
                            "in RuleBlock");
                    FL_LOG("[information] from version 6.0, the identifiers are "
                            "<activation: Activation> for Activation methods "
                            "and <implication: TNorm> for T-Norms");
                    FL_LOG("[backward compatibility] assumed "
                            "<implication: " << keyValue.second << "> "
                            "instead of <activation: " << keyValue.second << ">");
                } else {
                    ruleBlock->setActivation(parseActivation(keyValue.second));
                }
            } else if ("rule" == keyValue.first) {
                Rule* rule = new Rule;
                rule->setText(keyValue.second);
                try {
                    rule->load(engine);
                } catch (std::exception& ex) {
                    FL_LOG(ex.what());
                }
                ruleBlock->addRule(rule);
            } else {
                throw Exception("[import error] key <" + keyValue.first + "> not "
                        "recognized in pair <" + keyValue.first + ":" + keyValue.second + ">", FL_AT);
            }
        }
        if (not ruleBlock->getActivation()){
            ruleBlock->setActivation(new General);
        }
        engine->addRuleBlock(ruleBlock.release());
    }

    Term* FllImporter::parseTerm(const std::string& text, Engine* engine) const {
        std::vector<std::string> tokens = Op::split(text, " ");

        //MEDIUM Triangle 0.500 1.000 1.500

        if (tokens.size() < 2) {
            throw Exception("[syntax error] expected a term in format <name class parameters>, "
                    "but found <" + text + ">", FL_AT);
        }
        FL_unique_ptr<Term> term;
        term.reset(FactoryManager::instance()->term()->constructObject(tokens.at(1)));
        term->updateReference(engine);
        term->setName(Op::validName(tokens.at(0)));
        std::ostringstream parameters;
        for (std::size_t i = 2; i < tokens.size(); ++i) {
            parameters << tokens.at(i);
            if (i + 1 < tokens.size()) parameters << " ";
        }
        term->configure(parameters.str());
        return term.release();
    }

    TNorm* FllImporter::parseTNorm(const std::string& name) const {
        if (name == "none") return FactoryManager::instance()->tnorm()->constructObject("");
        return FactoryManager::instance()->tnorm()->constructObject(name);
    }

    SNorm* FllImporter::parseSNorm(const std::string& name) const {
        if (name == "none") return FactoryManager::instance()->snorm()->constructObject("");
        return FactoryManager::instance()->snorm()->constructObject(name);
    }

    Activation* FllImporter::parseActivation(const std::string& name) const {
        if (name == "none") return FactoryManager::instance()->activation()->constructObject("");
        std::vector<std::string> tokens = Op::split(name, " ");
        Activation* result = FactoryManager::instance()->activation()->constructObject(tokens.front());

        std::ostringstream parameters;
        for (std::size_t i = 1; i < tokens.size(); ++i) {
            parameters << tokens.at(i);
            if (i + 1 < tokens.size()) parameters << " ";
        }
        result->configure(parameters.str());
        return result;
    }

    Defuzzifier* FllImporter::parseDefuzzifier(const std::string& text) const {
        std::vector<std::string> parameters = Op::split(text, " ");
        std::string name = parameters.at(0);
        if (name == "none") return FactoryManager::instance()->defuzzifier()->constructObject("");
        Defuzzifier* defuzzifier = FactoryManager::instance()->defuzzifier()->constructObject(name);
        if (parameters.size() > 1) {
            std::string parameter(parameters.at(1));
            if (IntegralDefuzzifier * integralDefuzzifier = dynamic_cast<IntegralDefuzzifier*> (defuzzifier)) {
                integralDefuzzifier->setResolution((int) Op::toScalar(parameter));
            } else if (WeightedDefuzzifier * weightedDefuzzifier = dynamic_cast<WeightedDefuzzifier*> (defuzzifier)) {
                WeightedDefuzzifier::Type type = WeightedDefuzzifier::Automatic;
                if (parameter == "Automatic") type = WeightedDefuzzifier::Automatic;
                else if (parameter == "TakagiSugeno") type = WeightedDefuzzifier::TakagiSugeno;
                else if (parameter == "Tsukamoto") type = WeightedDefuzzifier::Tsukamoto;
                else throw Exception("[syntax error] unknown parameter of WeightedDefuzzifier <" + parameter + ">", FL_AT);
                weightedDefuzzifier->setType(type);
            }
        }
        return defuzzifier;
    }

    std::pair<scalar, scalar> FllImporter::parseRange(const std::string& text) const {
        std::pair<std::string, std::string> range = parseKeyValue(text, ' ');
        return std::pair<scalar, scalar>(Op::toScalar(range.first), Op::toScalar(range.second));
    }

    bool FllImporter::parseBoolean(const std::string& boolean) const {
        if ("true" == boolean) return true;
        if ("false" == boolean) return false;
        throw Exception("[syntax error] expected boolean <true|false>, "
                "but found <" + boolean + ">", FL_AT);
    }

    std::pair<std::string, std::string> FllImporter::parseKeyValue(const std::string& text,
            char separator) const {
        std::size_t half = text.find_first_of(separator);
        if (half == std::string::npos) {
            std::ostringstream ex;
            ex << "[syntax error] expected pair in the form "
                    "<key" << separator << "value>, but found <" << text << ">";
            throw Exception(ex.str(), FL_AT);
        }
        std::pair<std::string, std::string> result;
        result.first = text.substr(0, half);
        result.second = text.substr(half + 1);
        return result;
    }

    FllImporter* FllImporter::clone() const {
        return new FllImporter(*this);
    }

}
