/* 
 * File:   GaussianProduct.cpp
 * Author: jcrada
 * 
 * Created on 30 December 2012, 6:05 PM
 */

#include "fl/term/GaussianProduct.h"

namespace fl {

    GaussianProduct::GaussianProduct(const std::string& name,
            scalar meanA, scalar sigmaA, scalar meanB, scalar sigmaB)
    : Term(name), _meanA(meanA), _standardDeviationA(sigmaA), _meanB(meanB), _standardDeviationB(sigmaB) { }

    std::string GaussianProduct::className() const {
        return "GaussianProduct";
    }

    GaussianProduct* GaussianProduct::copy() const {
        return new GaussianProduct(*this);
    }

    scalar GaussianProduct::membership(scalar x) const {
        if (fl::Op::isNan(x)) return std::numeric_limits<scalar>::quiet_NaN();
        bool xLEa = fl::Op::isLE(x, _meanA);
        scalar a = std::exp((-(x - _meanA) * (x - _meanA)) / (2 * _standardDeviationA * _standardDeviationA))
                * xLEa + (1 - xLEa);
        bool xGEb = fl::Op::isGE(x, _meanB);
        scalar b = std::exp((-(x - _meanB) * (x - _meanB)) / (2 * _standardDeviationB * _standardDeviationB))
                * xGEb + (1 - xGEb);
        return a * b;
    }

    std::string GaussianProduct::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _meanA << ", " << _standardDeviationA << ", "
                << _meanB << ", " << _standardDeviationB << ")";
        return ss.str();
    }

    void GaussianProduct::setMeanA(scalar meanA) {
        this->_meanA = meanA;
    }

    scalar GaussianProduct::getMeanA() const {
        return this->_meanA;
    }

    void GaussianProduct::setStandardDeviationA(scalar sigmaA) {
        this->_standardDeviationA = sigmaA;
    }

    scalar GaussianProduct::getStandardDeviationA() const {
        return this->_standardDeviationA;
    }

    void GaussianProduct::setMeanB(scalar meanB) {
        this->_meanB = meanB;
    }

    scalar GaussianProduct::getMeanB() const {
        return this->_meanB;
    }

    void GaussianProduct::setStandardDeviationB(scalar sigmaB) {
        this->_standardDeviationB = sigmaB;
    }

    scalar GaussianProduct::getStandardDeviationB() const {
        return this->_standardDeviationB;
    }

}
