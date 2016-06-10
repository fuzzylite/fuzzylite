/*
 Copyright © 2010-2016 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "test/catch.hpp"
#include "fl/Headers.h"

namespace fl {

    TEST_CASE("Increment ", "[op][increment]") {
        std::vector<int> sampleValues, minSampleValues, maxSampleValues;
        for (std::size_t i = 0; i < 2; ++i) {
            sampleValues.push_back(0);
            minSampleValues.push_back(0);
            if (i == 0)
                maxSampleValues.push_back(10);
            else maxSampleValues.push_back(0);
        }
        int times = 0;
        do {
            times++;
            FL_LOG(times << " " << Op::join(sampleValues, ","));
        } while (Op::increment(sampleValues, minSampleValues, maxSampleValues));

        CHECK(times == 10 + 1);
    }

    TEST_CASE("Op::split splits multiline", "[op][split]") {
        std::string multiline = "1\n2\n3\n4";
        std::vector<std::string> split = Op::split(multiline, "\n");
        CHECK(split.size() == 4);
    }

    TEST_CASE("Op::str produces correct numbers", "[op][str]") {
        fuzzylite::setDecimals(3);
        CHECK(Op::str(1.0) == "1.000");
        CHECK(Op::str((long) 5000) == "5000");
        CHECK(Op::str((long long) 5000) == "5000");
        CHECK(Op::str((int) 6000) == "6000");
        CHECK(Op::str(std::size_t(6000)) == "6000");
        CHECK(Op::str(scalar(0.333333)) == "0.333");
        CHECK(Op::str(float(0.333333)) == "0.333");
        CHECK(Op::str(double(0.333333)) == "0.333");
    }


}
