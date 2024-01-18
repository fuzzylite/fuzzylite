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

#ifndef FL_VARIABLE_H
#define FL_VARIABLE_H

#include "fl/fuzzylite.h"
#include "fl/defuzzifier/Centroid.h"

#include <string>
#include <vector>

namespace fl {

    class Term;

    /**
      The Variable class is the base class for linguistic variables.

      @author Juan Rada-Vilela, Ph.D.
      @see InputVariable
      @see OutputVariable
      @see Term
      @since 4.0
     */
    class FL_API Variable {
    public:

        /**
         Indicates the type of the variable to avoid costly `dynamic_casts`
         */
        enum Type {
            None,
            Input,
            Output
        };
    protected:
        std::string _name;
        std::string _description;
        std::vector<Term*> _terms;
        scalar _value;
        scalar _minimum, _maximum;
        bool _enabled;
        bool _lockValueInRange;

    private:
        void copyFrom(const Variable& source);

    public:
        explicit Variable(const std::string& name = "",
                scalar minimum = -fl::inf,
                scalar maximum = fl::inf);
        explicit Variable(const Variable& other);
        Variable& operator=(const Variable& other);
        virtual ~Variable();
        FL_DEFAULT_MOVE(Variable)

        /**
          Sets the name of the variable
          @param name is the name of the variable
         */
        virtual void setName(const std::string& name);

        /**
          Gets the name of the variable
          @return the name of the variable
         */
        virtual std::string getName() const;

        /**
          Gets the description of the variable
          @return the description of the variable
         */
        virtual std::string getDescription() const;

        /**
          Sets the description of the variable
          @param description is the description of the variable
         */
        virtual void setDescription(const std::string& description);

        /**
          Sets the value of the variable
          @param value is the input value of an InputVariable, or the output
          value of an OutputVariable
         */
        virtual void setValue(scalar value);

        /**
          Gets the value of the variable
          @return the input value of an InputVariable, or the output value of
          an OutputVariable
         */
        virtual scalar getValue() const;

        /**
          Sets the range of the variable between `[minimum, maximum]`
          @param minimum is the minimum value in range
          @param maximum is the maximum value in range
         */
        virtual void setRange(scalar minimum, scalar maximum);
        /**
          Gets the magnitude of the range of the variable
          @return `maximum - minimum`
         */
        virtual scalar range() const;

        /**
          Sets the minimum value of the range of the variable
          @param minimum is the minimum value of the range
         */
        virtual void setMinimum(scalar minimum);
        /**
          Gets the minimum value of the range of the variable
          @return the minimum value of the range of the variable
         */
        virtual scalar getMinimum() const;

        /**
          Sets the maximum value of the range of the variable
          @param maximum is the maximum value of the range
         */
        virtual void setMaximum(scalar maximum);
        /**
          Gets the maximum value of the range of the variable
          @return the maximum value of the range of the variable
         */
        virtual scalar getMaximum() const;

        /**
          Sets whether the variable is enabled
          @param enabled determines whether to enable the variable
         */
        virtual void setEnabled(bool enabled);
        /**
          Gets whether the variable is enabled
          @return whether the variable is enabled
         */
        virtual bool isEnabled() const;

        /**
          Sets whether the variable locks the current value to the range of the
          variable.

          If enabled in an InputVariable @f$i@f$, the input value @f$x_i@f$
          will be used when computing the Antecedent::activationDegree() as
          long as @f$x_i \in [\mbox{min}, \mbox{max}]@f$. Else, for the case of
          @f$x_i \not\in [\mbox{min}, \mbox{max}]@f$, the range values will be
          used instead but without changing the input value @f$x_i@f$.

          If enabled in an OutputVariable @f$j@f$, the output value @f$y_j@f$
          will be overriden by the range values when @f$y_j \not\in
          [\mbox{min}, \mbox{max}]@f$. See OutputVariable for more information.

          @param lockValueInRange indicates whether to lock the value to the
          range of the variable
         */
        virtual void setLockValueInRange(bool lockValueInRange);

        /**
          Gets whether the variable locks the current value to the
          range of the variable

          If enabled in an InputVariable @f$i@f$, the input value @f$x_i@f$
          will be used when computing the Antecedent::activationDegree() as
          long as @f$x_i \in [\mbox{min}, \mbox{max}]@f$. Else, for the case of
          @f$x_i \not\in [\mbox{min}, \mbox{max}]@f$, the range values will be
          used instead but without changing the input value @f$x_i@f$.

          If enabled in an OutputVariable @f$j@f$, the output value @f$y_j@f$
          will be overriden by the range values when @f$y_j \not\in
          [\mbox{min}, \mbox{max}]@f$. See OutputVariable for more information.

          @return whether the variable locks the current value to the range of
          the variable
         */
        virtual bool isLockValueInRange() const;

        /**
          Computes the aggregated complexity of the underlying terms
          @return the aggregated complexity of the underlying terms
         */
        virtual Complexity complexity() const;

        /**
          Evaluates the membership function of value @f$x@f$ for each
          term @f$i@f$, resulting in a fuzzy value in the form
          @f$\tilde{x}=\sum_i{\mu_i(x)/i}@f$
          @param x is the value to fuzzify
          @return the fuzzy value expressed as @f$\sum_i{\mu_i(x)/i}@f$
         */
        virtual std::string fuzzify(scalar x) const;

        /**
          Gets the term which has the highest membership function value for
          @f$x@f$.
          @param x is the value of interest
          @param[out] yhighest is a pointer where the highest membership
          function value will be stored
          @return the term @f$i@f$ which maximimizes @f$\mu_i(x)@f$
         */
        virtual Term* highestMembership(scalar x, scalar* yhighest = fl::null) const;

        /**
         Returns the type of the variable
         @return the type of the variable
         */
        virtual Type type() const;

        /**
          Gets a string representation of the variable in the FuzzyLite Language
          @return a string representation of the variable in the FuzzyLite
          Language
          @see FllExporter
         */
        virtual std::string toString() const;

        /**
          Sorts the terms in ascending order according to their centroids
         */
        virtual void sort();
        /**
          Adds a term to the variable
          @param term is the term to add
         */
        virtual void addTerm(Term* term);
        /**
          Inserts the term in the variable
          @param term is the term to insert
          @param index is the index where the term will be inserted
         */
        virtual void insertTerm(Term* term, std::size_t index);
        /**
          Gets the term at the given index
          @param index is the position of the term in the vector
          @return the term at the given index
         */
        virtual Term* getTerm(std::size_t index) const;
        /**
          Gets the term of the given name.
          @param name is the name of the term to retrieve
          @return the term of the given name
          @throws Exception if the term is not found
         */
        virtual Term* getTerm(const std::string& name) const;
        /**
          Gets whether a term of the given name has been added
          @param name the name of the term
          @return whether the term of the given name is found
         */
        virtual bool hasTerm(const std::string& name) const;
        /**
          Removes the term at the given index
          @param index the index of the term to remove
          @return the removed term
         */
        virtual Term* removeTerm(std::size_t index);
        /**
          Gets the number of terms added to the variable
          @return the number of terms in the variable
         */
        virtual std::size_t numberOfTerms() const;
        /**
          Sets the terms of the variable
          @param terms is a vector of terms
         */
        virtual void setTerms(const std::vector<Term*>& terms);
        /**
          Gets an immutable vector of the terms
          @return an immutable vector of terms
         */
        virtual const std::vector<Term*>& terms() const;
        /**
          Gets a mutable vector of the terms
          @return a mutable vector of terms
         */
        virtual std::vector<Term*>& terms();
        /**
          Creates a clone of the variable
          @return a clone of the variable
         */
        virtual Variable* clone() const;
    };
}
#endif /* FL_VARIABLE_H */
