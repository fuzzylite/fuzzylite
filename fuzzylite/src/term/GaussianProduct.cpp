/* 
 * File:   GaussianProduct.cpp
 * Author: jcrada
 * 
 * Created on 30 December 2012, 6:05 PM
 */

#include "fl/term/GaussianProduct.h"

#include "fl/operator/Operator.h"

namespace fl {

    GaussianProduct::GaussianProduct(const std::string& name,
            scalar meanA, scalar sigmaA, scalar meanB, scalar sigmaB)
    : _meanA(meanA), _sigmaA(sigmaA), _meanB(meanB), _sigmaB(sigmaB) { }

    std::string GaussianProduct::className() const {
        return "GaussianProduct";
    }

    GaussianProduct* GaussianProduct::copy() const {
        return new GaussianProduct(*this);
    }

    scalar GaussianProduct::membership(scalar x) const {
        bool xLEa = fl::Op::IsLE(x, _meanA);
        scalar a = std::exp((-(x - _meanA) * (x - _meanA)) / (2 * _sigmaA * _sigmaA))
                * xLEa + (1 - xLEa);
        bool xGEb = fl::Op::IsGE(x, _meanB);
        scalar b = std::exp((-(x - _meanB) * (x - _meanB)) / (2 * _sigmaB * _sigmaB))
                * xGEb + (1 - xGEb);
        return a * b;
    }

    std::string GaussianProduct::toString() const {
        std::ostringstream ss;
        ss << className() << " (" << _meanA << ", " << _sigmaA << ", "
                << _meanB << ", " << _sigmaB << ")";
        return ss.str();
    }

    void GaussianProduct::setMeanA(scalar meanA) {
        this->_meanA = meanA;
    }

    scalar GaussianProduct::getMeanA() const {
        return this->_meanA;
    }

    void GaussianProduct::setSigmaA(scalar sigmaA) {
        this->_sigmaA = sigmaA;
    }

    scalar GaussianProduct::getSigmaA() const {
        return this->_sigmaA;
    }

    void GaussianProduct::setMeanB(scalar meanB) {
        this->_meanB = meanB;
    }

    scalar GaussianProduct::getMeanB() const {
        return this->_meanB;
    }

    void GaussianProduct::setSigmaB(scalar sigmaB) {
        this->_sigmaB = sigmaB;
    }

    scalar GaussianProduct::getSigmaB() const {
        return this->_sigmaB;
    }

}
