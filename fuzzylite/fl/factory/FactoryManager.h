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

#ifndef FL_FACTORYMANAGER_H
#define FL_FACTORYMANAGER_H

#include "fl/fuzzylite.h"

#include "fl/factory/TNormFactory.h"
#include "fl/factory/SNormFactory.h"
#include "fl/factory/ActivationFactory.h"
#include "fl/factory/DefuzzifierFactory.h"
#include "fl/factory/TermFactory.h"
#include "fl/factory/HedgeFactory.h"
#include "fl/factory/FunctionFactory.h"

namespace fl {

    /**
      The FactoryManager class is a central class grouping different factories
      of objects, together with a singleton instance to access each of the
      factories throughout the library.


      @author Juan Rada-Vilela, Ph.D.
      @see TermFactory
      @see TNormFactory
      @see SNormFactory
      @see HedgeFactory
      @see ActivationFactory
      @see DefuzzifierFactory
      @see FunctionFactory
      @since 4.0
     */
    class FL_API FactoryManager {
    private:
        FL_unique_ptr<TNormFactory> _tnorm;
        FL_unique_ptr<SNormFactory> _snorm;
        FL_unique_ptr<ActivationFactory> _activation;
        FL_unique_ptr<DefuzzifierFactory> _defuzzifier;
        FL_unique_ptr<TermFactory> _term;
        FL_unique_ptr<HedgeFactory> _hedge;
        FL_unique_ptr<FunctionFactory> _function;

    public:
        FactoryManager();
        explicit FactoryManager(TNormFactory* tnorm, SNormFactory* snorm,
                ActivationFactory* activation, DefuzzifierFactory* defuzzifier,
                TermFactory* term, HedgeFactory* hedge, FunctionFactory* function);
        explicit FactoryManager(const FactoryManager& other);
        FactoryManager& operator=(const FactoryManager& other);
        FL_DEFAULT_MOVE(FactoryManager)
        virtual ~FactoryManager();

        /**
          Gets the static instance of the manager
          @return the static instance of the manager
         */
        static FactoryManager* instance();

        /**
          Sets the factory of TNorm%s
          @param tnorm is the factory of TNorm%s
         */
        virtual void setTnorm(TNormFactory* tnorm);
        /**
          Gets the factory of TNorm%s
          @return the factory of TNorm%s
         */
        virtual TNormFactory* tnorm() const;

        /**
          Sets the factory of SNorm%s
          @param snorm is the factory of SNorm%s
         */
        virtual void setSnorm(SNormFactory* snorm);
        /**
          Gets the factory of SNorm%s
          @return the factory of SNorm%s
         */
        virtual SNormFactory* snorm() const;

        /**
          Sets the factory of Activation methods
          @param activation is the factory of Activation methods
         */
        virtual void setActivation(ActivationFactory* activation);
        /**
          Gets the factory of Activation methods
          @return the factory of Activation methods
         */
        virtual ActivationFactory* activation() const;

        /**
          Sets the factory of Defuzzifier%s
          @param defuzzifier is the factory of Defuzzifier%s
         */
        virtual void setDefuzzifier(DefuzzifierFactory* defuzzifier);
        /**
          Gets the factory of Defuzzifier%s
          @return the factory of Defuzzifier%s
         */
        virtual DefuzzifierFactory* defuzzifier() const;

        /**
          Sets the factory of Term%s
          @param term is the factory of Term%s
         */
        virtual void setTerm(TermFactory* term);
        /**
          Gets the factory of Term%s
          @return the factory of Term%s
         */
        virtual TermFactory* term() const;

        /**
          Sets the factory of Hedge%s
          @param hedge is the factory of Hedge%s
         */
        virtual void setHedge(HedgeFactory* hedge);
        /**
          Gets the factory of Hedge%s
          @return the factory of Hedge%s
         */
        virtual HedgeFactory* hedge() const;

        /**
          Sets the factory of Function Element%s
          @param function is the factory of Function Element%s
         */
        virtual void setFunction(FunctionFactory* function);
        /**
          Gets the factory of Function Element%s
          @return the factory of Function Element%s
         */
        virtual FunctionFactory* function() const;
    };
}

#endif  /* FL_FACTORYMANAGER_H */

