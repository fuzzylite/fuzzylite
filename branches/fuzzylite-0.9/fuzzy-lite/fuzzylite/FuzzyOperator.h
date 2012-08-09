/*   Copyright 2010 Juan Rada-Vilela

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
/* 
 * File:   FuzzyOperator.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 3:05 PM
 */

#ifndef _FUZZYOPERATOR_H
#define	_FUZZYOPERATOR_H

#include <fuzzylite/defs.h>
#include <fuzzylite/flScalar.h>
#include <fuzzylite/LinguisticTerm.h>

namespace fl {
    class LinguisticTerm;

    class FuzzyOperator {
    public:

        enum TNORM {
            TN_MIN = 0, TN_PROD, TN_BDIFF,
        };

        enum SNORM {
            SN_MAX = 0, SN_SUM, SN_BSUM,
        };

        enum CMODULATION { //Consequent Modulation
            CM_CLIP = 0, CM_SCALE,
        };

        enum DEFUZZIFY {
            D_COG = 0,
        };
    private:
        TNORM _tnorm;
        SNORM _snorm;
        CMODULATION _cmod;
        DEFUZZIFY _defuzzify;
        SNORM _accumulate;
        int _samples;
    public:

        FuzzyOperator(TNORM tnorm = TN_MIN, SNORM snorm = SN_MAX, CMODULATION cmod =
                CM_CLIP, DEFUZZIFY defuzzify = D_COG, SNORM accumulate = SN_MAX,
                int samples = FL_SAMPLE_SIZE);
        virtual ~FuzzyOperator();

        virtual flScalar fuzzyAnd(flScalar mu1, flScalar mu2) const;
        virtual flScalar fuzzyOr(flScalar mu1, flScalar mu2) const;
        virtual flScalar modulate(flScalar modulation, flScalar mu) const;
        virtual flScalar defuzzify(const LinguisticTerm& term) const;
        virtual flScalar accumulate(flScalar mu1, flScalar mu2) const;
        virtual int samples() const;

        virtual void setTnorm(TNORM tnorm);
        virtual void setSnorm(SNORM snorm);
        virtual void setConsequentModulation(CMODULATION cmod);
        virtual void setDefuzzify(DEFUZZIFY defuzz);
        virtual void setAggregate(SNORM aggregate);
        virtual void setSamples(int samples);

        virtual TNORM getTnorm() const;
        virtual SNORM getSnorm() const;
        virtual CMODULATION getConsequentModulation() const;
        virtual DEFUZZIFY getDefuzzification() const;
        virtual SNORM getAggregation() const;

        static FuzzyOperator & DefaultFuzzyOperator();

        static flScalar FuzzyAnd(flScalar mu1, flScalar mu2, TNORM tnorm);
        static flScalar FuzzyOr(flScalar mu1, flScalar mu2, SNORM snorm);

        static flScalar Min(flScalar mu1, flScalar mu2);
        static flScalar AlgebraicProduct(flScalar mu1, flScalar mu2);

        static flScalar BoundedDiff(flScalar mu1, flScalar mu2);
        //SNORMS

        static flScalar Max(flScalar mu1, flScalar mu2);
        static flScalar AlgebraicSum(flScalar mu1, flScalar mu2);
        static flScalar BoundedSum(flScalar mu1, flScalar mu2);

        //Other operators

        static flScalar Absolute(flScalar value);
        static bool IsEq(flScalar x1, flScalar x2);
        static bool IsLEq(flScalar x1, flScalar x2);
        static bool IsGEq(flScalar x1, flScalar x2);

        static flScalar Scale(flScalar src_min, flScalar src_max, flScalar value,
                flScalar target_min, flScalar target_max);

        static flScalar Area(const LinguisticTerm& term, int samples = FL_SAMPLE_SIZE);
        static void Centroid(const LinguisticTerm& term, flScalar& centroid_x, flScalar& centroid_y,
                int samples = FL_SAMPLE_SIZE);
        static flScalar AreaAndCentroid(const LinguisticTerm& term, flScalar& centroid_x,
                flScalar& centroid_y, int samples = FL_SAMPLE_SIZE);

        static void ColorGradientF(flScalar from_r, flScalar from_g, flScalar from_b, flScalar from_a,
                flScalar to_r, flScalar to_g, flScalar to_b, flScalar to_a, flScalar degree,
                flScalar& red, flScalar& green, flScalar& blue, flScalar& alpha);

        static void ColorGradient(int from_r, int from_g, int from_b, int from_a,
                int to_r, int to_g, int to_b, int to_a, flScalar degree,
                int& red, int& green, int& blue, int& alpha);

        static void main(int argc, char** argv);

    };
}

#endif	/* _FUZZYOPERATOR_H */

