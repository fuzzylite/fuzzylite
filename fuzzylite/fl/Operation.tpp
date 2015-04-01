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
 * \file fl/Operation.tpp
 *
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly.
 */

#ifndef FL_OPERATION_TPP
#define FL_OPERATION_TPP

namespace fl {

template <typename T>
T Operation::min(T a, T b) {
    if (isNaN(a)) return b;
    if (isNaN(b)) return a;
    return a < b ? a : b;
}

template <typename T>
T Operation::max(T a, T b) {
    if (isNaN(a)) return b;
    if (isNaN(b)) return a;
    return a > b ? a : b;
}

template <typename T>
T Operation::bound(T x, T min, T max) {
    if (isGt(x, max)) return max;
    if (isLt(x, min)) return min;
    return x;
}

template <typename T>
bool Operation::in(T x, T min, T max, bool geq, bool leq) {
    bool left = geq ? isGE(x, min) : isGt(x, min);
    bool right = leq ? isLE(x, max) : isLt(x, max);
    return (left and right);
}

template <typename T>
bool Operation::isInf(T x) {
    return std::abs(x) == fl::inf;
}

template <typename T>
bool Operation::isNaN(T x) {
    return not (x == x);
}

template <typename T>
bool Operation::isFinite(T x) {
    return not (isNaN(x) or isInf(x));
}

template <typename T>
std::string Operation::str(T x, int decimals) {
    std::ostringstream ss;
    ss << std::setprecision(decimals) << std::fixed;
    if (isNaN(x)) {
        ss << "nan";
    } else if (isInf(x)) {
        if (isLt(x, 0.0)) ss << "-";
        ss << "inf";
    } else if (isEq(x, 0.0)) {
        ss << std::fabs((x * 0.0)); //believe it or not, -1.33227e-15 * 0.0 = -0.0
    } else ss << x;
    return ss.str();
}

template <typename T>
std::string Operation::join(const std::vector<T>& x, const std::string& separator) {
    std::ostringstream ss;
    for (std::size_t i = 0; i < x.size(); ++i) {
        ss << str(x.at(i));
        if (i + 1 < x.size()) ss << separator;
    }
    return ss.str();
}

template <typename T>
std::string Operation::join(int items, const std::string& separator, T first, ...) {
    std::ostringstream ss;
    ss << str(first);
    if (items > 1) ss << separator;
    std::va_list args;
    va_start(args, first);
    for (int i = 0; i < items - 1; ++i) {
        ss << str(va_arg(args, T));
        if (i + 1 < items - 1) ss << separator;
    }
    va_end(args);
    return ss.str();
}

} // Namespace fl

#endif // FL_OPERATION_TPP
