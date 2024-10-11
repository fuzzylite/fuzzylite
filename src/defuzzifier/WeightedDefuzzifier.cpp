/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/defuzzifier/WeightedDefuzzifier.h"

#include <set>

#include "fuzzylite/term/Activated.h"
#include "fuzzylite/term/Aggregated.h"
#include "fuzzylite/term/Concave.h"
#include "fuzzylite/term/Constant.h"
#include "fuzzylite/term/Function.h"
#include "fuzzylite/term/Linear.h"
#include "fuzzylite/term/Ramp.h"
#include "fuzzylite/term/SShape.h"
#include "fuzzylite/term/Sigmoid.h"
#include "fuzzylite/term/ZShape.h"
#include "fuzzylite/variable/Variable.h"

namespace fuzzylite {

    WeightedDefuzzifier::WeightedDefuzzifier(Type type) : _type(type) {}

    WeightedDefuzzifier::WeightedDefuzzifier(const std::string& type) {
        try {
            setType(type);
        } catch (...) { setType(Automatic); }
    }

    WeightedDefuzzifier::~WeightedDefuzzifier() {}

    std::string WeightedDefuzzifier::typeName(Type type) {
        switch (type) {
            case Automatic:
                return "Automatic";
            case TakagiSugeno:
                return "TakagiSugeno";
            case Tsukamoto:
                return "Tsukamoto";
            default:
                return "";
        }
    }

    void WeightedDefuzzifier::setType(Type type) {
        this->_type = type;
    }

    WeightedDefuzzifier::Type WeightedDefuzzifier::getType() const {
        return this->_type;
    }

    void WeightedDefuzzifier::setType(const std::string& type) {
        if (type == "Automatic")
            setType(Automatic);
        else if (type == "TakagiSugeno")
            setType(TakagiSugeno);
        else if (type == "Tsukamoto")
            setType(Tsukamoto);
        else
            throw Exception("invalid type '" + type + "' for WeightedDefuzzifier");
    }

    std::string WeightedDefuzzifier::getTypeName() const {
        return typeName(getType());
    }

    WeightedDefuzzifier::Type WeightedDefuzzifier::inferType(const Variable* variable) const {
        std::vector<const Term*> const_terms(variable->terms().begin(), variable->terms().end());
        return inferType(const_terms);
    }

    WeightedDefuzzifier::Type WeightedDefuzzifier::inferType(const std::vector<const Term*>& terms) const {
        std::set<WeightedDefuzzifier::Type> types;
        for (std::size_t i = 0; i < terms.size(); ++i)
            types.insert(this->inferType(terms.at(i)));
        switch (types.size()) {
            case 0:
                // cannot infer type of empty term, and won't matter anyway
                return Automatic;
            case 1:
                return *types.begin();
            default:
                throw Exception("cannot infer type of '" + this->className() + "', got multiple types");
        }
    }

    WeightedDefuzzifier::Type WeightedDefuzzifier::inferType(const Term* term) const {
        if (const Aggregated* aggregated = dynamic_cast<const Aggregated*>(term)) {
            std::vector<const Term*> terms;
            terms.reserve(aggregated->numberOfTerms());
            for (std::size_t i = 0; i < aggregated->numberOfTerms(); ++i)
                terms.push_back(aggregated->getTerm(i).getTerm());
            return inferType(terms);
        }
        if (const Activated* activated = dynamic_cast<const Activated*>(term))
            return inferType(activated->getTerm());
        if (dynamic_cast<const Constant*>(term) or dynamic_cast<const Linear*>(term)
            or dynamic_cast<const Function*>(term)) {
            return TakagiSugeno;
        }
        if (term->isMonotonic())
            return Tsukamoto;
        return Automatic;
    }

    scalar WeightedDefuzzifier::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        FL_IUNUSED(minimum);
        FL_IUNUSED(maximum);

        const Aggregated* fuzzyOutput = dynamic_cast<const Aggregated*>(term);
        if (not fuzzyOutput) {
            std::ostringstream ss;
            ss << "[defuzzification error]" << "expected an Aggregated term instead of" << "<"
               << (term ? term->toString() : "null") << ">";
            throw Exception(ss.str(), FL_AT);
        }

        if (fuzzyOutput->isEmpty())
            return fl::nan;

        Type type = getType();
        if (type == Automatic)
            type = inferType(fuzzyOutput);

        switch (type) {
            case TakagiSugeno:
                return takagiSugeno(fuzzyOutput);
            case Tsukamoto:
                return tsukamoto(fuzzyOutput);
            default:
                throw Exception("unexpected type: " + typeName(type));
        }
    }

}
