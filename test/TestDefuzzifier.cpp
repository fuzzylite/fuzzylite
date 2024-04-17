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

#include "Headers.h"
#include "fl/Headers.h"

namespace fuzzylite {
    class NaN : public Constant {};

    struct DefuzzifierAssert {
        FL_unique_ptr<Defuzzifier> actual;

        DefuzzifierAssert(Defuzzifier* actual) : actual(actual) {}

        DefuzzifierAssert& configured_as(const std::string& parameters) {
            FL_IUNUSED(parameters);
            if (auto integralDefuzzifier = dynamic_cast<IntegralDefuzzifier*>(actual.get()))
                integralDefuzzifier->setResolution(std::stoi(parameters));
            return *this;
        }

        DefuzzifierAssert& has_parameters(const std::string& parameters) {
            FL_IUNUSED(parameters);
            return *this;
        }

        DefuzzifierAssert& exports_fll(const std::string& fll) {
            CHECK(FllExporter().toString(actual.get()) == fll);
            return *this;
        }

        DefuzzifierAssert& defuzzifies(
            scalar minimum,
            scalar maximum,
            const std::map<fl::Term*, float>& terms,
            scalar precision = fuzzylite::macheps()
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

    TEST_CASE("Integral Defuzzifier", "[defuzzifier]") {
        SECTION("Bisector") {
            DefuzzifierAssert(new Bisector()).defuzzifies(0, 2, {{new NaN(), fl::nan}});
        }

        SECTION("Centroid") {
            const scalar precision = 1e-3;
            DefuzzifierAssert(new Centroid())
                .exports_fll("Centroid")
                .configured_as("200")
                .has_parameters("200")
                .exports_fll("Centroid 200");

            DefuzzifierAssert(new Centroid()).defuzzifies(-inf, 0.0, {{new Triangle(), nan}});
            DefuzzifierAssert(new Centroid()).defuzzifies(0.0, inf, {{new Triangle(), nan}});
            DefuzzifierAssert(new Centroid()).defuzzifies(nan, 0.0, {{new Triangle(), nan}});
            //
            FL_unique_ptr<Minimum> minimum(new Minimum());
            FL_unique_ptr<Triangle> medium(new Triangle("Medium", 0.25, 0.5, 0.75));
            FL_unique_ptr<Triangle> high(new Triangle("High", 0.5, 0.75, 1.0));
            DefuzzifierAssert(new Centroid())
                .defuzzifies(
                    -1,
                    1,
                    {{new Triangle("", -inf, 0), nan},
                     {new Triangle("", 0, inf), nan},
                     {new Triangle("", nan, 0), nan},
                     {new Triangle("", -1, 0), -0.5},
                     {new Triangle("", -1, 1), 0.0},
                     {new Triangle("", 0, 1), 0.5},
                     {new Aggregated(
                          "",
                          0,
                          1,
                          new Maximum(),
                          {
                              Activated(medium.get(), 0.2, minimum.get()),
                              Activated(high.get(), 0.8, minimum.get()),
                          }
                      ),
                      0.6896552}},
                    precision
                );
            DefuzzifierAssert(new Centroid()).defuzzifies(-1, 1, {{new NaN(), nan}});
        }
    }

    TEST_CASE("WeightedDefuzzifer", "[defuzzifier]") {}

    TEST_CASE("Issues", "[defuzzifier][!shouldfail]") {
        // TODO: Fix bisector for multiple global minima
        // TODO: Increase precision
        SECTION("Bisector") {
            const scalar precision = 1e-3;
            DefuzzifierAssert(new Bisector()).exports_fll("Bisector").configured_as("200").exports_fll("Bisector 200");

            DefuzzifierAssert(new Bisector())
                .defuzzifies(
                    0,
                    1,
                    {{new Triangle("", 0, 1, 1), 0.7065},
                     {new Triangle("", 0, 0, 1), 0.2925},
                     {new Triangle("", 0, 0.5, 1), 0.4995},
                     {new Rectangle("", 0, 1), 0.4995}},
                    precision
                );
            DefuzzifierAssert(new Bisector())
                .defuzzifies(
                    -1,
                    1,
                    {
                        {new Rectangle("", -1, 1), -0.001},
                    },
                    precision
                );

            FL_unique_ptr<AlgebraicProduct> algebraicProduct(new AlgebraicProduct());
            FL_unique_ptr<Triangle> low(new Triangle("low", -1, -1, -0.5));
            FL_unique_ptr<Triangle> high(new Triangle("high", 0.5, 1, 1));
            DefuzzifierAssert(new Bisector())
                .defuzzifies(
                    -1,
                    1,
                    {
                        {new Aggregated(
                             "",
                             -1,
                             1,
                             new UnboundedSum(),
                             {
                                 Activated(low.get(), 1.0, algebraicProduct.get()),
                                 Activated(high.get(), 1.0, algebraicProduct.get()),
                             }
                         ),
                         -0.001},
                    },
                    precision
                );
        }
        SECTION("SmallestOfMaximum") {
            DefuzzifierAssert(new SmallestOfMaximum())
                .exports_fll("SmallestOfMaximum")
                .configured_as("200")
                .exports_fll("SmallestOfMaximum 200");

            //        # Test case:
            //        #            ______
            //        #      _____/      \
//        # ____/             \
//        # |                   \____
            FL_unique_ptr<Discrete> term(new Discrete(
                "test",
                {
                    {0.0, 0.25},
                    {0.1, 0.25},
                    {0.2, 0.5},
                    {0.4, 0.5},
                    {0.5, 1.0},  // # SOM
                    {0.7, 1.0},
                    {0.9, 1.0},
                    {1.0, 0.0},
                }
            ));

            DefuzzifierAssert(new SmallestOfMaximum()).defuzzifies(-fl::inf, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new SmallestOfMaximum()).defuzzifies(-fl::inf, 0.0, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new SmallestOfMaximum()).defuzzifies(0.0, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new SmallestOfMaximum()).defuzzifies(fl::nan, fl::nan, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new SmallestOfMaximum()).defuzzifies(fl::nan, 0, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new SmallestOfMaximum()).defuzzifies(0, fl::nan, {{term->clone(), fl::nan}});

            DefuzzifierAssert(new SmallestOfMaximum())
                .defuzzifies(
                    0,
                    1,
                    {
                        {term->clone(), 0.5},
                        {new Trapezoid("", 0.0, 0.2, 0.4, 0.6), 0.2},
                    }
                );
            DefuzzifierAssert(new SmallestOfMaximum()).defuzzifies(-1, 1, {{new NaN(), fl::nan}});
        }
        SECTION("LargestOfMaximum") {
            DefuzzifierAssert(new LargestOfMaximum())
                .exports_fll("LargestOfMaximum")
                .configured_as("200")
                .has_parameters("200")
                .exports_fll("LargestOfMaximum 200");

            //        # Test case:
            //        #            ______
            //        #      _____/      \
            //        # ____/             \
            //        # |                   \____
            FL_unique_ptr<Discrete> term(new Discrete(
                "test",
                {
                    {0.0, 0.25},
                    {0.1, 0.25},
                    {0.2, 0.5},
                    {0.4, 0.5},
                    {0.5, 1.0},
                    {0.7, 1.0},
                    {0.9, 1.0},  // #LOM
                    {1.0, 0.0},
                }
            ));

            DefuzzifierAssert(new LargestOfMaximum()).defuzzifies(-fl::inf, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new LargestOfMaximum()).defuzzifies(-fl::inf, 0.0, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new LargestOfMaximum()).defuzzifies(0.0, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new LargestOfMaximum()).defuzzifies(fl::nan, fl::nan, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new LargestOfMaximum()).defuzzifies(fl::nan, 0, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new LargestOfMaximum()).defuzzifies(0, fl::nan, {{term->clone(), fl::nan}});

            DefuzzifierAssert(new LargestOfMaximum())
                .defuzzifies(
                    0,
                    1,
                    {
                        {term->clone(), 0.9},
                        {new Trapezoid("", 0.0, 0.2, 0.4, 0.6), 0.4},
                    }
                );
            DefuzzifierAssert(new LargestOfMaximum()).defuzzifies(-1, 1, {{new NaN(), fl::nan}});
        }

        SECTION("MeanOfMaximum") {
            DefuzzifierAssert(new MeanOfMaximum())
                .exports_fll("MeanOfMaximum")
                .configured_as("200")
                .has_parameters("200")
                .exports_fll("MeanOfMaximum 200");

            //        # Test case:
            //        #            ______
            //        #      _____/      \
            //        # ____/             \
            //        # |                   \____

            FL_unique_ptr<Discrete> term(new Discrete(
                "test",
                {
                    {0.0, 0.25},
                    {0.1, 0.25},
                    {0.2, 0.5},
                    {0.4, 0.5},
                    {0.5, 1.0},
                    {0.7, 1.0},  // # MOM: (0.5 + 0.9)/2=0.7
                    {0.9, 1.0},
                    {1.0, 0.0},
                }
            ));

            DefuzzifierAssert(new MeanOfMaximum()).defuzzifies(-fl::inf, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new MeanOfMaximum()).defuzzifies(-fl::inf, 0.0, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new MeanOfMaximum()).defuzzifies(0.0, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new MeanOfMaximum()).defuzzifies(fl::nan, fl::nan, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new MeanOfMaximum()).defuzzifies(fl::nan, 0, {{term->clone(), fl::nan}});
            DefuzzifierAssert(new MeanOfMaximum()).defuzzifies(0, fl::nan, {{term->clone(), fl::nan}});

            DefuzzifierAssert(new MeanOfMaximum())
                .defuzzifies(
                    0,
                    1,
                    {{term->clone(), 0.7},
                     {
                         new Trapezoid("", 0.0, 0.2, 0.4, 0.6),
                         0.3,
                     }}
                );
            DefuzzifierAssert(new MeanOfMaximum()).defuzzifies(-1, 1, {{new NaN(), fl::nan}});
        }
    }

}
