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

    /**
     * Tests: norm/NormFunctions
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    static std::string snormEngine() {
#ifdef FL_CPP98
        return "";
#else
        return R""(
Engine: tipper
InputVariable: service
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: poor Gaussian 0.000 1.500
  term: good Gaussian 5.000 1.500
  term: excellent Gaussian 10.000 1.500
InputVariable: food
  enabled: true
  range: 0.000 10.000
  lock-range: false
  term: rancid Trapezoid 0.000 0.000 1.000 3.000
  term: delicious Trapezoid 7.000 9.000 10.000 10.000
OutputVariable: tip
  enabled: true
  range: 0.000 30.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: cheap Triangle 0.000 5.000 10.000
  term: average Triangle 10.000 15.000 20.000
  term: generous Triangle 20.000 25.000 30.000
RuleBlock:
  enabled: true
  conjunction: Minimum
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if service is poor or food is rancid then tip is cheap
  rule: if service is good then tip is average
  rule: if service is excellent or food is delicious then tip is generous
)"";
#endif
    }

    static SNorm* myMaximumNorm() {
        return new SNormFunction("max(a,b)");
    }

    static SNorm* myNotSoMaximumNorm() {
        return new SNormFunction("max(a,b) * 0.5");
    }

    TEST_CASE("SNormFunction (max(a,b)) is equivalent to Maximum", "[snorm][maximum]") {
#ifdef FL_CPP98
        FL_IUNUSED(&(myMaximumNorm));
        FL_IUNUSED(&(myNotSoMaximumNorm));
        FL_IUNUSED(&(snormEngine));
        WARN("Test only runs with -DFL_CPP98=OFF");
        return;
#else
        std::string fllEngine = snormEngine();
        FL_unique_ptr<Engine> engine(FllImporter().fromString(fllEngine));
        std::string fld = FldExporter().toString(engine.get(), 1024);

        SNormFactory* factory = FactoryManager::instance()->snorm();
        factory->registerConstructor("Maximum", &(myMaximumNorm));

        //Check our custom SNorm is registered
        FL_unique_ptr<SNorm> x(factory->constructObject("Maximum"));
        CHECK(Op::isEq(x->compute(0, 0.5), 0.5));

        //Test creating an engine with the new SNorm
        engine.reset(FllImporter().fromString(fllEngine));
        std::string anotherFld = FldExporter().toString(engine.get(), 1024);

        CHECK(fld == anotherFld);

        //Make sure a different SNorm fails in results

        factory->registerConstructor("Maximum", &(myNotSoMaximumNorm));
        engine.reset(FllImporter().fromString(fllEngine));
        anotherFld = FldExporter().toString(engine.get(), 1024);

        CHECK(fld != anotherFld);
#endif
    }

    static std::string tnormEngine() {
#ifdef FL_CPP98
        return "";
#else
        return R""(
Engine: mam21
InputVariable: angle
  enabled: true
  range: -5.000 5.000
  lock-range: false
  term: small Bell -5.000 5.000 8.000
  term: big Bell 5.000 5.000 8.000
InputVariable: velocity
  enabled: true
  range: -5.000 5.000
  lock-range: false
  term: small Bell -5.000 5.000 2.000
  term: big Bell 5.000 5.000 2.000
OutputVariable: force
  enabled: true
  range: -5.000 5.000
  lock-range: false
  aggregation: Maximum
  defuzzifier: Centroid 200
  default: nan
  lock-previous: false
  term: negBig Bell -5.000 1.670 8.000
  term: negSmall Bell -1.670 1.670 8.000
  term: posSmall Bell 1.670 1.670 8.000
  term: posBig Bell 5.000 1.670 8.000
RuleBlock:
  enabled: true
  conjunction: Minimum
  disjunction: Maximum
  implication: Minimum
  activation: General
  rule: if angle is small and velocity is small then force is negBig
  rule: if angle is small and velocity is big then force is negSmall
  rule: if angle is big and velocity is small then force is posSmall
  rule: if angle is big and velocity is big then force is posBig
)"";
#endif
    }

    static TNorm* myMinimumNorm() {
        return new TNormFunction("min(a,b)");
    }

    static TNorm* myNotSoMinimumNorm() {
        return new TNormFunction("min(a,b) * 0.5");
    }

    TEST_CASE("TNormFunction (min(a,b)) is equivalent to Minimum", "[tnorm][minimum]") {
#ifdef FL_CPP98
        FL_IUNUSED(&(myMinimumNorm));
        FL_IUNUSED(&(myNotSoMinimumNorm));
        FL_IUNUSED(&(tnormEngine));
        WARN("Test only runs with -DFL_CPP98=OFF");
        return;
#else
        std::string fllEngine = tnormEngine();
        FL_unique_ptr<Engine> engine(FllImporter().fromString(fllEngine));
        std::string fld = FldExporter().toString(engine.get(), 1024);

        TNormFactory* factory = FactoryManager::instance()->tnorm();
        factory->registerConstructor("Minimum", &(myMinimumNorm));

        //Check our custom SNorm is registered
        FL_unique_ptr<TNorm> x(factory->constructObject("Minimum"));
        CHECK(Op::isEq(x->compute(0.5, 1), 0.5));

        //Test creating an engine with the new SNorm
        engine.reset(FllImporter().fromString(fllEngine));
        std::string anotherFld = FldExporter().toString(engine.get(), 1024);

        CHECK(fld == anotherFld);

        //Make sure a different SNorm fails in results

        factory->registerConstructor("Minimum", &(myNotSoMinimumNorm));
        engine.reset(FllImporter().fromString(fllEngine));
        anotherFld = FldExporter().toString(engine.get(), 1024);

        CHECK(fld != anotherFld);
#endif
    }

}
