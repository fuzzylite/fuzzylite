/*   Copyright 2013 Juan Rada-Vilela

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
 * File:   GaussianProduct.cpp
 * Author: jcrada
 * 
 * Created on 30 December 2012, 6:05 PM
 */

#include "fl/term/GaussianProduct.h"

namespace fl {

    GaussianProduct::GaussianProduct(const std::string& name,
            scalar meanA, scalar standardDeviationA, scalar meanB, scalar standardDeviationB)
    : Term(name), _meanA(meanA), _standardDeviationA(standardDeviationA),
    _meanB(meanB), _standardDeviationB(standardDeviationB) {
    }

    GaussianProduct::~GaussianProduct() {
    }

    std::string GaussianProduct::className() const {
        return "GaussianProduct";
    }

    GaussianProduct* GaussianProduct::copy() const {
        return new GaussianProduct(*this);
    }

    scalar GaussianProduct::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
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
        ss << className() << " ("
                << fl::Op::str(_meanA) << ", "
                << fl::Op::str(_standardDeviationA) << ", "
                << fl::Op::str(_meanB) << ", "
                << fl::Op::str(_standardDeviationB) << ")";
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

     void GaussianProduct::configure(const std::vector<scalar>& parameters){
         if (parameters.size() < 4){
             std::ostringstream ex;
             ex << "[configuration error] term <" << className() << ">"
                     << " requires <" << 4 << "> parameters";
             throw fl::Exception(ex.str(), FL_AT);
         }
         setMeanA(parameters.at(0));
         setStandardDeviationA(parameters.at(1));
         setMeanB(parameters.at(2));
         setStandardDeviationB(parameters.at(3));
     }
    
    Term* GaussianProduct::constructor() {
        return new GaussianProduct;
    }

}
