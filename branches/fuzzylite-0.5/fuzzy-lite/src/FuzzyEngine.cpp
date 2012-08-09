#include "FuzzyEngine.h"

#include "MamdaniRule.h"
#include "FuzzyOperator.h"
#include "TriangularDTerm.h"
#include "FuzzyExceptions.h"
#include "defs.h"
#include "StringOperator.h"

namespace fuzzy_lite {

    FuzzyEngine::FuzzyEngine(FuzzyOperator& fuzzy_operator) : _fuzzy_op(&fuzzy_operator) {

    }

    FuzzyEngine::~FuzzyEngine() {

    }

    void FuzzyEngine::process() {
        for (int i = 0 ; i < numberOfOutputLVars(); ++i){
            getOutputLVar(i).clearOutput();
        }
        for (size_t i = 0; i < _rules.size(); ++i) {
            _rules[i]->fire(_rules[i]->evaluate(getFuzzyOperator()), getFuzzyOperator());
        }
    }

    //INPUTLVAR

    void FuzzyEngine::addInputLVar(InputLVar& lvar) {
        _input_lvars.push_back(&lvar);
    }

    InputLVar* FuzzyEngine::getInputLVar(const std::string& name) const {
        for (size_t i = 0; i < _input_lvars.size(); ++i) {
            if (_input_lvars[i]->getName() == name) {
                return _input_lvars[i];
            }
        }
        return NULL;
    }

    InputLVar& FuzzyEngine::getInputLVar(int index) const throw (OutOfRangeException) {
        //        OutOfRangeException::CheckArray(FL_AT, index, numberOfInputLVars());
        return *_input_lvars[index];
    }

    InputLVar* FuzzyEngine::removeInputLVar(const std::string& name) throw (InvalidArgumentException) {
        int index = indexOfInputLVar(name);
        if (index == -1) {
            throw InvalidArgumentException(FL_AT, "Input variable <" + name + "> not registered in fuzzy engine");
        }
        InputLVar* result = &getInputLVar(index);
        _input_lvars.erase(_input_lvars.begin() + index);
        return result;
    }

    int FuzzyEngine::indexOfInputLVar(const std::string& name) const {
        for (size_t i = 0; i < _input_lvars.size(); ++i) {
            if (_input_lvars[i]->getName() == name) {
                return i;
            }
        }
        return -1;
    }

    int FuzzyEngine::numberOfInputLVars() const {
        return _input_lvars.size();
    }


    //OUTPUTLVAR

    void FuzzyEngine::addOutputLVar(OutputLVar& lvar) {
        _output_lvars.push_back(&lvar);
    }

    OutputLVar* FuzzyEngine::getOutputLVar(const std::string& name) const {
        for (size_t i = 0; i < _output_lvars.size(); ++i) {
            if (_output_lvars[i]->getName() == name) {
                return _output_lvars[i];
            }
        }
        return NULL;
    }

    OutputLVar& FuzzyEngine::getOutputLVar(int index) const throw (OutOfRangeException) {
        //        OutOfRangeException::CheckArray(FL_AT, index, numberOfOutputLVars());
        return *_output_lvars[index];
    }

    OutputLVar* FuzzyEngine::removeOutputLVar(const std::string& name) throw (InvalidArgumentException) {
        int index = indexOfOutputLVar(name);
        if (index == -1) {
            throw InvalidArgumentException(FL_AT, "Input variable <" + name + "> not registered in fuzzy engine");
        }
        OutputLVar* result = &getOutputLVar(index);
        _output_lvars.erase(_output_lvars.begin() + index);
        return result;
    }

    int FuzzyEngine::indexOfOutputLVar(const std::string& name) const {
        for (size_t i = 0; i < _output_lvars.size(); ++i) {
            if (_output_lvars[i]->getName() == name) {
                return i;
            }
        }
        return -1;
    }

    int FuzzyEngine::numberOfOutputLVars() const {
        return _output_lvars.size();
    }

    void FuzzyEngine::addHedge(Hedge& hedge) {
        _hedges.push_back(&hedge);
    }

    Hedge* FuzzyEngine::getHedge(const std::string& name) const {
        for (size_t i = 0; i < _hedges.size(); ++i) {
            if (_hedges[i]->getName() == name) {
                return _hedges[i];
            }
        }
        return NULL;
    }

    void FuzzyEngine::setFuzzyOperator(FuzzyOperator& fuzzy_operator) {
        this->_fuzzy_op = &fuzzy_operator;
    }

    FL_INLINE FuzzyOperator& FuzzyEngine::getFuzzyOperator() const {
        return *this->_fuzzy_op;
    }

    //RULES

    void FuzzyEngine::addRule(FuzzyRule& rule) {
        _rules.push_back(&rule);
    }

    FuzzyRule& FuzzyEngine::getRule(int index) const throw (OutOfRangeException) {
        //        OutOfRangeException::CheckArray(FL_AT, index, numberOfRules());
        return *_rules[index];
    }

    FuzzyRule* FuzzyEngine::removeRule(int index) throw (OutOfRangeException) {
        //        OutOfRangeException::CheckArray(FL_AT,index,numberOfRules());
        FuzzyRule* result = _rules[index];
        _rules.erase(_rules.begin() + index);
        return result;
    }

    std::vector<FuzzyRule*> FuzzyEngine::removeAllRules() {
        std::vector<FuzzyRule*> result = _rules;
        _rules.clear();
        return result;
    }

    flScalar FuzzyEngine::evaluateRule(FuzzyRule& rule) {
        return rule.evaluate(getFuzzyOperator());
    }

    int FuzzyEngine::numberOfRules() const {
        return _rules.size();
    }

    FL_INLINE void FuzzyEngine::setInput(const std::string& input_lvar, flScalar value) throw (InvalidArgumentException) {
        InputLVar* input = getInputLVar(input_lvar);
        if (!input) {
            throw InvalidArgumentException(FL_AT, "Input variable <" + input_lvar + "> not registered in fuzzy engine");
        }
        input->setInput(value);
    }

    FL_INLINE flScalar FuzzyEngine::getOutput(const std::string& output_lvar) const throw (InvalidArgumentException) {
        OutputLVar* output = getOutputLVar(output_lvar);
        if (!output) {
            throw InvalidArgumentException(FL_AT, "Output variable <" + output_lvar + "> not registered in fuzzy engine");
        }
        return output->getDefuzzifiedOutput(getFuzzyOperator());
    }

    std::string FuzzyEngine::getFuzzyOutput(const std::string& output_lvar) const throw (InvalidArgumentException) {
        flScalar defuzzified = getOutput(output_lvar);
        FL_DEBUG("defuzzy: " << defuzzified);
        OutputLVar* output = getOutputLVar(output_lvar);
        if (!output) {
            throw InvalidArgumentException(FL_AT, "Output variable <" + output_lvar + "> not registered in fuzzy engine");
        }
        return output->fuzzify(defuzzified);
    }

    void FuzzyEngine::testComplex() {
        FuzzyOperator* fuzzy_op = new FuzzyOperator();
        FuzzyEngine engine(*fuzzy_op);

        InputLVar energy("Energy");
        TriangularDTerm low_energy("LOW", 0, 50);
        TriangularDTerm med_energy("MEDIUM", 25, 75);
        TriangularDTerm hi_energy("HIGH", 50, 100);
        energy.addTerm(low_energy);
        energy.addTerm(med_energy);
        energy.addTerm(hi_energy);
        engine.addInputLVar(energy);

        InputLVar distance("Distance");
        TriangularDTerm near_distance("NEAR", 0, 500);
        TriangularDTerm far_distance("FAR", 250, 750);
        TriangularDTerm faraway_distance("FAR_AWAY", 500, 1000);
        distance.addTerm(near_distance);
        distance.addTerm(far_distance);
        distance.addTerm(faraway_distance);
        engine.addInputLVar(distance);

        OutputLVar power("Power");
        TriangularDTerm low_power("LOW", 0, 100);
        TriangularDTerm med_power("MEDIUM", 50, 150);
        TriangularDTerm hi_power("HIGH", 100, 200);
        power.addTerm(low_power);
        power.addTerm(med_power);
        power.addTerm(hi_power);
        engine.addOutputLVar(power);

        HedgeNot h_not;
        HedgeSomewhat h_somewhat;
        HedgeVery h_very;
        engine.addHedge(h_not);
        engine.addHedge(h_somewhat);
        engine.addHedge(h_very);

        MamdaniRule rule1;
        MamdaniRule rule2;
        MamdaniRule rule3;
        MamdaniRule rule4;
        MamdaniRule rule5;
        MamdaniRule rule6;
        MamdaniRule rule7;
        MamdaniRule rule8;
        MamdaniRule rule9;

        rule1.parse("if Energy is LOW and Distance is FAR_AWAY then Power is LOW", engine);
        rule2.parse("if Energy is LOW and Distance is FAR then Power is MEDIUM", engine);
        rule3.parse("if Energy is LOW and Distance is NEAR then Power is HIGH", engine);
        rule4.parse("if Energy is MEDIUM and Distance is FAR_AWAY then Power is LOW", engine);
        rule5.parse("if Energy is MEDIUM and Distance is FAR then Power is MEDIUM", engine);
        rule6.parse("if Energy is MEDIUM and Distance is NEAR then Power is HIGH", engine);
        rule7.parse("if Energy is HIGH and Distance is FAR_AWAY then Power is LOW", engine);
        rule8.parse("if Energy is HIGH and Distance is FAR then Power is MEDIUM", engine);
        rule9.parse("if Energy is HIGH and Distance is NEAR then Power is HIGH", engine);

        engine.addRule(rule1);
        engine.addRule(rule2);
        engine.addRule(rule3);
        engine.addRule(rule4);
        engine.addRule(rule5);
        engine.addRule(rule6);
        engine.addRule(rule7);
        engine.addRule(rule8);
        engine.addRule(rule9);

        energy.setInput(48);
        distance.setInput(900);
        FL_DEBUG(engine.evaluateRule(rule1));
        engine.process();
        FL_DEBUG(power.getOutput().toString())
                //        FL_LOGM(engine.getFuzzyOutput("Power"));

    }

    void FuzzyEngine::testSimple() {
        FuzzyOperator* op = new FuzzyOperator();
        FuzzyEngine engine(*op);

        InputLVar energy("Energy");
        TriangularDTerm low_energy("LOW", 0, 0, 50);
        low_energy.setFromInfinity(true);
        TriangularDTerm med_energy("MEDIUM", 25, 75);
        TriangularDTerm hi_energy("HIGH", 50, 100, 100);
        hi_energy.setToInfinity(true);
        energy.addTerm(low_energy);
        energy.addTerm(med_energy);
        energy.addTerm(hi_energy);
        engine.addInputLVar(energy);

        OutputLVar health("Health");
        TriangularDTerm bad_health("BAD", 0, 100);
        TriangularDTerm reg_health("REGULAR", 50, 150);
        TriangularDTerm good_health("GOOD", 100, 200);
        health.addTerm(bad_health);
        health.addTerm(reg_health);
        health.addTerm(good_health);
        engine.addOutputLVar(health);

        MamdaniRule rule1;
        MamdaniRule rule2;
        MamdaniRule rule3;

        rule1.parse("if Energy is LOW then Health is BAD", engine);
        rule2.parse("if Energy is MEDIUM then Health is REGULAR", engine);
        rule3.parse("if Energy is HIGH then Health is GOOD", engine);

        engine.addRule(rule1);
        engine.addRule(rule2);
        engine.addRule(rule3);

        FL_DEBUG(rule1.toString());
        engine.setInput("Energy", -1);
        engine.process();
        FL_DEBUG("Health: " << engine.getOutput("Health"));
        FL_DEBUG("Health is " << engine.getFuzzyOutput("Health"));
    }

    void FuzzyEngine::main(int argc, char** argv) {
        testSimple();
    }

}
