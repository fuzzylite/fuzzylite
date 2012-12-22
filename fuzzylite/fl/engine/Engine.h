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

    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    class Hedge;
    class Configuration;

    class Engine {
    protected:
        std::string _name;
        std::vector<InputVariable*> _inputVariables;
        std::vector<OutputVariable*> _outputVariables;
        std::vector<RuleBlock*> _ruleblocks;
        std::map<std::string, Hedge*> _hedges;
        Configuration* _configuration;

    public:
        Engine(const std::string& name = "");
        virtual ~Engine();

        virtual void configure(Configuration* config,
                bool storeToDelete = false);
        
        virtual void process();

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        /**
         * Operations for iterable datatype _inputVariables
         */
        virtual void addInputVariable(InputVariable* inputVariable);
        virtual void insertInputVariable(InputVariable* inputVariable, int index);
        virtual InputVariable* getInputVariable(int index) const;
        virtual InputVariable* getInputVariable(const std::string& name) const;
        virtual InputVariable* removeInputVariable(int index);
        virtual bool hasInputVariable(const std::string& name) const;
        virtual int numberOfInputVariables() const;
        virtual const std::vector<InputVariable*>& inputVariables() const;

        /**
         * Operations for iterable datatype _outputVariables
         */
        virtual void addOutputVariable(OutputVariable* outputVariable);
        virtual void insertOutputVariable(OutputVariable* outputVariable, int index);
        virtual OutputVariable* getOutputVariable(int index) const;
        virtual OutputVariable* getOutputVariable(const std::string& name) const;
        virtual bool hasOutputVariable(const std::string& name) const;
        virtual OutputVariable* removeOutputVariable(int index);
        virtual int numberOfOutputVariables() const;
        virtual const std::vector<OutputVariable*>& outputVariables() const;

        /**
         * Operations for iterable datatype _ruleblocks
         */
        virtual void addRuleBlock(RuleBlock* ruleblock);
        virtual void insertRuleBlock(RuleBlock* ruleblock, int index);
        virtual RuleBlock* getRuleBlock(int index) const;
        virtual RuleBlock* getRuleBlock(const std::string& name) const;
        virtual bool hasRuleBlock(const std::string& name) const;
        virtual RuleBlock* removeRuleBlock(int index);
        virtual int numberOfRuleBlocks() const;
        virtual const std::vector<RuleBlock*>& ruleBlocks() const;

        /**
         * Operations for map _hedges
         */
        virtual void addHedge(Hedge* hedge);
        virtual Hedge* removeHedge(const std::string& name);
        virtual Hedge* getHedge(const std::string& name) const;
        virtual bool hasHedge(const std::string& name) const;
        virtual const std::map<std::string, Hedge*>& hedges() const;


    };

} 
#endif /* FL_ENGINE_H_ */
