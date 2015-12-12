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

#ifndef FL_FIRST_H
#define FL_FIRST_H

#include "fl/activation/Activation.h"

namespace fl {

    /**
     
      The First class is a RuleBlock Activation method that activates the first
      rule whose activation degree is greater than the given threshold, and
      deactivates the remaining rules. The rules are iterated in the order they
      were added to the rule block. @todo add threshold value
   
      @author Juan Rada-Vilela, Ph.D.
      @see Last
      @see Rule
      @see RuleBlock
      @see ActivationFactory
      @since 6.0

     */

    class FL_API First : public Activation {
    public:

        First();
        virtual ~First();
        FL_DEFAULT_COPY_AND_MOVE(First)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          Returns the threshold of the activation method.
          
          @return the threshold of the activation method
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          Configures the activation method with the given threshold.
          
          @param parameters contains the threshold of the activation method
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
          Activates the first rule whose activation degree is greater than the
          given threshold, and deactivates the remaining rules. The rules are
          iterated in the order the rules were added to the rule block.
          
          @param ruleBlock is the rule block to activate
         */
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual First* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };

}


#endif /* FL_FIRST_H */
