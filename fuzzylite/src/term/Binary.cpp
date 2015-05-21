/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/term/Binary.h"
#include "fl/Operation.h"

namespace fl {

    Binary::Binary(const std::string& name, scalar threshold, scalar height)
    : Term(name, height), _threshold(threshold) {
    }

    Binary::~Binary() {
    }

    std::string Binary::className() const {
        return "Binary";
    }

    scalar Binary::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        if (fl::Op::isLt(x, _threshold)) {
            return _height * 0.0;
        }
        return _height * 1.0;
    }

    std::string Binary::parameters() const {
        return Op::join(1, " ", _threshold) +
                (not Op::isEq(_height, 1.0) ? " " + Op::str(_height) : "");
    }

    void Binary::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 1;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setThreshold(Op::toScalar(values.at(0)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Binary::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }
    
    scalar Binary::getThreshold() const {
        return this->_threshold;
    }


    Binary* Binary::clone() const {
        return new Binary(*this);
    }

    Term* Binary::constructor() {
        return new Binary;
    }

}

