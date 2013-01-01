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

        static scalar Min(scalar a, scalar b) {
            return a < b ? a : b;
        }

        static scalar Max(scalar a, scalar b) {
            return a > b ? a : b;
        }

        static bool IsInf(scalar x) {
            return std::isinf(x);
        }

        static bool IsNan(scalar x) {
            return std::isnan(x);
        }

        //Is less than

        static bool IsLt(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return not IsEq(a, b, tolerance) and a < b;
        }

        //Is less than

        static bool IsLE(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return IsEq(a, b, tolerance) or a < b;
        }

        //Is equal

        static bool IsEq(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return std::fabs(a - b) < tolerance;
        }

        //Is greater than

        static bool IsGt(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return not IsEq(a, b, tolerance) and a > b;
        }

        static bool IsGE(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return IsEq(a, b, tolerance) or a > b;
        }

        static scalar Scale(scalar x, scalar fromMin, scalar fromMax, scalar toMin, scalar toMax) {
            return (toMax - toMin) / (fromMax - fromMin) * (x - fromMin) + toMin;
        }

        static std::string FindReplace(const std::string& str, const std::string& find,
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

        static std::vector<std::string> Split(const std::string& str, const std::string& delimiters) {
            std::vector<std::string> result;
            std::string::size_type last_pos =
                    str.find_first_not_of(delimiters, 0);
            // Find first "non-delimiter".
            std::string::size_type pos =
                    str.find_first_of(delimiters, last_pos);

            while (std::string::npos != pos || std::string::npos != last_pos) {
                // Found a token, add it to the vector.
                result.push_back(str.substr(last_pos, pos - last_pos));
                // Skip delimiters.  Note the "not_of"
                last_pos = str.find_first_not_of(delimiters, pos);
                // Find next "non-delimiter"
                pos = str.find_first_of(delimiters, last_pos);
            }
            return result;
        }

        static std::string LeftTrim(const std::string& text) {
            std::size_t index = text.find_first_not_of(" ");
            if (index != std::string::npos)
                return text.substr(index);
            return text;
        }

        static std::string RightTrim(const std::string& text) {
            std::size_t index = text.find_last_not_of(" ");
            if (index != std::string::npos) {
                return text.substr(0, index + 1);
            }
            return text;
        }

        static std::string Trim(const std::string& text) {
            return RightTrim(LeftTrim(text));
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
                throw fl::Exception(ex.str());
            }
            return alternative;
        }

        template <typename T>
        static std::string toString(T x, int precision = 3) {
            std::ostringstream ss;
            ss << std::setprecision(precision) << std::fixed;
            ss << x;
            return ss.str();
        }
        
        template <typename T>
        static std::string toString(int precision = 3,
                const std::string& separator = " ", int argc = 0, ...) {
            std::ostringstream ss;
            ss << std::setprecision(precision) << std::fixed;
            va_list list;
            va_start(list, argc);
            for (int i = 0; i < argc; ++i) {
                ss << separator << va_arg(list, T);
            }
            va_end(list);
            return ss.str();
        }

        static std::vector<std::pair<scalar, scalar> >
        Overlap(int number_of_sections,
                scalar min, scalar max, scalar factor = 0.5) {
            std::vector<std::pair<scalar, scalar> > limits;
            scalar range = (max - min) / (factor * (number_of_sections + 1));
            scalar current_step = min;
            for (int i = 0; i < number_of_sections; ++i) {
                limits.push_back(std::make_pair(current_step,
                        current_step + range));
                current_step += factor * range;
            }
            return limits;
        }
    };
}
#endif	/* FL_OPERATOR_H */

