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

#ifndef FL_AGGREGATED_H
#define FL_AGGREGATED_H

#include "fl/term/Term.h"

#include "fl/term/Activated.h"

#include <vector>

namespace fl {

    class SNorm;
    class TNorm;

    /**
      The Aggregated class is a special Term that stores a fuzzy set with the
      Activated terms from the Antecedent%s of a Rule, thereby serving mainly
      as the fuzzy output value of the OutputVariable%s. The ownership of the
      activated terms will be transfered to objects of this class, and
      therefore their destructors will be called upon destruction of this term
      (or calling Aggregated::clear()).

      @author Juan Rada-Vilela, Ph.D.
      @see Antecedent
      @see Rule
      @see OutputVariable
      @see Activated
      @see Term
      @since 6.0
     */
    class FL_API Aggregated : public Term {
    private:
        std::vector<Activated> _terms;
        scalar _minimum, _maximum;
        FL_unique_ptr<SNorm> _aggregation;

        void copyFrom(const Aggregated& source);
    public:
        explicit Aggregated(const std::string& name = "",
                scalar minimum = fl::nan,
                scalar maximum = fl::nan,
                SNorm* aggregation = fl::null);
        Aggregated(const Aggregated& other);
        Aggregated& operator=(const Aggregated& other);
        virtual ~Aggregated() FL_IOVERRIDE;
        FL_DEFAULT_MOVE(Aggregated)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Returns the parameters of the term
          @return `"aggregation minimum maximum terms"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Does nothing
          @param parameters are irrelevant
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Aggregated* clone() const FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;
        virtual Complexity complexityOfMembership() const;
        virtual Complexity complexityOfActivationDegree() const;

        /**
          Aggregates the membership function values of @f$x@f$ utilizing the
          aggregation operator
          @param x is a value
          @return @f$\sum_i{\mu_i(x)}, i \in \mbox{terms}@f$
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;
        /**
          Computes the aggregated activation degree for the given term.
          If the same term is present multiple times, the aggregation operator
          is utilized to sum the activation degrees of the term. If the
          aggregation operator is fl::null, a regular sum is performed.
          @param forTerm is the term for which to compute the aggregated
          activation degree
          @return the aggregated activation degree for the given term
         */
        virtual scalar activationDegree(const Term* forTerm) const;

        /**
          Iterates over the Activated terms to find the term with the maximum
          activation degree
          @return the term with the maximum activation degree
         */
        virtual const Activated* highestActivatedTerm() const;

        virtual std::string toString() const FL_IOVERRIDE;

        /**
          Sets the minimum of the range of the fuzzy set
          @param minimum is the minimum of the range of the fuzzy set
         */
        virtual void setMinimum(scalar minimum);
        /**
          Gets the minimum of the range of the fuzzy set
          @return the minimum of the range of the fuzzy set
         */
        virtual scalar getMinimum() const;

        /**
          Sets the maximum of the range of the fuzzy set
          @param maximum is the maximum of the range of the fuzzy set
         */
        virtual void setMaximum(scalar maximum);
        /**
          Gets the maximum of the range of the fuzzy set
          @return the maximum of the range of the fuzzy set
         */
        virtual scalar getMaximum() const;

        /**
          Sets the range of the fuzzy set to `[minimum, maximum]`
          @param minimum is the minimum of the range of the fuzzy set
          @param maximum is the maximum of the range of the fuzzy set
         */
        virtual void setRange(scalar minimum, scalar maximum);
        /**
          Returns the magnitude of the range of the fuzzy set,
          @return the magnitude of the range of the fuzzy set,
          i.e., `maximum - minimum`
         */
        virtual scalar range() const;

        /**
          Sets the aggregation operator
          @param aggregation is the aggregation operator
         */
        virtual void setAggregation(SNorm* aggregation);
        /**
          Gets the aggregation operator
          @return the aggregation operator
         */
        virtual SNorm* getAggregation() const;

        /**
          Adds a new Activated term (from the parameters) to the fuzzy set
          @param term is the activated term
          @param degree is the activation degree
          @param implication is the implication operator
         */
        virtual void addTerm(const Term* term, scalar degree, const TNorm* implication);
        /**
          Adds the activated term to the fuzzy set. The activated term
          will be deleted when Aggregated::clear()
          @param term is the activated term
         */
        virtual void addTerm(const Activated& term);
        /**
          Gets the term at the given index
          @param index is the index of the term
          @return the activated term at the given index
         */
        virtual const Activated& getTerm(std::size_t index) const;
        /**
          Removes the term at the given index without deleting the term
          @param index is the index of the term
          @return the removed term
         */
        virtual const Activated& removeTerm(std::size_t index);
        /**
          Returns the number of activated terms
          @return the number of activated terms
         */
        virtual std::size_t numberOfTerms() const;

        /**
          Sets the activated terms
          @param terms is the activated terms
         */
        virtual void setTerms(const std::vector<Activated>& terms);
        /**
          Returns an immutable vector of activated terms
          @return an immutable vector of activated terms
         */
        virtual const std::vector<Activated>& terms() const;
        /**
          Returns a mutable vector of activated terms
          @return a mutable vector of activated terms
         */
        virtual std::vector<Activated>& terms();
        /**
          Indicates whether the vector of activated terms is empty
          @return whether the vector of activated terms is empty
         */
        virtual bool isEmpty() const;
        /**
          Clears and deletes the activated terms
         */
        virtual void clear();
    };
}
#endif /* FL_AGGREGATED_H */
