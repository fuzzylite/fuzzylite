/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.
 */

#ifndef FL_LAST_H
#define FL_LAST_H


#include "fl/activation/Activation.h"

namespace fl {

    /**
    
     The Last class is a RuleBlock Activation method that activates the last
     rule whose activation degree is greater than the given threshold, and
     deactivates the remaining rules. The rules are iterated in the reverse
     order in which they were added to the rule block.
    
     @todo Revise if natural order is a better option.
     @todo add threshold value
      
     @author Juan Rada-Vilela, Ph.D.
     @see First
     @see Rule
     @see RuleBlock
     @see ActivationFactory
     @since 6.0
     
     */

    class FL_API Last : public Activation {
    public:

        Last();
        virtual ~Last();
        FL_DEFAULT_COPY_AND_MOVE(Last)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          No parameters are required to configure the activation method.
          
          @return empty string
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          No parameters are required to configure the activation method.
          
          @param parameters is an empty string
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
          Activates the last rule whose activation degree is greater than the
          given threshold, and deactivates the remaining rules. The rules are
          iterated in the reverse order that the rules were added to the rule
          block.
          
          @param ruleBlock is the rule block to activate
         */
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual Last* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };

}


#endif /* FL_LAST_H */
