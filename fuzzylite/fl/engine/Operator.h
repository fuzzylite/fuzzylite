/*
 * Operator.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_OPERATOR_H_
#define FL_OPERATOR_H_

#include "fl/scalar.h"

#include "fl/definitions.h"

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>
#include <limits>

namespace fl {

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
            return std::fabs(a - b) < FL_EPSILON;
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
            } while (nextIndex != std::string::npos);
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

        static scalar Scalar(const std::string& x, bool quiet = false,
                scalar alternative = std::numeric_limits<scalar>::quiet_NaN())
                        throw (std::exception) {
            std::istringstream iss(x);
            scalar result;
            iss >> result;
            if (iss.good()) return result;

            if (iss.fail()) {
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
                if (!quiet) throw std::exception();
            }
            return alternative;
        }
    };

    class Operator {
    public:
        Operator() {
        }
        virtual ~Operator() {
        }

        virtual std::string name() const = 0;
        virtual scalar compute(scalar a, scalar b) const = 0;

    };

    /*
     * Fuzzy And
     */
    class Min: public Operator {
        /*
         * Minimum
         */
    public:
        std::string name() const {
            return "MIN";
        }

        scalar compute(scalar a, scalar b) const {
            return Op::Min(a, b);
        }
    };

    class Prod: public Operator {
        /*
         * Product
         */
    public:
        std::string name() const {
            return "PROD";
        }

        scalar compute(scalar a, scalar b) const {
            return a * b;
        }
    };

    class BDif: public Operator {
        /*
         * Bounded Difference
         */
    public:
        std::string name() const {
            return "BDIF";
        }

        scalar compute(scalar a, scalar b) const {
            return Op::Max(0, a + b - 1);
        }
    };

    /*
     * Fuzzy Or
     */
    class Max: public Operator {
        /*
         * Maximum
         */
    public:
        std::string name() const {
            return "MAX";
        }
        scalar compute(scalar a, scalar b) const {
            return Op::Max(a, b);
        }
    };

    class ASum: public Operator {
        /*
         * Algebraic Sum
         */
    public:
        std::string name() const {
            return "ASUM";
        }

        scalar compute(scalar a, scalar b) const {
            return a + b - (a * b);
        }
    };

    class BSum: public Operator {
        /*
         * Algebraic Bounded Sum
         */

    public:
        std::string name() const {
            return "BSUM";
        }

        scalar compute(scalar a, scalar b) const {
            return Op::Min(1, a + b);
        }
    };

    class NSum: public Operator {
        /*
         * Normalized Sum
         */
    public:
        std::string name() const {
            return "NSUM";
        }
        scalar compute(scalar a, scalar b) const {
            return a + b / Op::Max(1, Op::Max(a, b));
        }
    };

} // namespace fl

#endif /* FL_OPERATOR_H_ */
