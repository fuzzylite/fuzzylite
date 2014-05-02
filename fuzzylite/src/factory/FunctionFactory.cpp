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

/* 
 * File:   FunctionFactory.cpp
 * Author: jcrada
 *
 * Created on 26 March 2014, 10:12 PM
 */

#include "fl/factory/FunctionFactory.h"

#include "fl/rule/Rule.h"

namespace fl {

    FunctionFactory::FunctionFactory() {

        //OPERATORS:
        int p = 10;
        //First order: not, negate: 
        registerObject("!", new Function::Element("!", Function::Element::OPERATOR,
                &(fl::Op::logicalNot), p, 1)); //logical not
        registerObject("~", new Function::Element("~", Function::Element::OPERATOR,
                &(fl::Op::negate), p, 1)); // ~ negates a number

        --p;
        //Second order: power
        registerObject("^", new Function::Element("^", Function::Element::OPERATOR,
                &(std::pow), p, 1));

        --p;
        //Third order: multiplication, division, modulo
        registerObject("*", new Function::Element("*", Function::Element::OPERATOR,
                &(fl::Op::multiply), p));
        registerObject("/", new Function::Element("/", Function::Element::OPERATOR,
                &(fl::Op::divide), p));
        registerObject("%", new Function::Element("%", Function::Element::OPERATOR,
                &(fl::Op::modulo), p));

        --p;
        //Fourth order: addition, subtraction
        registerObject("+", new Function::Element("+", Function::Element::OPERATOR,
                &(fl::Op::add), p));
        registerObject("-", new Function::Element("-",
                Function::Element::OPERATOR, &(fl::Op::subtract), p));

        //Fifth order: logical and, logical or
        --p; //Logical AND
        registerObject(fl::Rule::andKeyword(), new Function::Element(fl::Rule::andKeyword(),
                Function::Element::OPERATOR, &(fl::Op::logicalAnd), p));
        --p; //Logical OR
        registerObject(fl::Rule::orKeyword(), new Function::Element(fl::Rule::orKeyword(),
                Function::Element::OPERATOR, &(fl::Op::logicalOr), p));

        //FUNCTIONS

        registerObject("gt", new Function::Element("gt", Function::Element::FUNCTION,
                &(fl::Op::gt)));
        registerObject("ge", new Function::Element("ge", Function::Element::FUNCTION,
                &(fl::Op::ge)));
        registerObject("eq", new Function::Element("eq", Function::Element::FUNCTION,
                &(fl::Op::eq)));
        registerObject("neq", new Function::Element("neq", Function::Element::FUNCTION,
                &(fl::Op::neq)));
        registerObject("le", new Function::Element("le", Function::Element::FUNCTION,
                &(fl::Op::le)));
        registerObject("lt", new Function::Element("lt", Function::Element::FUNCTION,
                &(fl::Op::lt)));

        //        registerObject("not", new Function::Element("not", Function::Element::FUNCTION,
        //                &(fl::Op::logicalNot))); //Change name as conflicts when parsing Hedges 
        registerObject("round", new Function::Element("round", Function::Element::FUNCTION,
                &(fl::Op::round)));

        registerObject("acos", new Function::Element("acos", Function::Element::FUNCTION,
                &(std::acos)));
        registerObject("asin", new Function::Element("asin", Function::Element::FUNCTION,
                &(std::asin)));
        registerObject("atan", new Function::Element("atan", Function::Element::FUNCTION,
                &(std::atan)));

        registerObject("ceil", new Function::Element("ceil", Function::Element::FUNCTION,
                &(std::ceil)));
        registerObject("cos", new Function::Element("cos", Function::Element::FUNCTION,
                &(std::cos)));
        registerObject("cosh", new Function::Element("cosh", Function::Element::FUNCTION,
                &(std::cosh)));
        registerObject("exp", new Function::Element("exp", Function::Element::FUNCTION,
                &(std::exp)));
        registerObject("fabs", new Function::Element("fabs", Function::Element::FUNCTION,
                &(std::fabs)));
        registerObject("floor", new Function::Element("floor", Function::Element::FUNCTION,
                &(std::floor)));
        registerObject("log", new Function::Element("log", Function::Element::FUNCTION,
                &(std::log)));
        registerObject("log10", new Function::Element("log10", Function::Element::FUNCTION,
                &(std::log10)));

        registerObject("sin", new Function::Element("sin", Function::Element::FUNCTION,
                &(std::sin)));
        registerObject("sinh", new Function::Element("sinh", Function::Element::FUNCTION,
                &(std::sinh)));
        registerObject("sqrt", new Function::Element("sqrt", Function::Element::FUNCTION,
                &(std::sqrt)));
        registerObject("tan", new Function::Element("tan", Function::Element::FUNCTION,
                &(std::tan)));
        registerObject("tanh", new Function::Element("tanh", Function::Element::FUNCTION,
                &(std::tanh)));

#if defined(FL_UNIX) && !defined(FL_USE_FLOAT)
        //found in Unix when using double precision. not found in Windows.
        registerObject("log1p", new Function::Element("log1p", Function::Element::FUNCTION,
                &(log1p)));
        registerObject("acosh", new Function::Element("acosh", Function::Element::FUNCTION,
                &(acosh)));
        registerObject("asinh", new Function::Element("asinh", Function::Element::FUNCTION,
                &(asinh)));
        registerObject("atanh", new Function::Element("atanh", Function::Element::FUNCTION,
                &(atanh)));
#endif

        registerObject("pow", new Function::Element("pow", Function::Element::FUNCTION,
                &(std::pow)));
        registerObject("atan2", new Function::Element("atan2", Function::Element::FUNCTION,
                &(std::atan2)));
        registerObject("fmod", new Function::Element("fmod", Function::Element::FUNCTION,
                &(std::fmod)));
    }

    FunctionFactory::~FunctionFactory() {

    }

    std::vector<std::string> FunctionFactory::availableOperators() const {
        std::vector<std::string> result;
        std::map<std::string, Function::Element*>::const_iterator it = this->_objects.begin();
        while (it != this->_objects.end()) {
            if (it->second and it->second->type == Function::Element::OPERATOR)
                result.push_back(it->first);
            ++it;
        }
        return result;
    }

    std::vector<std::string> FunctionFactory::availableFunctions() const {
        std::vector<std::string> result;
        std::map<std::string, Function::Element*>::const_iterator it = this->_objects.begin();
        while (it != this->_objects.end()) {
            if (it->second and it->second->type == Function::Element::FUNCTION)
                result.push_back(it->first);
            ++it;
        }
        return result;
    }

}
