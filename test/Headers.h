
#ifndef FL_TEST_HEADERS_H
#define FL_TEST_HEADERS_H

#include <catch2/catch.hpp>

#include "Headers.h"
#include "fl/Headers.h"

class Approximates : public Catch::MatcherBase<double> {
  private:
    double expected;

  public:
    explicit Approximates(double expected) : expected(expected) {}

    bool match(const double& obtained) const override {
        return fl::Op::isClose(expected, obtained, fl::fuzzylite::macheps());
    }

    std::string describe() const override {
        std::ostringstream ss;
        ss << "≈ " << expected;
        return ss.str();
    }
};

#endif  // FL_TEST_HEADERS_H
