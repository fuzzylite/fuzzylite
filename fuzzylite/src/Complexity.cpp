/*
 Copyright (C) 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite(R).

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite(R) is a registered trademark of FuzzyLite Limited.
 */

#include "fl/Complexity.h"

#include "fl/Operation.h"

namespace fl {

    Complexity::Complexity() :
    _comparison(0.0), _arithmetic(0.0), _function(0.0), _other(0.0) {
    }

    Complexity::~Complexity() {
    }

    Complexity& Complexity::operator+=(const Complexity& other) {
        return this->plus(other);
    }

    Complexity& Complexity::operator-=(const Complexity& other) {
        return this->minus(other);
    }

    Complexity& Complexity::operator*=(const Complexity& other) {
        return this->multiply(other);
    }

    Complexity& Complexity::operator/=(const Complexity& other) {
        return this->divide(other);
    }

    Complexity Complexity::operator+(const Complexity& rhs) const {
        return Complexity(*this).plus(rhs);
    }

    Complexity Complexity::operator-(const Complexity& rhs) const {
        return Complexity(*this).minus(rhs);
    }

    Complexity Complexity::operator*(const Complexity& rhs) const {
        return Complexity(*this).multiply(rhs);
    }

    Complexity Complexity::operator/(const Complexity& rhs) const {
        return Complexity(*this).divide(rhs);
    }

    bool Complexity::operator==(const Complexity& rhs) const {
        return equals(rhs);
    }

    bool Complexity::operator!=(const Complexity& rhs) const {
        return not equals(rhs);
    }

    bool Complexity::operator<(const Complexity& rhs) const {
        return lessThan(rhs);
    }

    bool Complexity::operator<=(const Complexity& rhs) const {
        return lessThanOrEqualsTo(rhs);
    }

    bool Complexity::operator>(const Complexity& rhs) const {
        return greaterThan(rhs);
    }

    bool Complexity::operator>=(const Complexity& rhs) const {
        return greaterThanOrEqualsTo(rhs);
    }

    Complexity& Complexity::plus(const Complexity& other) {
        this->_arithmetic += other._arithmetic;
        this->_comparison += other._comparison;
        this->_function += other._function;
        this->_other += other._other;
        return *this;
    }

    Complexity& Complexity::plus(scalar x) {
        return this->plus(Complexity().arithmetic(x).comparison(x).function(x).other(x));
    }

    Complexity& Complexity::minus(const Complexity& other) {
        this->_comparison -= other._comparison;
        this->_arithmetic -= other._arithmetic;
        this->_function -= other._function;
        this->_other -= other._other;
        return *this;
    }

    Complexity& Complexity::minus(scalar x) {
        return this->minus(Complexity().arithmetic(x).comparison(x).function(x).other(x));
    }

    Complexity& Complexity::multiply(const Complexity& other) {
        this->_comparison *= other._comparison;
        this->_arithmetic *= other._arithmetic;
        this->_function *= other._function;
        this->_other *= other._other;
        return *this;
    }

    Complexity& Complexity::multiply(scalar x) {
        return this->multiply(Complexity().arithmetic(x).comparison(x).function(x).other(x));
    }

    Complexity& Complexity::divide(const Complexity& other) {
        this->_comparison /= other._comparison;
        this->_arithmetic /= other._arithmetic;
        this->_function /= other._function;
        this->_other /= other._other;
        return *this;
    }

    Complexity& Complexity::divide(scalar x) {
        return this->divide(Complexity().arithmetic(x).comparison(x).function(x).other(x));
    }

    bool Complexity::equals(const Complexity& x, scalar macheps) const {
        return Op::isEq(_comparison, x._comparison, macheps) and
                Op::isEq(_arithmetic, x._arithmetic, macheps) and
                Op::isEq(_function, x._function, macheps) and
                Op::isEq(_other, x._other, macheps);
    }

    bool Complexity::lessThan(const Complexity& x, scalar macheps) const {
        return Op::isLt(_comparison, x._comparison, macheps) and
                Op::isLt(_arithmetic, x._arithmetic, macheps) and
                Op::isLt(_function, x._function, macheps) and
                Op::isLt(_other, x._other, macheps);
    }

    bool Complexity::lessThanOrEqualsTo(const Complexity& x, scalar macheps) const {
        return Op::isLE(_comparison, x._comparison, macheps) and
                Op::isLE(_arithmetic, x._arithmetic, macheps) and
                Op::isLE(_function, x._function, macheps) and
                Op::isLE(_other, x._other, macheps);
    }

    bool Complexity::greaterThan(const Complexity& x, scalar macheps) const {
        return Op::isGt(_comparison, x._comparison, macheps) and
                Op::isGt(_arithmetic, x._arithmetic, macheps) and
                Op::isGt(_function, x._function, macheps) and
                Op::isGt(_other, x._other, macheps);
    }

    bool Complexity::greaterThanOrEqualsTo(const Complexity& x, scalar macheps) const {
        return Op::isGE(_comparison, x._comparison, macheps) and
                Op::isGE(_arithmetic, x._arithmetic, macheps) and
                Op::isGE(_function, x._function, macheps) and
                Op::isGE(_other, x._other, macheps);
    }

    Complexity& Complexity::comparison(scalar comparison) {
        this->_comparison += comparison;
        return *this;
    }

    void Complexity::setComparison(scalar comparison) {
        this->_comparison = comparison;
    }

    scalar Complexity::getComparison() const {
        return _comparison;
    }

    Complexity& Complexity::arithmetic(scalar arithmetic) {
        this->_arithmetic += arithmetic;
        return *this;
    }

    void Complexity::setArithmetic(scalar arithmetic) {
        this->_arithmetic = arithmetic;
    }

    scalar Complexity::getArithmetic() const {
        return _arithmetic;
    }

    Complexity& Complexity::function(scalar trigonometric) {
        this->_function += trigonometric;
        return *this;
    }

    void Complexity::setFunction(scalar trigonometric) {
        this->_function = trigonometric;
    }

    scalar Complexity::getFunction() const {
        return _function;
    }

    Complexity& Complexity::other(scalar others) {
        this->_other += others;
        return *this;
    }

    void Complexity::setOther(scalar others) {
        this->_other = others;
    }

    scalar Complexity::getOther() const {
        return _other;
    }

    scalar Complexity::sum() const {
        return _arithmetic + _comparison + _function + _other;
    }

    scalar Complexity::norm() const {
        return std::sqrt(Complexity(*this).multiply(*this).sum());
    }

    std::string Complexity::toString() const {
        std::vector<std::string> result;
        result.push_back("arithmetic=" + Op::str(_arithmetic));
        result.push_back("comparison=" + Op::str(_comparison));
        result.push_back("function=" + Op::str(_function));
        result.push_back("other=" + Op::str(_other));
        return "Complexity[" + Op::join(result, ", ") + "]";
    }

}
