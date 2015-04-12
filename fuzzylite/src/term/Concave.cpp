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

#include "fl/term/Concave.h"

namespace fl {

    Concave::Concave(const std::string& name, scalar inflection, scalar end, scalar height)
    : Term(name, height), _inflection(inflection), _end(end) {

    }

    Concave::~Concave() {

    }

    std::string Concave::className() const {
        return "Concave";
    }

    scalar Concave::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        if (fl::Op::isLE(_inflection, _end)) { //Concave increasing
            if (fl::Op::isLt(x, _end)) {
                return _height * (_end - _inflection) / (2 * _end - _inflection - x);
            }
        } else { //Concave decreasing
            if (fl::Op::isGt(x, _end)) {
                return _height * (_inflection - _end) / (_inflection - 2 * _end + x);
            }
        }
        return _height * 1.0;
    }

    std::string Concave::parameters() const {
        return Op::join(2, " ", _inflection, _end) +
                (not Op::isEq(_height, 1.0) ? " " + Op::str(_height) : "");

    }

    void Concave::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setInflection(Op::toScalar(values.at(0)));
        setEnd(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));

    }

    void Concave::setInflection(scalar start) {
        this->_inflection = start;
    }

    scalar Concave::getInflection() const {
        return this->_inflection;
    }

    void Concave::setEnd(scalar end) {
        this->_end = end;
    }

    scalar Concave::getEnd() const {
        return this->_end;
    }

    Concave* Concave::clone() const {
        return new Concave(*this);
    }

    Term* Concave::constructor() {
        return new Concave;
    }





}
