// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

/* 
 * File:   Copy.h
 * Author: jcrada
 *
 * Created on 17 March 2014, 12:50 PM
 */

#ifndef FL_COPY_H
#define	FL_COPY_H


namespace fl {

    class Engine;
    class Variable;
    class InputVariable;
    class OutputVariable;
    class RuleBlock;
    
    class Copy {
    public:
        Copy();
        virtual ~Copy();

        enum Type {
            DEEP, SHALLOW
        };

        virtual Engine* engine(const Engine* source, Engine* target, Type type);
        virtual Variable* variable(const Variable* source, Variable* target, Type type);
        virtual InputVariable* inputVariable(const InputVariable* source, InputVariable* target, Type type);
        virtual OutputVariable* outputVariable(const OutputVariable* source, OutputVariable* target, Type type);
        virtual RuleBlock* ruleBlock(const RuleBlock* source, RuleBlock* target, Type type);

        //Returns a copy of source without being processed.
        virtual Engine* deepEngine(const Engine* source, Engine* target);
        virtual Variable* deepVariable(const Variable* source, Variable* target);
        virtual InputVariable* deepInputVariable(const InputVariable* source, InputVariable* target);
        virtual OutputVariable* deepOutputVariable(const OutputVariable* source, OutputVariable* target);
        
        //Returns target with unloaded rules.
        virtual RuleBlock* deepRuleBlock(const RuleBlock* source, RuleBlock* target);

        virtual Engine* shallowEngine(const Engine* source, Engine* target);
        virtual Variable* shallowVariable(const Variable* source, Variable* target);
        virtual InputVariable* shallowInputVariable(const InputVariable* source, InputVariable* target);
        virtual OutputVariable* shallowOutputVariable(const OutputVariable* source, OutputVariable* target);
        virtual RuleBlock* shallowRuleBlock(const RuleBlock* source, RuleBlock* target);
    };
}

#endif	/* FL_COPY_H */

