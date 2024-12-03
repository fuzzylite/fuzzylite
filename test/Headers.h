
#ifndef FL_TEST_HEADERS_H
#define FL_TEST_HEADERS_H

#include <catch2/catch_all.hpp>

#include "Assert.h"
#include "fuzzylite/Headers.h"

namespace fuzzylite { namespace test {
    class Approximates : public Catch::Matchers::MatcherBase<scalar> {
      private:
        scalar expected;
        scalar margin;

      public:
        Approximates(scalar expected, double margin = fl::fuzzylite::absoluteTolerance()) :
            expected(expected),
            margin(margin) {}

        bool match(const scalar& obtained) const override {
            return fl::Op::isClose(expected, obtained, margin);
        }

        std::string describe() const override {
            std::ostringstream ss;
            ss << "≈ " << expected << " @(" << margin << ")";
            return ss.str();
        }
    };
}}
#endif  // FL_TEST_HEADERS_H
