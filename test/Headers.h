
#ifndef FL_TEST_HEADERS_H
#define FL_TEST_HEADERS_H

#include <catch2/catch.hpp>

#include "fl/Headers.h"

namespace fuzzylite {
    class Approximates : public Catch::MatcherBase<double> {
      private:
        double expected;
        double margin;

      public:
        Approximates(double expected, double margin = fl::fuzzylite::absoluteTolerance()) :
            expected(expected),
            margin(margin) {}

        bool match(const double& obtained) const override {
            return fl::Op::isClose(expected, obtained, margin);
        }

        std::string describe() const override {
            std::ostringstream ss;
            ss << "≈ " << expected << " @(" << margin << ")";
            return ss.str();
        }
    };
}
#endif  // FL_TEST_HEADERS_H
