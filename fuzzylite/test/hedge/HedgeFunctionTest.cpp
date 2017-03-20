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
     * Tests: hedge/HedgeFunction
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    static std::string hedgeEngine() {
#ifdef FL_CPP98
        return "";
#else
        return R""(
Engine: Sugeno-Tip-Calculator
InputVariable: FoodQuality
  enabled: true
  range: 1.000 10.000
  lock-range: false
  term: Bad Trapezoid 0.000 1.000 3.000 7.000
  term: Good Trapezoid 3.000 7.000 10.000 11.000
InputVariable: Service
  enabled: true
  range: 1.000 10.000
  lock-range: false
  term: Bad Trapezoid 0.000 1.000 3.000 7.000
  term: Good Trapezoid 3.000 7.000 10.000 11.000
OutputVariable: CheapTip
  enabled: true
  range: 5.000 25.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: Low Constant 10.000
  term: Medium Constant 15.000
  term: High Constant 20.000
OutputVariable: AverageTip
  enabled: true
  range: 5.000 25.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: Low Constant 10.000
  term: Medium Constant 15.000
  term: High Constant 20.000
OutputVariable: GenerousTip
  enabled: true
  range: 5.000 25.000
  lock-range: false
  aggregation: none
  defuzzifier: WeightedAverage TakagiSugeno
  default: nan
  lock-previous: false
  term: Low Constant 10.000
  term: Medium Constant 15.000
  term: High Constant 20.000
RuleBlock:
  enabled: true
  conjunction: EinsteinProduct
  disjunction: none
  implication: none
  activation: General
  rule: if FoodQuality is extremely Bad and Service is extremely Bad then CheapTip is extremely Low and AverageTip is very Low and GenerousTip is Low
  rule: if FoodQuality is Good and Service is extremely Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium
  rule: if FoodQuality is very Good and Service is very Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is Bad and Service is Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium
  rule: if FoodQuality is Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is extremely Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is very High
  rule: if FoodQuality is Bad and Service is Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is Good and Service is Good then CheapTip is Medium and AverageTip is Medium and GenerousTip is very High
  rule: if FoodQuality is very Bad and Service is very Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High
  rule: if FoodQuality is very very Good and Service is very very Good then CheapTip is High and AverageTip is very High and GenerousTip is extremely High
)"";
#endif
    }

    static Hedge* myVeryConstructor() {
        return new HedgeFunction("x*x");
    }

    static Hedge* myExtraVeryConstructor() {
        return new HedgeFunction("x*x*x");
    }

    TEST_CASE("HedgeFunction x*x is equivalent to hedge Very", "[hedge][function]") {
#ifdef FL_CPP98
        FL_IUNUSED(&(hedgeEngine));
        FL_IUNUSED(&(myVeryConstructor));
        FL_IUNUSED(&(myExtraVeryConstructor));
        WARN("Test only runs with -DFL_CPP98=OFF");
        return;
#else
        std::string fllEngine = hedgeEngine();
        //Import using regular hedge very
        FL_unique_ptr<Engine> engine(FllImporter().fromString(fllEngine));
        std::string fldVery = FldExporter().toString(engine.get(), 1024);

        //Replace hedge very with a HedgeFunction(x*x)
        HedgeFactory* factory = FactoryManager::instance()->hedge();
        factory->registerConstructor("very", &(myVeryConstructor));
        //Import again with new HedgeFunction
        engine.reset(FllImporter().fromString(fllEngine));
        std::string anotherFld = FldExporter().toString(engine.get(), 1024);
        //Both must be equal
        CHECK(fldVery == anotherFld);

        //Replace very with a HedgeFunction(x*x*x)
        factory->registerConstructor("very", &(myExtraVeryConstructor));

        engine.reset(FllImporter().fromString(fllEngine));
        anotherFld = FldExporter().toString(engine.get(), 1024);

        //Must be different
        CHECK(fldVery != anotherFld);
#endif
    }

}
