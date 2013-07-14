/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "fl/Operation.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cstdarg>
#include <cctype>

namespace fl {

    template <typename T>
    T Operation::min(T a, T b) {
        if (isNan(a)) return b;
        if (isNan(b)) return a;
        return a < b ? a : b;
    }
    template FL_EXPORT scalar Operation::min(scalar a, scalar b);
    template FL_EXPORT int Operation::min(int a, int b);

    template <typename T>
    T Operation::max(T a, T b) {
        if (isNan(a)) return b;
        if (isNan(b)) return a;
        return a > b ? a : b;
    }
    template FL_EXPORT scalar Operation::max(scalar a, scalar b);
    template FL_EXPORT int Operation::max(int a, int b);

    template <typename T>
    bool Operation::isInf(T x) {
        return std::abs(x) == fl::inf;
    }
    template FL_EXPORT bool Operation::isInf(int x);
    template FL_EXPORT bool Operation::isInf(scalar x);

    template <typename T>
    bool Operation::isNan(T x) {
        return not (x == x);
    }
    template FL_EXPORT bool Operation::isNan(int x);
    template FL_EXPORT bool Operation::isNan(scalar x);

    bool Operation::isLt(scalar a, scalar b, scalar tolerance) {
        return not isEq(a, b, tolerance) and a < b;
    }

    bool Operation::isLE(scalar a, scalar b, scalar tolerance) {
        return isEq(a, b, tolerance) or a < b;
    }

    bool Operation::isEq(scalar a, scalar b, scalar tolerance) {
        return std::fabs(a - b) < tolerance;
    }

    bool Operation::isGt(scalar a, scalar b, scalar tolerance) {
        return not isEq(a, b, tolerance) and a > b;
    }

    bool Operation::isGE(scalar a, scalar b, scalar tolerance) {
        return isEq(a, b, tolerance) or a > b;
    }

    scalar Operation::scale(scalar x, scalar fromMin, scalar fromMax, scalar toMin, scalar toMax) {
        return (toMax - toMin) / (fromMax - fromMin) * (x - fromMin) + toMin;
    }

    scalar Operation::plus(scalar a, scalar b) {
        return a + b;
    }

    scalar Operation::minus(scalar a, scalar b) {
        return a - b;
    }

    scalar Operation::multiplies(scalar a, scalar b) {
        return a * b;
    }

    scalar Operation::divides(scalar a, scalar b) {
        return a / b;
    }

    scalar Operation::modulus(scalar a, scalar b) {
        return fmod(a, b);
    }

    scalar Operation::logical_and(scalar a, scalar b) {
        return not (isEq(a, 0.0) or isEq(b, 0.0));
    }

    scalar Operation::logical_or(scalar a, scalar b) {
        return not (isEq(a, 0.0) and isEq(b, 0.0));
    }

    scalar Operation::negate(scalar a) {
        return -a;
    }

    int Operation::isValidForName(int character) {
        return character == '_' or character == '.' or isalnum(character);
    }

    std::string Operation::findReplace(const std::string& str, const std::string& find,
            const std::string& replace, bool replaceAll) {
        std::ostringstream result;
        std::size_t fromIndex = 0, nextIndex;
        do {
            nextIndex = str.find(find, fromIndex);
            result << str.substr(fromIndex, nextIndex - fromIndex);
            if (nextIndex != std::string::npos)
                result << replace;
            fromIndex = nextIndex + find.size();
        } while (replaceAll and nextIndex != std::string::npos);
        return result.str();
    }

    std::vector<std::string> Operation::split(const std::string& str,
            const std::string& delimiter, bool ignoreEmpty) {
        std::vector<std::string> result;
        if (delimiter.empty()) {
            result.push_back(str);
            return result;
        }
        std::string::const_iterator position = str.begin(), next = str.begin();
        while (next != str.end()) {
            next = std::search(position, str.end(), delimiter.begin(), delimiter.end());
            std::string token(position, next);
            if (not (token.empty() and ignoreEmpty)) {
                result.push_back(token);
            }
            if (next != str.end()) {
                position = next + delimiter.size();
            }
        }
        return result;
    }

    std::string Operation::trim(const std::string& text) {
        if (text.empty()) return text;
        if (not (std::isspace(text.at(0)) or std::isspace(text.at(text.size() - 1))))
            return text;
        int start = 0, end = text.size() - 1;
        while (start <= end and std::isspace(text.at(start))) {
            ++start;
        }
        while (end >= start and std::isspace(text.at(end))) {
            --end;
        }
        int length = end - start + 1;
        if (length <= 0) return "";
        return text.substr(start, length);
    }

    std::string Operation::format(const std::string& text, int matchesChar(int),
            const std::string& replacement) {
        std::ostringstream ss;
        std::string::const_iterator it = text.begin();
        while (it != text.end()) {
            if (matchesChar(*it)) {
                ss << *it;
            } else {
                ss << replacement;
            }
            ++it;
        }
        return ss.str();
    }

    scalar Operation::toScalar(const std::string& x, bool quiet, scalar alternative)
    throw (fl::Exception) {
        std::istringstream iss(x);
        scalar result;
        iss >> result;
        char strict;
        if (not (iss.fail() or iss.get(strict))) return result;
        
        std::ostringstream nan, pInf, nInf;
        nan << fl::nan;
        pInf << fl::inf;
        nInf << (-fl::inf);

        if (x == nan.str() or x == "nan")
            return fl::nan;
        if (x == pInf.str() or x == "inf")
            return fl::inf;
        if (x == nInf.str() or x == "-inf")
            return -fl::inf;
        if (!quiet) {
            std::ostringstream ex;
            ex << "[conversion error] from <" << x << "> to scalar";
            throw fl::Exception(ex.str(), FL_AT);
        }
        return alternative;
    }

    bool Operation::isNumeric(const std::string& x) {
        try {
            fl::Op::toScalar(x);
            return true;
        } catch (fl::Exception& ex) {
            (void) ex;
            return false;
        }
    }

    template <typename T>
    std::string Operation::str(T x, int decimals) {
        std::ostringstream ss;
        ss << std::setprecision(decimals) << std::fixed;
        if (fl::Op::isNan(x)) {
            ss << "nan";
        } else if (fl::Op::isInf(x)) {
            if (fl::Op::isLt(x, 0.0)) ss << "-";
            ss << "inf";
        } else if (fl::Op::isEq(x, 0.0)) {
            ss << std::fabs((x * 0.0)); //believe it or not, -1.33227e-15 * 0.0 = -0.0
        } else ss << x;
        return ss.str();
    }
    template FL_EXPORT std::string Operation::str(short x, int precision);
    template FL_EXPORT std::string Operation::str(int x, int precision);
    template FL_EXPORT std::string Operation::str(scalar x, int precision);

    template <> FL_EXPORT std::string Operation::str(const std::string& x, int precision) {
        (void) precision;
        return x;
    }

    template <typename T>
    std::string Operation::str(const std::vector<T>& x,
            const std::string& separator) {
        std::ostringstream ss;
        for (std::size_t i = 0; i < x.size(); ++i) {
            ss << str(x.at(i));
            if (i + 1 < x.size()) ss << separator;
        }
        return ss.str();
    }
    template FL_EXPORT std::string Operation::str(const std::vector<int>& x,
            const std::string& separator);
    template FL_EXPORT std::string Operation::str(const std::vector<scalar>& x,
            const std::string& separator);

    template <> FL_EXPORT
    std::string Operation::str(const std::vector<std::string>& x,
            const std::string& separator) {
        std::ostringstream ss;
        for (std::size_t i = 0; i < x.size(); ++i) {
            ss << x.at(i);
            if (i + 1 < x.size()) ss << separator;
        }
        return ss.str();
    }

    template <typename T>
    std::string Operation::str(int items, const std::string& separator, T first, ...) {
        std::ostringstream ss;
        ss << str(first);
        if (items > 1) ss << separator;
        va_list args;
        va_start(args, first);
        for (int i = 0; i < items - 1; ++i) {
            ss << str(va_arg(args, T));
            if (i + 1 < items - 1) ss << separator;
        }
        va_end(args);
        return ss.str();
    }

    template FL_EXPORT std::string Operation::str(int items, const std::string& separator,
            int first, ...);
    template FL_EXPORT std::string Operation::str(int items, const std::string& separator,
            scalar first, ...);

    template <> FL_EXPORT
    std::string Operation::str(int items, const std::string& separator, const char* first, ...) {
        std::ostringstream ss;
        ss << first;
        if (items > 1) ss << separator;
        va_list args;
        va_start(args, first);
        for (int i = 0; i < items - 1; ++i) {
            ss << va_arg(args, const char*);
            if (i + 1 < items - 1) ss << separator;
        }
        va_end(args);
        return ss.str();
    }

}
