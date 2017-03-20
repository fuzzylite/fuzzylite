/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/Complexity.h"

#include "fl/Engine.h"

#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"

namespace fl {

    Complexity::Complexity(scalar all) :
    _comparison(all), _arithmetic(all), _function(all) { }

    Complexity::Complexity(scalar comparison, scalar arithmetic,
            scalar function)
    : _comparison(comparison), _arithmetic(arithmetic), _function(function) { }

    Complexity::~Complexity() { }

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
        return *this;
    }

    Complexity& Complexity::plus(scalar x) {
        return this->plus(Complexity().arithmetic(x).comparison(x).function(x));
    }

    Complexity& Complexity::minus(const Complexity& other) {
        this->_comparison -= other._comparison;
        this->_arithmetic -= other._arithmetic;
        this->_function -= other._function;
        return *this;
    }

    Complexity& Complexity::minus(scalar x) {
        return this->minus(Complexity().arithmetic(x).comparison(x).function(x));
    }

    Complexity& Complexity::multiply(const Complexity& other) {
        this->_comparison *= other._comparison;
        this->_arithmetic *= other._arithmetic;
        this->_function *= other._function;
        return *this;
    }

    Complexity& Complexity::multiply(scalar x) {
        return this->multiply(Complexity().arithmetic(x).comparison(x).function(x));
    }

    Complexity& Complexity::divide(const Complexity& other) {
        this->_comparison /= other._comparison;
        this->_arithmetic /= other._arithmetic;
        this->_function /= other._function;
        return *this;
    }

    Complexity& Complexity::divide(scalar x) {
        return this->divide(Complexity().arithmetic(x).comparison(x).function(x));
    }

    bool Complexity::equals(const Complexity& x, scalar macheps) const {
        return Op::isEq(_comparison, x._comparison, macheps) and
                Op::isEq(_arithmetic, x._arithmetic, macheps) and
                Op::isEq(_function, x._function, macheps);
    }

    bool Complexity::lessThan(const Complexity& x, scalar macheps) const {
        return Op::isLt(_comparison, x._comparison, macheps) and
                Op::isLt(_arithmetic, x._arithmetic, macheps) and
                Op::isLt(_function, x._function, macheps);
    }

    bool Complexity::lessThanOrEqualsTo(const Complexity& x, scalar macheps) const {
        return Op::isLE(_comparison, x._comparison, macheps) and
                Op::isLE(_arithmetic, x._arithmetic, macheps) and
                Op::isLE(_function, x._function, macheps);
    }

    bool Complexity::greaterThan(const Complexity& x, scalar macheps) const {
        return Op::isGt(_comparison, x._comparison, macheps) and
                Op::isGt(_arithmetic, x._arithmetic, macheps) and
                Op::isGt(_function, x._function, macheps);
    }

    bool Complexity::greaterThanOrEqualsTo(const Complexity& x, scalar macheps) const {
        return Op::isGE(_comparison, x._comparison, macheps) and
                Op::isGE(_arithmetic, x._arithmetic, macheps) and
                Op::isGE(_function, x._function, macheps);
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

    std::vector<Complexity::Measure> Complexity::measures() const {
        std::vector<Measure> result;
        result.push_back(Measure("arithmetic", _arithmetic));
        result.push_back(Measure("comparison", _comparison));
        result.push_back(Measure("function", _function));
        return result;
    }

    scalar Complexity::sum() const {
        return _arithmetic + _comparison + _function;
    }

    scalar Complexity::norm() const {
        return std::sqrt(Complexity(*this).multiply(*this).sum());
    }

    std::string Complexity::toString() const {
        std::vector<std::string> result;
        result.push_back("a=" + Op::str(_arithmetic));
        result.push_back("c=" + Op::str(_comparison));
        result.push_back("f=" + Op::str(_function));
        return "C[" + Op::join(result, ", ") + "]";
    }

    Complexity Complexity::compute(const Engine* engine) const {
        return engine->complexity();
    }

    Complexity Complexity::compute(const InputVariable* inputVariable) const {
        return inputVariable->complexity();
    }

    Complexity Complexity::compute(const OutputVariable* outputVariable) const {
        return outputVariable->complexity();
    }

    Complexity Complexity::compute(const RuleBlock* ruleBlock) const {
        return ruleBlock->complexity();
    }

    Complexity Complexity::compute(const std::vector<InputVariable*>& inputVariables) const {
        Complexity result;
        for (std::size_t i = 0; i < inputVariables.size(); ++i) {
            result += inputVariables.at(i)->complexity();
        }
        return result;
    }

    Complexity Complexity::compute(const std::vector<OutputVariable*>& outputVariables,
            bool complexityOfDefuzzification) const {
        Complexity result;
        for (std::size_t i = 0; i < outputVariables.size(); ++i) {
            if (complexityOfDefuzzification)
                result += outputVariables.at(i)->complexityOfDefuzzification();
            else
                result += outputVariables.at(i)->complexity();
        }
        return result;
    }

    Complexity Complexity::compute(const std::vector<Variable*>& variables) const {
        Complexity result;
        for (std::size_t i = 0; i < variables.size(); ++i) {
            result += variables.at(i)->complexity();
        }
        return result;
    }

    Complexity Complexity::compute(const std::vector<RuleBlock*>& ruleBlocks) const {
        Complexity result;
        for (std::size_t i = 0; i < ruleBlocks.size(); ++i) {
            result += ruleBlocks.at(i)->complexity();
        }
        return result;
    }

}
