/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.

 fuzzylite™ is a trademark of FuzzyLite Limited.

 */

/**
 * \file fl/term/Linear.tpp
 *
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 */

#ifndef FL_LINEAR_TPP
#define FL_LINEAR_TPP

namespace fl {

//Warning: this method is unsafe, make sure you use it correctly.
template <typename T>
Linear* Linear::create(const std::string& name, const Engine* engine,
        T firstCoefficient, ...) { // throw (fl::Exception);
    if (not engine) throw fl::Exception("[linear error] cannot create term <" + name + "> "
            "without a reference to the engine", FL_AT);
    std::vector<scalar> coefficients;
    coefficients.push_back(firstCoefficient);

    std::va_list args;
    va_start(args, firstCoefficient);
    for (std::size_t i = 0; i < engine->inputVariables().size(); ++i) {
        coefficients.push_back((scalar) va_arg(args, T));
    }
    va_end(args);

    return new Linear(name, coefficients, engine);
}

} // Namespace fl

#endif // FL_LINEAR_TPP
