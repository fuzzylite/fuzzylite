/* 
 * File:   Operator.h
 * Author: jcrada
 *
 * Created on 21 December 2012, 9:31 AM
 */

#ifndef FL_OPERATOR_H
#define	FL_OPERATOR_H

#include "fl/scalar.h"

#include "fl/definitions.h"

#include "fl/Exception.h"

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>
#include <limits>
#include <utility>
#include <stdarg.h>
namespace fl {

    class Operator {
    public:

        Operator() {
        }

        virtual ~Operator() {
        }

        virtual std::string className() const = 0;
        virtual scalar compute(scalar a, scalar b) const = 0;

    };

    class Op {
    public:

        static scalar min(scalar a, scalar b) {
            return a < b ? a : b;
        }

        static scalar max(scalar a, scalar b) {
            return a > b ? a : b;
        }

        static bool isInf(scalar x) {
            return std::isinf(x);
        }

        static bool isNan(scalar x) {
            return std::isnan(x);
        }

        //Is less than

        static bool isLt(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return not isEq(a, b, tolerance) and a < b;
        }

        static bool isLE(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return isEq(a, b, tolerance) or a < b;
        }

        //Is equal

        static bool isEq(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return std::fabs(a - b) < tolerance;
        }

        //Is greater than

        static bool isGt(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return not isEq(a, b, tolerance) and a > b;
        }

        static bool isGE(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return isEq(a, b, tolerance) or a > b;
        }

        static scalar scale(scalar x, scalar fromMin, scalar fromMax, scalar toMin, scalar toMax) {
            return (toMax - toMin) / (fromMax - fromMin) * (x - fromMin) + toMin;
        }

        static std::string findReplace(const std::string& str, const std::string& find,
                const std::string& replace, bool replaceAll = true) {
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

        static std::vector<std::string> split(const std::string& str,
                const std::string& delimiter = " ", bool ignoreEmpty = true) {
            std::vector<std::string> result;
            if (delimiter.empty()) {
                result.push_back(str);
                return result;
            }
            std::string::const_iterator position = str.begin(), next;
            while (next != str.end()) {
                next = std::search(position, str.end(), delimiter.begin(), delimiter.end());
                std::string token(position, next);

                if (not (token.empty() and ignoreEmpty)) {
                    result.push_back(token);
                }

                position = next + delimiter.size();
            }
            return result;
        }

        static std::string leftTrim(const std::string& text) {
            std::size_t index = text.find_first_not_of(" ");
            if (index != std::string::npos)
                return text.substr(index);
            return text;
        }

        static std::string rightTrim(const std::string& text) {
            std::size_t index = text.find_last_not_of(" ");
            if (index != std::string::npos) {
                return text.substr(0, index + 1);
            }
            return text;
        }

        static std::string trim(const std::string& text) {
            return rightTrim(leftTrim(text));
        }

        static scalar toScalar(const std::string& x, bool quiet = false,
                scalar alternative = std::numeric_limits<scalar>::quiet_NaN())
        throw (fl::Exception) {
            std::istringstream iss(x);
            scalar result;
            iss >> result;
            if (not iss.fail()) return result;

            std::ostringstream nan, pInf, nInf;
            nan << std::numeric_limits<scalar>::quiet_NaN();
            pInf << std::numeric_limits<scalar>::infinity();
            nInf << (-std::numeric_limits<scalar>::infinity());

            if (x == nan.str())
                return std::numeric_limits<scalar>::quiet_NaN();
            if (x == pInf.str())
                return std::numeric_limits<scalar>::infinity();
            if (x == nInf.str())
                return -std::numeric_limits<scalar>::infinity();
            if (!quiet) {
                std::ostringstream ex;
                ex << "[conversion error] from <" << x << "> to scalar";
                throw fl::Exception(ex.str(), FL_AT);
            }
            return alternative;
        }

        static std::string str(scalar x, int precision = FL_DECIMALS) {
            std::ostringstream ss;
            ss << std::setprecision(precision) << std::fixed;
            ss << x;
            return ss.str();
        }

        static std::string str(int items, scalar* x, const std::string& separator = ",",
                int precision = FL_DECIMALS) {
            std::ostringstream ss;
            ss << std::setprecision(precision) << std::fixed;
            for (int i = 0; i < items; ++i) {
                ss << x[i];
                if (i < items - 1) ss << separator;
            }
            return ss.str();
        }

        template <typename T>
        static std::string str(const std::vector<T>& x,
                const std::string& separator = ", ", int precision = FL_DECIMALS) {
            std::ostringstream ss;
            ss << std::setprecision(precision) << std::fixed;
            for (std::size_t i = 0; i < x.size(); ++i) {
                ss << x[i];
                if (i < x.size() - 1) ss << separator;
            }
            return ss.str();
        }
    };
}
#endif	/* FL_OPERATOR_H */

