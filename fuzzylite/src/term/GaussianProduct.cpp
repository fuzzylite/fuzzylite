/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/term/GaussianProduct.h"

namespace fl {

    GaussianProduct::GaussianProduct(const std::string& name,
            scalar meanA, scalar standardDeviationA, scalar meanB, scalar standardDeviationB,
            scalar height)
    : Term(name, height), _meanA(meanA), _standardDeviationA(standardDeviationA),
    _meanB(meanB), _standardDeviationB(standardDeviationB) {
    }

    GaussianProduct::~GaussianProduct() {
    }

    std::string GaussianProduct::className() const {
        return "GaussianProduct";
    }

    scalar GaussianProduct::membership(scalar x) const {
        if (FL_IS_NAN(x)) return fl::nan;
        bool xLEa = fl::Op::isLE(x, _meanA);
        scalar a = (1 - xLEa) + xLEa * std::exp(
                (-(x - _meanA) * (x - _meanA)) / (2 * _standardDeviationA * _standardDeviationA)
                );
        bool xGEb = fl::Op::isGE(x, _meanB);
        scalar b = (1 - xGEb) + xGEb * std::exp(
                (-(x - _meanB) * (x - _meanB)) / (2 * _standardDeviationB * _standardDeviationB)
                );
        return Term::_height * a * b;
    }

    std::string GaussianProduct::parameters() const {
        return Op::join(4, " ", _meanA, _standardDeviationA, _meanB, _standardDeviationB) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void GaussianProduct::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 4;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setMeanA(Op::toScalar(values.at(0)));
        setStandardDeviationA(Op::toScalar(values.at(1)));
        setMeanB(Op::toScalar(values.at(2)));
        setStandardDeviationB(Op::toScalar(values.at(3)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
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

    GaussianProduct* GaussianProduct::clone() const {
        return new GaussianProduct(*this);
    }

    Term* GaussianProduct::constructor() {
        return new GaussianProduct;
    }


}
