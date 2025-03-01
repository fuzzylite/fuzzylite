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

#include "Headers.h"

namespace fuzzylite { namespace test {
    class NaN : public Constant {};

    template <class T>
    struct DefuzzifierAssert {
        FL_unique_ptr<T> actual;

        DefuzzifierAssert() : actual(new T) {}

        DefuzzifierAssert& configured_as(const std::string& parameters) {
            FL_IUNUSED(parameters);
            if (auto integralDefuzzifier = dynamic_cast<IntegralDefuzzifier*>(actual.get()))
                integralDefuzzifier->setResolution(std::stoi(parameters));
            else if (auto weightedDefuzzifier = dynamic_cast<WeightedDefuzzifier*>(actual.get()))
                weightedDefuzzifier->setType(parameters);
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

        DefuzzifierAssert& can_clone() {
            std::unique_ptr<T> clone(actual->clone());
            FllExporter exporter;
            CHECK(exporter.toString(clone.get()) == exporter.toString(actual.get()));
            return *this;
        }

        DefuzzifierAssert& defuzzifies(
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

    class WeightedDefuzzifierAssert {
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

        WeightedDefuzzifierAssert&
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

    TEST_CASE("IntegralDefuzzifier", "[defuzzifier][integral]") {
        CHECK(IntegralDefuzzifier::defaultResolution() == 1000);
        IntegralDefuzzifier::setDefaultResolution(1000000);
        CHECK(IntegralDefuzzifier::defaultResolution() == 1000000);
        IntegralDefuzzifier::setDefaultResolution(1000);
        CHECK(IntegralDefuzzifier::defaultResolution() == 1000);
    }

    TEST_CASE("Bisector", "[defuzzifier][bisector]") {
        DefuzzifierAssert<Bisector>()
            .exports_fll("Bisector")
            .configured_as("200")
            .exports_fll("Bisector 200")
            .can_clone();

        DefuzzifierAssert<Bisector>().defuzzifies(
            0,
            1,
            {{new Triangle("", 0, 1, 1), 0.7065},
             {new Triangle("", 0, 0, 1), 0.2925},
             {new Triangle("", 0, 0.5, 1), 0.4995},
             {new Rectangle("", 0, 1), 0.4995}}
        );
        DefuzzifierAssert<Bisector>().defuzzifies(
            -1,
            1,
            {
                {new Rectangle("", -1, 1), -0.001},
            }
        );

        FL_unique_ptr<AlgebraicProduct> algebraicProduct(new AlgebraicProduct());
        FL_unique_ptr<Triangle> low(new Triangle("low", -1, -1, -0.5));
        FL_unique_ptr<Triangle> high(new Triangle("high", 0.5, 1, 1));
        DefuzzifierAssert<Bisector>().defuzzifies(
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
            }
        );
        DefuzzifierAssert<Bisector>().defuzzifies(0, 2, {{new NaN(), fl::nan}});
    }

    TEST_CASE("Centroid", "[defuzzifier][centroid]") {
        DefuzzifierAssert<Centroid>()
            .exports_fll("Centroid")
            .configured_as("200")
            .has_parameters("200")
            .exports_fll("Centroid 200")
            .can_clone();

        DefuzzifierAssert<Centroid>().defuzzifies(-inf, 0.0, {{new Triangle(), nan}});
        DefuzzifierAssert<Centroid>().defuzzifies(0.0, inf, {{new Triangle(), nan}});
        DefuzzifierAssert<Centroid>().defuzzifies(nan, 0.0, {{new Triangle(), nan}});
        //
        FL_unique_ptr<Minimum> minimum(new Minimum());
        FL_unique_ptr<Triangle> medium(new Triangle("Medium", 0.25, 0.5, 0.75));
        FL_unique_ptr<Triangle> high(new Triangle("High", 0.5, 0.75, 1.0));
        DefuzzifierAssert<Centroid>().defuzzifies(
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
              0.6896552}}

        );
        DefuzzifierAssert<Centroid>().defuzzifies(-1, 1, {{new NaN(), nan}});
    }

    TEST_CASE("SmallestOfMaximum", "[defuzzifier][som]") {
        SECTION("SmallestOfMaximum") {
            DefuzzifierAssert<SmallestOfMaximum>()
                .exports_fll("SmallestOfMaximum")
                .configured_as("200")
                .exports_fll("SmallestOfMaximum 200")
                .can_clone();

            /* Test case:
                        ______
                  _____/      \
             ____/             \
            |                   \____
            */
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

            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(-fl::inf, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(-fl::inf, 0.0, {{term->clone(), fl::nan}});
            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(0.0, fl::inf, {{term->clone(), fl::nan}});
            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(fl::nan, fl::nan, {{term->clone(), fl::nan}});
            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(fl::nan, 0, {{term->clone(), fl::nan}});
            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(0, fl::nan, {{term->clone(), fl::nan}});

            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(
                0,
                1,
                {
                    {term->clone(), 0.5},
                    {new Trapezoid("", 0.0, 0.2, 0.4, 0.6), 0.2},
                }
            );
            DefuzzifierAssert<SmallestOfMaximum>().defuzzifies(-1, 1, {{new NaN(), fl::nan}});
        }
    }

    TEST_CASE("LargestOfMaximum", "[defuzzifier][lom]") {
        DefuzzifierAssert<LargestOfMaximum>()
            .exports_fll("LargestOfMaximum")
            .configured_as("200")
            .has_parameters("200")
            .exports_fll("LargestOfMaximum 200")
            .can_clone();

        /* Test case:
                    ______
              _____/      \
         ____/             \
         |                   \____
        /
        */
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

        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(-fl::inf, fl::inf, {{term->clone(), fl::nan}});
        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(-fl::inf, 0.0, {{term->clone(), fl::nan}});
        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(0.0, fl::inf, {{term->clone(), fl::nan}});
        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(fl::nan, fl::nan, {{term->clone(), fl::nan}});
        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(fl::nan, 0, {{term->clone(), fl::nan}});
        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(0, fl::nan, {{term->clone(), fl::nan}});

        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(
            0,
            1,
            {
                {term->clone(), 0.9},
                {new Trapezoid("", 0.0, 0.2, 0.4, 0.6), 0.4},
            }
        );
        DefuzzifierAssert<LargestOfMaximum>().defuzzifies(-1, 1, {{new NaN(), fl::nan}});
    }

    TEST_CASE("MeanOfMaximum", "[defuzzifier][mom]") {
        DefuzzifierAssert<MeanOfMaximum>()
            .exports_fll("MeanOfMaximum")
            .configured_as("200")
            .has_parameters("200")
            .exports_fll("MeanOfMaximum 200")
            .can_clone();

        /* Test case:
                    ______
              _____/      \
         ____/             \
         |                   \____
        */
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

        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(-fl::inf, fl::inf, {{term->clone(), fl::nan}});
        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(-fl::inf, 0.0, {{term->clone(), fl::nan}});
        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(0.0, fl::inf, {{term->clone(), fl::nan}});
        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(fl::nan, fl::nan, {{term->clone(), fl::nan}});
        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(fl::nan, 0, {{term->clone(), fl::nan}});
        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(0, fl::nan, {{term->clone(), fl::nan}});

        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(
            0,
            1,
            {{term->clone(), 0.7},
             {
                 new Trapezoid("", 0.0, 0.2, 0.4, 0.6),
                 0.3,
             }}
        );
        DefuzzifierAssert<MeanOfMaximum>().defuzzifies(-1, 1, {{new NaN(), fl::nan}});
    }

    TEST_CASE("Infer defuzzifier type", "[defuzzifier][weighted]") {
        std::vector<fl::Term*> takagiSugenoTerms = {
            new fl::Constant(),
            new fl::Linear(),
            new fl::Function(),
        };
        std::vector<fl::Term*> tsukamotoTerms = {
            new fl::Concave(),
            new fl::Ramp(),
            new fl::SShape(),
            new fl::Sigmoid(),
            new fl::ZShape(),
        };
        SECTION("TakagiSugeno") {
            WeightedDefuzzifierAssert().inferredType(takagiSugenoTerms, WeightedDefuzzifier::TakagiSugeno);
        }
        SECTION("TakagiSugenoActivated") {
            std::vector<fl::Term*> activatedTerms;
            for (fl::Term* term : takagiSugenoTerms) {
                Activated* activated = new Activated();
                activated->setTerm(term);
                activatedTerms.push_back(activated);
            }
            WeightedDefuzzifierAssert().inferredType(activatedTerms, WeightedDefuzzifier::TakagiSugeno);
        }
        SECTION("TakagiSugenoAggregated") {
            std::vector<Activated> activatedTerms;
            for (fl::Term* term : takagiSugenoTerms) {
                Activated activated;
                activated.setTerm(term);
                activatedTerms.push_back(activated);
            }
            Aggregated* aggregated = new Aggregated;
            aggregated->setTerms(activatedTerms);
            WeightedDefuzzifierAssert().inferredType({aggregated}, WeightedDefuzzifier::TakagiSugeno);
        }

        SECTION("Tsukamoto") {
            WeightedDefuzzifierAssert().inferredType(tsukamotoTerms, WeightedDefuzzifier::Tsukamoto);
        }

        SECTION("TsukamotoActivated") {
            std::vector<fl::Term*> activatedTerms;
            for (fl::Term* term : tsukamotoTerms) {
                Activated* activated = new Activated();
                activated->setTerm(term);
                activatedTerms.push_back(activated);
            }
            WeightedDefuzzifierAssert().inferredType(activatedTerms, WeightedDefuzzifier::Tsukamoto);
        }
        SECTION("TsukamotoAggregated") {
            std::vector<Activated> activatedTerms;
            for (fl::Term* term : tsukamotoTerms) {
                Activated activated;
                activated.setTerm(term);
                activatedTerms.push_back(activated);
            }
            Aggregated* aggregated = new Aggregated;
            aggregated->setTerms(activatedTerms);
            WeightedDefuzzifierAssert().inferredType({aggregated}, WeightedDefuzzifier::Tsukamoto);
        }

        SECTION("Can't infer") {
            WeightedDefuzzifierAssert().inferredType({new Aggregated}, WeightedDefuzzifier::Automatic);
        }

        SECTION("Mixed types") {
            WeightedDefuzzifierAssert::MockDefuzzifier defuzzifier;
            std::vector<const fl::Term*> mixedTypes = {new Constant, new Concave};
            CHECK_THROWS_AS(defuzzifier.inferType(mixedTypes), fl::Exception);
            CHECK_THROWS_WITH(
                defuzzifier.inferType(mixedTypes),
                Catch::Matchers::StartsWith("cannot infer type of") && Catch::Matchers::EndsWith(", got multiple types")
            );
        }
    }

    TEST_CASE("WeightedAverage", "[defuzzifier][weighted]") {
        CHECK(WeightedAverage("TakagiSugeno").toString() == "WeightedAverage TakagiSugeno");
        CHECK(WeightedAverage("Tsukamoto").toString() == "WeightedAverage Tsukamoto");
        CHECK(WeightedAverage("Automatic").toString() == "WeightedAverage");
        CHECK(WeightedAverage("Invalid").toString() == "WeightedAverage");

        DefuzzifierAssert<WeightedAverage>().exports_fll("WeightedAverage").can_clone();

        DefuzzifierAssert<WeightedAverage>()
            .configured_as("TakagiSugeno")
            .exports_fll("WeightedAverage TakagiSugeno")
            .configured_as("Tsukamoto")
            .exports_fll("WeightedAverage Tsukamoto");

        FL_unique_ptr<Constant> a(new Constant("A"));
        FL_unique_ptr<Constant> b(new Constant("B"));
        FL_unique_ptr<Constant> c(new Constant("C"));

        a->setValue(1.0);
        b->setValue(2.0);
        c->setValue(3.0);
        DefuzzifierAssert<WeightedAverage>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {
                    {new fl::Aggregated(), fl::nan},
                    {new fl::Aggregated(
                         "A",
                         fl::nan,
                         fl::nan,
                         fl::null,
                         {Activated(a.get(), 1.0), Activated(b.get(), 1.0), Activated(c.get(), 1.0)}
                     ),
                     2.0},
                    {new fl::Aggregated(
                         "A",
                         fl::nan,
                         fl::nan,
                         fl::null,
                         {Activated(a.get(), 1.0), Activated(b.get(), 0.5), Activated(c.get(), 1.0)}
                     ),
                     2.0},
                }
            );
        a->setValue(-1.0);
        b->setValue(-2.0);
        c->setValue(3.0);
        DefuzzifierAssert<WeightedAverage>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {
                    {new fl::Aggregated(
                         "A",
                         fl::nan,
                         fl::nan,
                         fl::null,
                         {fl::Activated(a.get(), 1.0), fl::Activated(b.get(), 1.0), fl::Activated(c.get(), 1.0)}
                     ),
                     0.0},
                }
            );

        a->setValue(1.0);
        b->setValue(-2.0);
        c->setValue(-3.0);
        DefuzzifierAssert<WeightedAverage>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {
                    {new fl::Aggregated(
                         "A",
                         fl::nan,
                         fl::nan,
                         fl::null,
                         {fl::Activated(a.get(), 1.0), fl::Activated(b.get(), 1.0), fl::Activated(c.get(), 0.5)}
                     ),
                     -1.0},
                }
            );
    }

    TEST_CASE("WeightedAverage Grouped", "[defuzzifier][weighted]") {
        FL_unique_ptr<Constant> a(new Constant("A"));
        FL_unique_ptr<Constant> b(new Constant("B"));
        FL_unique_ptr<Constant> c(new Constant("C"));

        a->setValue(1.0);
        b->setValue(2.0);
        c->setValue(3.0);

        DefuzzifierAssert<WeightedAverage>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(a.get(), 0.5),
                       Activated(a.get(), 0.5),
                       Activated(b.get(), 1.0),
                       Activated(c.get(), 1.0)}
                  ),
                  2.0},
                 {new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(a.get(), 1.0),
                       Activated(b.get(), 0.25),
                       Activated(b.get(), 0.25),
                       Activated(c.get(), 1.0)}
                  ),
                  2.0}}
            );

        a->setValue(-1.0);
        b->setValue(-2.0);
        c->setValue(3.0);
        DefuzzifierAssert<WeightedAverage>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      new Maximum(),
                      {Activated(a.get(), 1.0),
                       Activated(a.get(), 1.0),
                       Activated(b.get(), 1.0),
                       Activated(c.get(), 1.0)}
                  ),
                  0.0}}
            );

        a->setValue(1.0);
        b->setValue(-2.0);
        c->setValue(-3.0);
        DefuzzifierAssert<WeightedAverage>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{
                    new fl::Aggregated(
                        "A",
                        fl::nan,
                        fl::nan,
                        new AlgebraicSum(),
                        {Activated(a.get(), 1.0),
                         Activated(b.get(), 1.0),
                         Activated(c.get(), 0.5),
                         Activated(c.get(), 0.5)}
                    ),
                    -1.181818  //((1 * 1 + 1 * -2 + (0.5 + 0.5 - (0.25)) * -3) / (1 + 1 + (0.5 + 0.5 - 0.25)))
                }}
            );
    }

    TEST_CASE("WeightedSum", "[defuzzifier][weighted]") {
        CHECK(WeightedSum("TakagiSugeno").toString() == "WeightedSum TakagiSugeno");
        CHECK(WeightedSum("Tsukamoto").toString() == "WeightedSum Tsukamoto");
        CHECK(WeightedSum("Automatic").toString() == "WeightedSum");
        CHECK(WeightedSum("Invalid").toString() == "WeightedSum");

        DefuzzifierAssert<WeightedSum>().exports_fll("WeightedSum").can_clone();

        DefuzzifierAssert<WeightedSum>()
            .exports_fll("WeightedSum")
            .configured_as("TakagiSugeno")
            .exports_fll("WeightedSum TakagiSugeno")
            .configured_as("Tsukamoto")
            .exports_fll("WeightedSum Tsukamoto");

        FL_unique_ptr<Constant> a(new Constant("A"));
        FL_unique_ptr<Constant> b(new Constant("B"));
        FL_unique_ptr<Constant> c(new Constant("C"));

        a->setValue(1.0);
        b->setValue(2.0);
        c->setValue(3.0);
        DefuzzifierAssert<WeightedSum>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(), fl::nan},
                 {new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(a.get(), 1.0),
                          fl::Activated(b.get(), 1.0),
                          fl::Activated(c.get(), 1.0),
                      }
                  ),
                  6.0},
                 {new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(a.get(), 1.0),
                          fl::Activated(b.get(), 0.5),
                          fl::Activated(c.get(), 1.0),
                      }
                  ),
                  5.0}}
            );

        a->setValue(-1.0);
        b->setValue(-2.0);
        c->setValue(3.0);

        DefuzzifierAssert<WeightedSum>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(a.get(), 1.0),
                          fl::Activated(b.get(), 1.0),
                          fl::Activated(c.get(), 1.0),
                      }
                  ),
                  0.0}}
            );

        a->setValue(1.0);
        b->setValue(-2.0);
        c->setValue(-3.0);
        DefuzzifierAssert<WeightedSum>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(a.get(), 1.0),
                          fl::Activated(b.get(), 1.0),
                          fl::Activated(c.get(), 0.5),
                      }
                  ),
                  -2.5}}
            );
    }

    TEST_CASE("WeightedSum Grouped", "[defuzzifier][weighted]") {
        FL_unique_ptr<Constant> a(new Constant("A", 1.0));
        DefuzzifierAssert<WeightedSum>()
            .configured_as("TakagiSugeno")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(), fl::nan},
                 {new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          Activated(a.get(), 0.0),
                          Activated(a.get(), 0.3),
                          Activated(a.get(), 0.6),
                      }
                  ),
                  (0.9 * 1.0)},
                 {new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          Activated(a.get(), 1.0),
                          Activated(a.get(), 0.3),
                          Activated(a.get(), 0.6),
                      }
                  ),
                  (1.9 * 1.0)},
                 {new fl::Aggregated(
                      "A",
                      fl::nan,
                      fl::nan,
                      new fl::Maximum(),
                      {
                          Activated(a.get(), 0.0),
                          Activated(a.get(), 0.3),
                          Activated(a.get(), 0.6),
                      }
                  ),
                  (0.6 * 1.0)},
                 {new fl::Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      new fl::AlgebraicSum(),
                      {
                          Activated(a.get(), 0.0),
                          Activated(a.get(), 0.3),
                          Activated(a.get(), 0.6),
                      }
                  ),
                  (0.72 * 1.0)}}
            );
    }

    TEST_CASE("WeightedSum Tsukamoto", "[defuzzifier][weighted][tsukamoto]") {
        DefuzzifierAssert<WeightedSum>()
            .configured_as("Tsukamoto")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new Aggregated(), fl::nan},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          Activated(new Ramp("a", 0, 0.25), 0.015),
                          Activated(new Ramp("b", 0.6, 0.4), 1.0),
                          Activated(new Ramp("c", 0.7, 1.0), 0.015),
                      }
                  ),
                  0.410},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(new Sigmoid("a", 0.13, 30), 0.015),
                       Activated(new Sigmoid("b", 0.5, -30), 1.0),
                       Activated(new Sigmoid("c", 0.83, 30), 0.015)}
                  ),
                  -fl::inf},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(new Concave("a", 0.24, 0.25), 0.015),
                       Activated(new Concave("b", 0.5, 0.4), 1.0),
                       Activated(new Concave("c", 0.9, 1.0), 0.015)}
                  ),
                  0.310},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(new SShape("a", 0.000, 0.250), 0.015),
                       Activated(new ZShape("b", 0.300, 0.600), 1.0),
                       Activated(new SShape("c", 0.700, 1.000), 0.015)}
                  ),
                  0.311}}
            );
    }

    TEST_CASE("WeightedAverage Tsukamoto", "[defuzzifier][weighted][tsukamoto]") {
        DefuzzifierAssert<WeightedAverage>()
            .configured_as("Tsukamoto")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new Aggregated(), fl::nan},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          Activated(new Ramp("a", 0, 0.25), 0.015),
                          Activated(new Ramp("b", 0.6, 0.4), 1.0),
                          Activated(new Ramp("c", 0.7, 1.0), 0.015),
                      }
                  ),
                  0.398},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(new Sigmoid("a", 0.13, 30), 0.015),
                       Activated(new Sigmoid("b", 0.5, -30), 1.0),
                       Activated(new Sigmoid("c", 0.83, 30), 0.015)}
                  ),
                  -fl::inf},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(new Concave("a", 0.24, 0.25), 0.015),
                       Activated(new Concave("b", 0.5, 0.4), 1.0),
                       Activated(new Concave("c", 0.9, 1.0), 0.015)}
                  ),
                  0.301},
                 {new Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {Activated(new SShape("a", 0.000, 0.250), 0.015),
                       Activated(new ZShape("b", 0.300, 0.600), 1.0),
                       Activated(new SShape("c", 0.700, 1.000), 0.015)}
                  ),
                  0.302}}
            );
    }

    TEST_CASE("WeightedSum Tsukamoto Grouped", "[defuzzifier][weighted][tsukamoto]") {
        DefuzzifierAssert<fl::WeightedSum>()
            .configured_as("Tsukamoto")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(new fl::Ramp("a", 0, 0.25), 0.0075),
                          fl::Activated(new fl::Ramp("a", 0, 0.25), 0.0075),
                          fl::Activated(new fl::Ramp("b", 0.6, 0.4), 1.0),
                          fl::Activated(new fl::Ramp("c", 0.7, 1.0), 0.015),
                      }
                  ),
                  0.410},
                 {new fl::Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(new fl::Concave("a", 0.24, 0.25), 0.015),
                          fl::Activated(new fl::Concave("b", 0.5, 0.4), 0.5),
                          fl::Activated(new fl::Concave("b", 0.5, 0.4), 0.5),
                          fl::Activated(new fl::Concave("c", 0.9, 1.0), 0.015),
                      }
                  ),
                  0.310},
                 {new fl::Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      new fl::Maximum(),
                      {
                          fl::Activated(new fl::SShape("a", 0.000, 0.250), 0.015),
                          fl::Activated(new fl::ZShape("b", 0.300, 0.600), 1.0),
                          fl::Activated(new fl::ZShape("b", 0.300, 0.600), 1.0),
                          fl::Activated(new fl::SShape("c", 0.700, 1.000), 0.015),
                      }
                  ),
                  0.311}}
            );
    }

    TEST_CASE("WeightedAverage Tsukamoto Grouped", "[defuzzifier][weighted][tsukamoto]") {
        DefuzzifierAssert<fl::WeightedAverage>()
            .configured_as("Tsukamoto")
            .defuzzifies(
                -fl::inf,
                fl::inf,
                {{new fl::Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(new fl::Ramp("a", 0, 0.25), 0.0075),
                          fl::Activated(new fl::Ramp("a", 0, 0.25), 0.0075),
                          fl::Activated(new fl::Ramp("b", 0.6, 0.4), 1.0),
                          fl::Activated(new fl::Ramp("c", 0.7, 1.0), 0.015),
                      }
                  ),
                  0.398},
                 {new fl::Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      fl::null,
                      {
                          fl::Activated(new fl::Concave("a", 0.24, 0.25), 0.015),
                          fl::Activated(new fl::Concave("b", 0.5, 0.4), 0.5),
                          fl::Activated(new fl::Concave("b", 0.5, 0.4), 0.5),
                          fl::Activated(new fl::Concave("c", 0.9, 1.0), 0.015),
                      }
                  ),
                  0.301},
                 {new fl::Aggregated(
                      "",
                      fl::nan,
                      fl::nan,
                      new fl::Maximum(),
                      {
                          fl::Activated(new fl::SShape("a", 0.000, 0.250), 0.015),
                          fl::Activated(new fl::ZShape("b", 0.300, 0.600), 1.0),
                          fl::Activated(new fl::ZShape("b", 0.300, 0.600), 1.0),
                          fl::Activated(new fl::SShape("c", 0.700, 1.000), 0.015),
                      }
                  ),
                  0.302}}
            );
    }

    TEST_CASE("all defuzzifiers return nan when term is empty", "[defuzzifier]") {
        fl::DefuzzifierFactory factory;
        Aggregated aggregated;
        for (const std::string& name : factory.available()) {
            if (name.empty())
                continue;
            FL_unique_ptr<Defuzzifier> defuzzifier(factory.constructObject(name));
            CAPTURE(defuzzifier->className());
            scalar obtained = defuzzifier->defuzzify(&aggregated, -fl::inf, fl::inf);
            CHECK_THAT(obtained, Approximates(fl::nan));
        }
    }
}}
