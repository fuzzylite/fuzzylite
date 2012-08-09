
#include "FuzzyOperator.h"
#include "StringOperator.h"

#include <limits>
#include <math.h>
namespace fuzzy_lite {

    FuzzyOperator::FuzzyOperator(TNORM tnorm, SNORM snorm, CMODULATION cmod, DEFUZZIFY defuzzify, SNORM aggregate)
    : _tnorm(tnorm), _snorm(snorm), _cmod(cmod), _defuzzify(defuzzify), _aggregate(aggregate) {

    }

    FuzzyOperator::~FuzzyOperator() {

    }

    flScalar FuzzyOperator::fuzzyAnd(flScalar mu1, flScalar mu2) const {
        return FuzzyOperator::FuzzyAnd(mu1, mu2, _tnorm);
    }

    flScalar FuzzyOperator::fuzzyOr(flScalar mu1, flScalar mu2) const {
        return FuzzyOperator::FuzzyOr(mu1, mu2, _snorm);
    }

    flScalar FuzzyOperator::modulate(flScalar degree, flScalar mu_consequent, flScalar mu_prob) const {
        //To use in each y-coord of a term.
        switch (_cmod) {
            case CM_CLIP: return FuzzyOperator::Min(degree, mu_consequent * mu_prob);
            case CM_SCALE: return FuzzyOperator::AlgebraicProduct(degree, mu_consequent * mu_prob);
            default:
                throw InvalidArgumentException(FL_AT, StringOperator::IntToString(_cmod) + " is not an option for modulation");
        }
    }

    flScalar FuzzyOperator::defuzzify(const DiscreteTerm& term) const {
        switch (_defuzzify) {
            case D_CENTROID:
            {
                //TODO: implementation and what to do with x and y? return x? or x*y? because y is the degree of membership...
                flScalar x, y;
                term.centroid(x, y);
                //                FL_DEBUG("defuzzy: x=" << x << "; y=" << y);
                return x;
            }
            default:
                throw InvalidArgumentException(FL_AT, StringOperator::IntToString(_defuzzify) + " is not an option for defuzzification");
        }
        return flScalar(-1.0);
    }

    flScalar FuzzyOperator::aggregate(flScalar mu1, flScalar mu2) const {
        return FuzzyOperator::FuzzyOr(mu1, mu2, _aggregate);
    }

    //static

    flScalar FuzzyOperator::FuzzyAnd(flScalar mu1, flScalar mu2, TNORM tnorm) {
        switch (tnorm) {
            case TN_MIN: return FuzzyOperator::Min(mu1, mu2);
            case TN_PROD: return FuzzyOperator::AlgebraicProduct(mu1, mu2);
            case TN_BDIFF: return FuzzyOperator::BoundedDiff(mu1, mu2);
            default:
                throw InvalidArgumentException(FL_AT, StringOperator::IntToString(tnorm) + " is not an option for T-Norm");
        }
    }

    flScalar FuzzyOperator::FuzzyOr(flScalar mu1, flScalar mu2, SNORM snorm) {
        switch (snorm) {
            case SN_MAX: return FuzzyOperator::Max(mu1, mu2);
            case SN_SUM: return FuzzyOperator::AlgebraicSum(mu1, mu2);
            case SN_BSUM: return FuzzyOperator::BoundedSum(mu1, mu2);
            default:
                throw InvalidArgumentException(FL_AT, StringOperator::IntToString(snorm) + " is not an option for S-Norm");
        }
    }

    //SETTERS

    void FuzzyOperator::setTnorm(TNORM tnorm) {
        this->_tnorm = tnorm;
    }

    void FuzzyOperator::setSnorm(SNORM snorm) {
        this->_snorm = snorm;
    }

    void FuzzyOperator::setConsequentModulation(CMODULATION cmod) {
        this->_cmod = cmod;
    }

    void FuzzyOperator::setDefuzzify(DEFUZZIFY defuzz) {
        this->_defuzzify = defuzz;
    }

    void FuzzyOperator::setAggregate(SNORM aggregate) {
        this->_aggregate = aggregate;
    }
    //GETTERS

    FuzzyOperator::TNORM FuzzyOperator::getTnorm() const {
        return this->_tnorm;
    }

    FuzzyOperator::SNORM FuzzyOperator::getSnorm() const {
        return this->_snorm;
    }

    FuzzyOperator::CMODULATION FuzzyOperator::getConsequentModulation() const {
        return this->_cmod;
    }

    FuzzyOperator::DEFUZZIFY FuzzyOperator::getDefuzzification() const {
        return this->_defuzzify;
    }

    FuzzyOperator::SNORM FuzzyOperator::getAggregation() const {
        return this->_aggregate;
    }

    //STATIC

    FuzzyOperator& FuzzyOperator::DefaultFuzzyOperator() {
        static FuzzyOperator* FUZZY_OP = new fuzzy_lite::FuzzyOperator;
        return *FUZZY_OP;
    }

    flScalar FuzzyOperator::Min(flScalar mu1, flScalar mu2) {
        return mu1 < mu2 ? mu1 : mu2;
    }

    flScalar FuzzyOperator::AlgebraicProduct(flScalar mu1, flScalar mu2) {
        return mu1 * mu2;
    }

    flScalar FuzzyOperator::BoundedDiff(flScalar mu1, flScalar mu2) {
        flScalar result = mu1 + mu2;
        return result > flScalar(0.0) ? result : flScalar(0.0);
    }

    //SNORMS

    flScalar FuzzyOperator::Max(flScalar mu1, flScalar mu2) {
        return mu1 > mu2 ? mu1 : mu2;
    }

    flScalar FuzzyOperator::AlgebraicSum(flScalar mu1, flScalar mu2) {
        return mu1 + mu2 - (mu1 * mu2);
    }

    flScalar FuzzyOperator::BoundedSum(flScalar mu1, flScalar mu2) {
        flScalar result = mu1 + mu2;
        return result < flScalar(1.0) ? result : flScalar(1.0);
    }

    //Other operators

    flScalar FuzzyOperator::Absolute(flScalar value) {
        return value < flScalar(0.0) ? value * flScalar(-1.0) : value;
    }

    bool FuzzyOperator::isEq(flScalar x1, flScalar x2) {
        //http://stackoverflow.com/questions/17333/most-effective-way-for-float-and-double-comparison
        //        return fabs(x1 - x2) < std::numeric_limits<flScalar>::epsilon() ;
#ifdef FL_USE_DOUBLE_PRECISION
        return fabs(x1 - x2) < 1e-14;
#else
        return fabs(x1 - x2) < 1e-5;
#endif


    }

    bool FuzzyOperator::isLEq(flScalar x1, flScalar x2) {
        return isEq(x1, x2) ? true : x1 < x2;
    }

    bool FuzzyOperator::isGEq(flScalar x1, flScalar x2) {
        return isEq(x1, x2) ? true : x1 > x2;
    }

    flScalar FuzzyOperator::scaleParma(flScalar src_min, flScalar src_max,
            flScalar value, flScalar target_min, flScalar target_max) {
        //Courtesy of Rubén Parma :)
        return (target_max - target_min) / (src_max - src_min) * (value - src_min) + target_min;
    }
}
