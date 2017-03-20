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
        fuzzylite::setLogging(true);
        fuzzylite::setDecimals(3);

        FL_LOG(Op::str(1.0));
        FL_LOG(Op::str((long) 5000));
        FL_LOG(Op::str((int) 6000));
        FL_LOG(Op::str(std::size_t(6000)));
        FL_LOG(Op::str(scalar(0.333333)));
        FL_LOG(Op::str(float(0.333333)));
        FL_LOG(Op::str(double(0.333333)));
        FL_LOG(Op::str(double(0.333333), 9));

        CHECK(Op::str(0) == "0");
        CHECK(Op::str(1) == "1");
        CHECK(Op::str(1.0) == "1.000");
        CHECK(Op::str((long) 5000) == "5000");
        CHECK(Op::str((int) 6000) == "6000");
        CHECK(Op::str(std::size_t(6000)) == "6000");
        CHECK(Op::str(scalar(0.333333)) == "0.333");
        CHECK(Op::str(float(0.333333)) == "0.333");
        CHECK(Op::str(double(0.333333)) == "0.333");
        CHECK(Op::str(double(0.0000333), 9) == "0.000033300");

        CHECK(Op::str(fuzzylite::macheps()) == "0.000");
        CHECK(Op::str(fuzzylite::macheps(), 6) == "0.000001");
        CHECK(Op::str(1e-7) == "0.000");
        CHECK(Op::str(1e-7, 6) == "0.000000");
        CHECK(Op::str(1e-7, 7) == "0.0000001");

        FL_LOG("scientific");
        fuzzylite::setScalarFormat(std::ios_base::scientific);
        FL_LOG(Op::str(1.0));
        FL_LOG(Op::str((long) 5000));
        FL_LOG(Op::str((int) 6000));
        FL_LOG(Op::str(std::size_t(6000)));
        FL_LOG(Op::str(scalar(0.333333)));
        FL_LOG(Op::str(float(0.333333)));
        FL_LOG(Op::str(double(0.333333)));
        FL_LOG(Op::str(double(0.0000333), 9));

        CHECK(Op::str(0) == "0");
        CHECK(Op::str(1.0) == "1.000e+00");
        CHECK(Op::str((long) 5000) == "5000");
        CHECK(Op::str((int) 6000) == "6000");
        CHECK(Op::str(std::size_t(6000)) == "6000");
        CHECK(Op::str(scalar(0.333333)) == "3.333e-01");
        CHECK(Op::str(float(0.333333)) == "3.333e-01");
        CHECK(Op::str(double(0.333333)) == "3.333e-01");
        CHECK(Op::str(double(0.0000333), 9) == "3.330000000e-05");

        CHECK(Op::isEq(fuzzylite::macheps(), 0.0) == false);
        CHECK(Op::isEq(fuzzylite::macheps(), 0.0, std::pow(10.0, -6)) == false);
        CHECK(Op::str(fuzzylite::macheps()) == "0.000e+00");
        CHECK(Op::str(fuzzylite::macheps(), -1) == "1.000000e-06");
        CHECK(Op::str(fuzzylite::macheps(), -1, std::ios_base::fmtflags(0x0)) == "1e-06");
        CHECK(Op::str(1e-7, 6) == "0.000000e+00");
        CHECK(Op::str(1e-7, 7) == "1.0000000e-07");
        CHECK(Op::str(1e-7, 7, std::ios_base::fmtflags(0x0)) == "1e-07");


        fuzzylite::setScalarFormat(std::ios_base::fixed);

        CHECK(Op::str(0.000001, 6, std::ios_base::fmtflags(0x0)) == "1e-06");
        CHECK(Op::str(1e-6, 6, std::ios_base::fmtflags(0x0)) == "1e-06");
        CHECK(Op::str(1e6, 3, std::ios_base::fmtflags(0x0)) == "1e+06");
        CHECK(Op::str(1000000, 3, std::ios_base::fmtflags(0x0)) == "1000000");
        CHECK(Op::str(1000000.0, 3, std::ios_base::fmtflags(0x0)) == "1e+06");
    }

    TEST_CASE("macro expansion does not evaluate parameter before expansion", "[op]") {
        std::ostringstream os;
#define FL_MACRO1(x)  os << x * 5;
        FL_MACRO1(4 + 10);
        CHECK(os.str() == "54");

#define xstr(s) str(s)
#define str(s) #s
        CHECK(xstr(4 + 10) == "4 + 10");
    }


}
