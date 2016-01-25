/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.
 */

#ifndef FL_PROBABILISTIC_H
#define FL_PROBABILISTIC_H

#include "fl/activation/Activation.h"

namespace fl {

    /**
      
      The Probabilistic class is a RuleBlock Activation method that activates
      the rules utilizing the weights of the rules as independent probabilities 
      of activation
      
      @author Juan Rada-Vilela, Ph.D.
      @see Rule
      @see RuleBlock
      @see ActivationFactory
      @since 6.0
    
     */
    class FL_API Probabilistic : public Activation {
    public:
        Probabilistic();
        virtual ~Probabilistic();
        FL_DEFAULT_COPY_AND_MOVE(Probabilistic)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          No parameters are required to configure the activation method
          
          @return an empty string
         */
        virtual std::string parameters() const FL_IOVERRIDE;

        /**
          No parameters are required to configure the activation method
          
          @param parameters is an empty string
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        /**
          Activates the rules utilizing their weights as independent 
          probabilities of activation.
          
          @param ruleBlock is the rule block to activate.
         */
        virtual void activate(RuleBlock* ruleBlock) const FL_IOVERRIDE;

        virtual Probabilistic* clone() const FL_IOVERRIDE;

        static Activation* constructor();
    };
}

#endif /* FL_PROBABILISTIC_H */
