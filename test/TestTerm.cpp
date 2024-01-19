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
#include <catch2/catch.hpp>

#include "fl/Headers.h"

namespace fuzzylite {
    class TermAssert {
      private:
        Term* actual;

      public:
        TermAssert(Term* actual) {
            this->actual = actual;
        }

        TermAssert* fllExportsTo(const std::string& obtained) {
            CHECK(this->actual->toString() == obtained);
            return this;
        }

        TermAssert* isMonotonic(bool monotonic) {
            CHECK(this->actual->isMonotonic() == monotonic);
            return this;
        }

        TermAssert* configuredAs(const std::string& parameters) {
            this->actual->configure(parameters);
            return this;
        }

        TermAssert* hasMemberships(const std::map<fl::scalar, fl::scalar>& values) {
            for (const auto& value : values) {
                const auto& x = value.first;
                const auto& expected_y = value.second;

                const auto& obtained_y = this->actual->membership(x);

                CHECK(expected_y == obtained_y);
            }
            return this;
        }
    };

    TEST_CASE("Bell", "[term][bell]") {
        TermAssert* termAssert = new TermAssert(new Bell("bell"));
        CHECK(termAssert->fllExportsTo("Bell bell nan nan nan"));
    }

}  // namespace fl
