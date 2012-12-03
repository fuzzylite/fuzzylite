/*
 * Engine.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_ENGINE_H_
#define FL_ENGINE_H_

#include <vector>
#include <string>
#include <map>


namespace fl {

    class Variable;
    class RuleBlock;
    class Hedge;
    class Configuration;

    class Engine {
    protected:
        std::string _name;
        std::vector<Variable*> _inputVariables;
        std::vector<Variable*> _outputVariables;
        std::vector<RuleBlock*> _ruleblocks;
        std::map<std::string, Hedge*> _hedges;
        Configuration* _configuration;

    public:
        Engine(const std::string& name);
        virtual ~Engine();

        virtual void configure(Configuration* config, bool storeToDelete = true);

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        /**
         * Operations for iterable datatype _inputVariables
         */
        virtual void addInputVariable(Variable* inputVariable);
        virtual void insertInputVariable(Variable* inputVariable, int index);
        virtual Variable* getInputVariable(int index) const;
        virtual Variable* removeInputVariable(int index);
        virtual int numberOfInputVariables() const;
        virtual const std::vector<Variable*>& inputVariables() const;

        /**
         * Operations for iterable datatype _outputVariables
         */
        virtual void addOutputVariable(Variable* outputVariable);
        virtual void insertOutputVariable(Variable* outputVariable, int index);
        virtual Variable* getOutputVariable(int index) const;
        virtual Variable* removeOutputVariable(int index);
        virtual int numberOfOutputVariables() const;
        virtual const std::vector<Variable*>& outputVariables() const;

        /**
         * Operations for iterable datatype _ruleblocks
         */
        virtual void addRuleblock(RuleBlock* ruleblock);
        virtual void insertRuleblock(RuleBlock* ruleblock, int index);
        virtual RuleBlock* getRuleblock(int index) const;
        virtual RuleBlock* removeRuleblock(int index);
        virtual int numberOfRuleblocks() const;
        virtual const std::vector<RuleBlock*>& ruleblocks() const;

        /**
         * Operations for map _hedges
         */
        virtual void addHedge(Hedge* hedge);
        virtual Hedge* removeHedge(const std::string& name);
        virtual Hedge* getHedge(const std::string& name) const;
        virtual const std::map<std::string, Hedge*>& hedges() const;


    };

} /* namespace fl */
#endif /* FL_ENGINE_H_ */
