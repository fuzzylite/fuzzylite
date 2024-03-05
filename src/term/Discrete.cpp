/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/term/Discrete.h"

namespace fuzzylite {

    Discrete::Discrete(const std::string& name, const std::vector<Pair>& xy, scalar height) :
        Term(name, height),
        _xy(xy) {}

    Discrete::~Discrete() {}

    std::string Discrete::className() const {
        return "Discrete";
    }

    bool compare(const Discrete::Pair& a, const Discrete::Pair& b) {
        return a.first < b.first;
    }

    void Discrete::sort(std::vector<Pair>& pairs) {
        std::sort(pairs.begin(), pairs.end(), compare);
    }

    void Discrete::sort() {
        std::sort(_xy.begin(), _xy.end(), compare);
    }

    scalar Discrete::membership(scalar x) const {
        if (Op::isNaN(x))
            return fl::nan;
        if (_xy.empty())
            throw Exception("[discrete error] term is empty", FL_AT);

        /*                ______________________
                         /                      \
                        /                        \
           ____________/                          \____________
                      x[0]                      x[n-1]
         */

        if (Op::isLE(x, _xy.front().first))
            return Term::_height * _xy.front().second;
        if (Op::isGE(x, _xy.back().first))
            return Term::_height * _xy.back().second;

        const Pair value(x, fl::nan);
        typedef std::vector<Discrete::Pair>::const_iterator Bound;
        // std::lower_bound finds the first number greater than or equal to x
        Bound lowerBound(std::lower_bound(_xy.begin(), _xy.end(), value, compare));

        // if the lower bound is equal to x
        if (Op::isEq(x, lowerBound->first))
            return Term::_height * lowerBound->second;
        // find the upper bound starting from a copy of lowerBound
        const Bound upperBound(std::upper_bound(_xy.begin(), _xy.end(), value, compare));
        --lowerBound;  // One arithmetic
        return Term::_height
               * Op::scale(x, lowerBound->first, upperBound->first, lowerBound->second, upperBound->second);
    }

    std::string Discrete::parameters() const {
        std::vector<std::string> result;
        for (std::size_t i = 0; i < xy().size(); ++i) {
            result.push_back(Op::str(xy().at(i).first));
            result.push_back(Op::str(xy().at(i).second));
        }
        if (not Op::isEq(getHeight(), 1.0))
            result.push_back(Op::str(getHeight()));
        return Op::join(result, " ");
    }

    void Discrete::configure(const std::string& parameters) {
        if (parameters.empty())
            return;
        std::vector<std::string> strValues = Op::split(parameters, " ");
        std::vector<scalar> values(strValues.size());
        for (std::size_t i = 0; i < strValues.size(); ++i)
            values.at(i) = Op::toScalar(strValues.at(i));
        if (values.size() % 2 == 0) {
            setHeight(1.0);
        } else {
            setHeight(values.back());
            values.pop_back();
        }
        this->_xy = toPairs(values);
    }

    void Discrete::setXY(const std::vector<Pair>& xy) {
        this->_xy = xy;
    }

    const std::vector<Discrete::Pair>& Discrete::xy() const {
        return this->_xy;
    }

    std::vector<Discrete::Pair>& Discrete::xy() {
        return this->_xy;
    }

    const Discrete::Pair& Discrete::xy(std::size_t index) const {
        return this->_xy.at(index);
    }

    Discrete::Pair& Discrete::xy(std::size_t index) {
        return this->_xy.at(index);
    }

    std::vector<scalar> Discrete::x() const {
        std::vector<scalar> result(_xy.size());
        for (std::size_t i = 0; i < result.size(); ++i)
            result.at(i) = _xy.at(i).first;
        return result;
    }

    std::vector<scalar> Discrete::y() const {
        std::vector<scalar> result(_xy.size());
        for (std::size_t i = 0; i < result.size(); ++i)
            result.at(i) = _xy.at(i).second;
        return result;
    }

    scalar Discrete::x(std::size_t index) const {
        return _xy.at(index).first;
    }

    scalar& Discrete::x(std::size_t index) {
        return _xy.at(index).first;
    }

    scalar Discrete::y(std::size_t index) const {
        return _xy.at(index).second;
    }

    scalar& Discrete::y(std::size_t index) {
        return _xy.at(index).second;
    }

    std::vector<Discrete::Pair> Discrete::toPairs(const std::vector<scalar>& xy) {
        if (xy.size() % 2 != 0) {
            std::ostringstream os;
            os << "[discrete error] missing value in set of pairs (|xy|=" << xy.size() << ")";
            throw Exception(os.str(), FL_AT);
        }

        std::vector<Pair> result((xy.size() + 1) / 2);
        for (std::size_t i = 0; i + 1 < xy.size(); i += 2) {
            result.at(i / 2).first = xy.at(i);
            result.at(i / 2).second = xy.at(i + 1);
        }
        return result;
    }

    std::vector<Discrete::Pair> Discrete::toPairs(const std::vector<scalar>& xy, scalar missingValue) FL_INOEXCEPT {
        if (xy.size() % 2 == 0)
            return toPairs(xy);
        std::vector<scalar> copy(xy);
        copy.push_back(missingValue);
        return toPairs(copy);
    }

    std::vector<scalar> Discrete::toVector(const std::vector<Pair>& xy) {
        std::vector<scalar> result(xy.size() * 2);
        for (std::size_t i = 0; i < xy.size(); ++i) {
            result.at(2 * i) = xy.at(i).first;
            result.at(2 * i + 1) = xy.at(i).second;
        }
        return result;
    }

    std::string Discrete::formatXY(
        const std::vector<Pair>& xy,
        const std::string& prefix,
        const std::string& innerSeparator,
        const std::string& suffix,
        const std::string& outerSeparator
    ) {
        std::ostringstream os;
        for (std::size_t i = 0; i < xy.size(); ++i) {
            os << prefix << Op::str(xy.at(i).first) << innerSeparator << Op::str(xy.at(i).second) << suffix;
            if (i + 1 < xy.size())
                os << outerSeparator;
        }
        return os.str();
    }

    Discrete*
    Discrete::discretize(const Term* term, scalar start, scalar end, int resolution, bool boundedMembershipFunction) {
        FL_unique_ptr<Discrete> result(new Discrete(term->getName()));
        scalar dx = (end - start) / resolution;
        scalar x, y;
        for (int i = 0; i <= resolution; ++i) {
            x = start + i * dx;
            y = term->membership(x);
            if (boundedMembershipFunction)
                y = Op::bound(y, scalar(0.0), scalar(1.0));
            result->xy().push_back(Discrete::Pair(x, y));
        }
        return result.release();
    }

    Discrete* Discrete::clone() const {
        return new Discrete(*this);
    }

    Term* Discrete::constructor() {
        return new Discrete;
    }

}
