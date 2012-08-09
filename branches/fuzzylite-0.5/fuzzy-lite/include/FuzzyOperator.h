/* 
 * File:   FuzzyOperator.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 3:05 PM
 */

#ifndef _FUZZYOPERATOR_H
#define	_FUZZYOPERATOR_H

#include "defs.h"
#include "flScalar.h"
#include "DiscreteTerm.h"

namespace fuzzy_lite {
    class DiscreteTerm;
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
            D_CENTROID = 0,
        };
    private:
        TNORM _tnorm;
        SNORM _snorm;
        CMODULATION _cmod;
        DEFUZZIFY _defuzzify;
        SNORM _aggregate;
    public:

        FuzzyOperator(TNORM tnorm = TN_MIN, SNORM snorm = SN_MAX, CMODULATION cmod = CM_CLIP,
                DEFUZZIFY defuzzify = D_CENTROID, SNORM aggregate = SN_MAX);
        virtual ~FuzzyOperator();

        virtual flScalar fuzzyAnd(flScalar mu1, flScalar mu2) const;
        virtual flScalar fuzzyOr(flScalar mu1, flScalar mu2) const;
        virtual flScalar modulate(flScalar degree, flScalar mu_consequent, flScalar mu_prob) const;
        virtual flScalar defuzzify(const DiscreteTerm & term) const;
        virtual flScalar aggregate(flScalar mu1, flScalar mu2) const;

        virtual void setTnorm(TNORM tnorm);
        virtual void setSnorm(SNORM snorm);
        virtual void setConsequentModulation(CMODULATION cmod);
        virtual void setDefuzzify(DEFUZZIFY defuzz);
        virtual void setAggregate(SNORM aggregate);

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
        static bool isEq(flScalar x1, flScalar x2);
        static bool isLEq(flScalar x1, flScalar x2);
        static bool isGEq(flScalar x1, flScalar x2);
        
        static flScalar scaleParma(flScalar src_min, flScalar src_max,
                flScalar value, flScalar target_min, flScalar target_max);

    };
}

#endif	/* _FUZZYOPERATOR_H */

