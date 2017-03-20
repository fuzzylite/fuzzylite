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

#ifndef FL_GAUSSIANPRODUCT_H
#define FL_GAUSSIANPRODUCT_H

#include "fl/term/Term.h"

namespace fl {

    /**
      The GaussianProduct class is an extended Term that represents the
      two-sided %Gaussian membership function.

      @image html gaussianProduct.svg

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see Variable
      @since 4.0
     */
    class FL_API GaussianProduct : public Term {
    private:
        scalar _meanA;
        scalar _standardDeviationA;
        scalar _meanB;
        scalar _standardDeviationB;

    public:
        explicit GaussianProduct(const std::string& name = "",
                scalar meanA = fl::nan,
                scalar standardDeviationA = fl::nan,
                scalar meanB = fl::nan,
                scalar standardDeviationB = fl::nan,
                scalar height = 1.0);
        virtual ~GaussianProduct() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(GaussianProduct)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Provides the parameters of the term
          @return `"meanA standardDeviationA meanB standardDeviationB [height]"`
         */
        virtual std::string parameters() const FL_IOVERRIDE;
        /**
          Configures the term with the parameters
          @param parameters as `"meanA standardDeviationA meanB
          standardDeviationB [height]"`
         */
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the membership function evaluated at @f$x@f$
          @param x
          @return @f$ h \left((1 - i) + i \times \exp(-(x - \mu_a)^2 /
          (2\sigma_a^2))\right)
          \left((1 - j) + j \times \exp(-(x - \mu_b)^2 / (2 \sigma_b)^2)\right)
          @f$

          where @f$h@f$ is the height of the Term,
                @f$\mu_a@f$ is the mean of the first GaussianProduct,
                @f$\sigma_a@f$ is the standard deviation of the first
                GaussianProduct,
                @f$\mu_b@f$ is the mean of the second GaussianProduct,
                @f$\sigma_b@f$ is the standard deviation of the second
                GaussianProduct,
                @f$i=\begin{cases}1 & \mbox{if $x \leq \mu_a$} \cr 0
                &\mbox{otherwise}\end{cases}@f$,
                @f$j=\begin{cases}1 & \mbox{if $x \geq \mu_b$} \cr 0
                &\mbox{otherwise}\end{cases}@f$
         */
        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        /**
          Sets the mean of the first %Gaussian curve
          @param meanA is the mean of the first %Gaussian curve
         */
        virtual void setMeanA(scalar meanA);
        /**
          Gets the mean of the first %Gaussian curve
          @return the mean of the first %Gaussian curve
         */
        virtual scalar getMeanA() const;

        /**
          Sets the standard deviation of the first %Gaussian curve
          @param standardDeviationA is the standard deviation of the first %Gaussian curve
         */
        virtual void setStandardDeviationA(scalar standardDeviationA);
        /**
          Gets the standard deviation of the first %Gaussian curve
          @return the standard deviation of the first %Gaussian curve
         */
        virtual scalar getStandardDeviationA() const;

        /**
          Sets the mean of the second %Gaussian curve
          @param meanB is the mean of the second %Gaussian curve
         */
        virtual void setMeanB(scalar meanB);
        /**
          Gets the mean of the second %Gaussian curve
          @return the mean of the second %Gaussian curve
         */
        virtual scalar getMeanB() const;

        /**
          Sets the standard deviation of the second %Gaussian curve
          @param standardDeviationB is the standard deviation of the second %Gaussian curve
         */
        virtual void setStandardDeviationB(scalar standardDeviationB);
        /**
          Gets the standard deviation of the second %Gaussian curve
          @return the standard deviation of the second %Gaussian curve
         */
        virtual scalar getStandardDeviationB() const;

        virtual GaussianProduct* clone() const FL_IOVERRIDE;

        static Term* constructor();
    };
}
#endif  /* FL_GAUSSIANPRODUCT_H */

