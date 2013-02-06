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

#include "fl/operator/Operation.h"

#include <algorithm>
#include <cmath>
//#include <sstream>
//#include <iostream>
#include <iomanip>
//#include <limits>
//#include <utility>
//#include <stdarg.h>

namespace fl {

    scalar Operation::min(scalar a, scalar b) {
        return a < b ? a : b;
    }

    scalar Operation::max(scalar a, scalar b) {
        return a > b ? a : b;
    }

    bool Operation::isInf(scalar x) {
        return std::abs(x) == std::numeric_limits<scalar>::infinity();
    }

    bool Operation::isNan(scalar x) {
        return not (x == x);
    }

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

    std::string Operation::leftTrim(const std::string& text) {
        std::size_t index = text.find_first_not_of(" ");
        if (index != std::string::npos)
            return text.substr(index);
        return text;
    }

    std::string Operation::rightTrim(const std::string& text) {
        std::size_t index = text.find_last_not_of(" ");
        if (index != std::string::npos) {
            return text.substr(0, index + 1);
        }
        return text;
    }

    std::string Operation::trim(const std::string& text) {
        return rightTrim(leftTrim(text));
    }

    scalar Operation::toScalar(const std::string& x, bool quiet, scalar alternative)
    throw (fl::Exception) {
        std::istringstream iss(x);
        scalar result;
        iss >> result;
        if (not iss.fail()) return result;

        std::ostringstream nan, pInf, nInf;
        nan << std::numeric_limits<scalar>::quiet_NaN();
        pInf << std::numeric_limits<scalar>::infinity();
        nInf << (-std::numeric_limits<scalar>::infinity());

        if (x == nan.str() or x == "nan")
            return std::numeric_limits<scalar>::quiet_NaN();
        if (x == pInf.str() or x == "inf")
            return std::numeric_limits<scalar>::infinity();
        if (x == nInf.str() or x == "-inf")
            return -std::numeric_limits<scalar>::infinity();
        if (!quiet) {
            std::ostringstream ex;
            ex << "[conversion error] from <" << x << "> to scalar";
            throw fl::Exception(ex.str(), FL_AT);
        }
        return alternative;
    }

    std::string Operation::str(int x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    }

    std::string Operation::str(scalar x, int precision) {
        std::ostringstream ss;
        ss << std::setprecision(precision) << std::fixed;
        if (fl::Op::isNan(x)) ss << "nan";
        else if (fl::Op::isInf(x)) {
            if (x < 0) ss << "-";
            ss << "inf";
        } else ss << x;
        return ss.str();
    }

    std::string Operation::str(int items, scalar* x, const std::string& separator,
            int precision) {
        std::ostringstream ss;
        for (int i = 0; i < items; ++i) {
            ss << fl::Op::str(x[i], precision);
            if (i < items - 1) ss << separator;
        }
        return ss.str();
    }

    template <typename T>
    std::string Operation::str(const std::vector<T>& x,
            const std::string& separator, int precision) {
        std::ostringstream ss;
        for (std::size_t i = 0; i < x.size(); ++i) {
            ss << fl::Op::str(x.at(i), precision);
            if (i < x.size() - 1) ss << separator;
        }
        return ss.str();
    }
}
