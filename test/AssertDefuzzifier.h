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

#ifndef FL_TEST_ASSERT_DEFUZZIFIER_H
#define FL_TEST_ASSERT_DEFUZZIFIER_H

#include "test/Headers.h"

namespace fuzzylite { namespace test {

    template <class T>
    struct AssertDefuzzifier {
        FL_unique_ptr<T> actual;

        AssertDefuzzifier() : actual(new T) {}

        AssertDefuzzifier& configured_as(const std::string& parameters) {
            FL_IUNUSED(parameters);
            if (auto integralDefuzzifier = dynamic_cast<IntegralDefuzzifier*>(actual.get()))
                integralDefuzzifier->setResolution(std::stoi(parameters));
            else if (auto weightedDefuzzifier = dynamic_cast<WeightedDefuzzifier*>(actual.get()))
                weightedDefuzzifier->setType(parameters);
            return *this;
        }

        AssertDefuzzifier& has_parameters(const std::string& parameters) {
            FL_IUNUSED(parameters);
            return *this;
        }

        AssertDefuzzifier& exports_fll(const std::string& fll) {
            CHECK(FllExporter().toString(actual.get()) == fll);
            return *this;
        }

        AssertDefuzzifier& can_clone() {
            std::unique_ptr<T> clone(actual->clone());
            FllExporter exporter;
            CHECK(exporter.toString(clone.get()) == exporter.toString(actual.get()));
            return *this;
        }

        AssertDefuzzifier& defuzzifies(
            scalar minimum,
            scalar maximum,
            const std::map<fl::Term*, scalar>& terms,
            scalar precision = fuzzylite::absoluteTolerance()
        ) {
            for (const auto& pair : terms) {
                const auto obtained = actual->defuzzify(pair.first, minimum, maximum);
                const auto expected = pair.second;
                CAPTURE(pair.first->toString());
                CHECK_THAT(obtained, Approximates(expected, precision));
                delete pair.first;
            }
            return *this;
        }
    };

    class AssertWeightedDefuzzifier {
      public:
        class MockDefuzzifier : public fl::WeightedDefuzzifier {
          public:
            std::string className() const {
                return "BasicWeightedDefuzzifier";
            }

            scalar takagiSugeno(const Aggregated* term) const {
                FL_IUNUSED(term);
                return fl::nan;
            }

            scalar tsukamoto(const Aggregated* term) const {
                FL_IUNUSED(term);
                return fl::nan;
            }

            Defuzzifier* clone() const {
                return fl::null;
            }

            scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const {
                FL_IUNUSED(term);
                FL_IUNUSED(minimum + maximum);
                return fl::nan;
            }
        };

        AssertWeightedDefuzzifier&
        inferredType(const std::vector<fl::Term*>& terms, WeightedDefuzzifier::Type expected) {
            MockDefuzzifier defuzzifier;
            for (const fl::Term* term : terms) {
                CAPTURE(term->toString());
                CHECK(defuzzifier.inferType(term) == expected);
            }

            std::vector<const fl::Term*> constTerms(terms.begin(), terms.end());
            CHECK(defuzzifier.inferType(constTerms) == expected);

            Variable variable;
            variable.setTerms(terms);
            CAPTURE(variable.toString());
            CHECK(defuzzifier.inferType(&variable) == expected);
            return *this;
        }
    };
}}
#endif
