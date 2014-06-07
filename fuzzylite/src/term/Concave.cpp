// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

#include "fl/term/Concave.h"

namespace fl {

    Concave::Concave(const std::string& name, scalar inflection, scalar end)
    : Term(name), _inflection(inflection), _end(end) {

    }

    Concave::~Concave() {

    }

    std::string Concave::className() const {
        return "Concave";
    }

    std::string Concave::parameters() const {
        return Op::join(2, " ", _inflection, _end);
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
    }

    scalar Concave::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        if (fl::Op::isLE(_inflection, _end)) { //Concave increasing
            if (fl::Op::isLt(x, _end)) {
                return (_end - _inflection) / (2 * _end - _inflection - x);
            }
        } else { //Concave decreasing
            if (fl::Op::isGt(x, _end)) {
                return (_inflection - _end) / (_inflection - 2 * _end + x);
            }
        }
        return 1.0;
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
