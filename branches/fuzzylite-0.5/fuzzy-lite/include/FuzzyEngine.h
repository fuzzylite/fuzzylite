/* 
 * File:   FuzzyEngine.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 4:51 PM
 */

#ifndef _FUZZYENGINE_H
#define	_FUZZYENGINE_H

#include "FuzzyOperator.h"

#include "InputLVar.h"
#include "OutputLVar.h"
#include "Hedge.h"
#include "FuzzyRule.h"

namespace fuzzy_lite {
    class FuzzyRule; //forward declaration

    class FuzzyEngine {
    private:
        FuzzyOperator* _fuzzy_op;
        std::vector<InputLVar*> _input_lvars;
        std::vector<OutputLVar*> _output_lvars;
        std::vector<Hedge*> _hedges;
        std::vector<FuzzyRule*> _rules;
        bool _winner_rule;
    public:
        FuzzyEngine(FuzzyOperator& fuzzy_operator);
        virtual ~FuzzyEngine();

        virtual void process();

        virtual void addInputLVar(InputLVar& lvar);
        virtual InputLVar* getInputLVar(const std::string& name) const;
        virtual InputLVar& getInputLVar(int index) const throw (OutOfRangeException);
        virtual InputLVar* removeInputLVar(const std::string& name) throw (InvalidArgumentException);
        virtual int indexOfInputLVar(const std::string& name) const;
        virtual int numberOfInputLVars() const;

        virtual void addOutputLVar(OutputLVar& lvar);
        virtual OutputLVar* getOutputLVar(const std::string& name) const;
        virtual OutputLVar& getOutputLVar(int index) const throw (OutOfRangeException);
        virtual OutputLVar* removeOutputLVar(const std::string& name) throw (InvalidArgumentException);
        virtual int indexOfOutputLVar(const std::string& name) const;
        virtual int numberOfOutputLVars() const;

        virtual void addHedge(Hedge& hedge);
        virtual Hedge* getHedge(const std::string& name) const;

        virtual void setFuzzyOperator(FuzzyOperator& fuzzy_operator);
        virtual FuzzyOperator& getFuzzyOperator() const;

        virtual void addRule(FuzzyRule& rule);
        virtual FuzzyRule& getRule(int index) const throw (OutOfRangeException);
        virtual FuzzyRule* removeRule(int index) throw (OutOfRangeException);
        virtual std::vector<FuzzyRule*> removeAllRules();
        virtual flScalar evaluateRule(FuzzyRule& rule);
        virtual int numberOfRules() const;


        virtual void setInput(const std::string& input_lvar, flScalar value) throw (InvalidArgumentException);
        virtual flScalar getOutput(const std::string& output_lvar) const throw (InvalidArgumentException);

        virtual std::string getFuzzyOutput(const std::string& output_lvar) const throw (InvalidArgumentException);


        static void testSimple();
        static void testComplex();
        static void main(int argc, char** argv);

    };
}


#endif	/* _FUZZYENGINE_H */

