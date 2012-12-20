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

#include "fl/Exception.h"

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>
#include <limits>
#include <utility>

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

        static scalar Scalar(const std::string& x, bool quiet = false,
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
    class Min : public Operator {
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

    class Prod : public Operator {
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

    class BDif : public Operator {
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

    class DProd : public Operator {
        /*
         * Drastic product
         */

    public:

        std::string name() const {
            return "DPROD";
        }

        scalar compute(scalar a, scalar b) const {
            if (Op::IsEq(Op::Max(a, b), 1.0)) {
                return Op::Min(a, b);
            }
            return 0.0;
        }
    };

    class EProd : public Operator {
        /*
         * Einstein product
         */

    public:

        std::string name() const {
            return "EPROD";
        }

        scalar compute(scalar a, scalar b) const {
            return (a * b) / (2 - (a + b - a * b));
        }
    };

    class HProd : public Operator {
        /*
         * Hamacher product
         */

    public:

        std::string name() const {
            return "HPROD";
        }

        scalar compute(scalar a, scalar b) const {
            return (a * b) / (a + b - a * b);
        }
    };

    /*
     * Fuzzy Or
     */
    class Max : public Operator {
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

    class ASum : public Operator {
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

    class BSum : public Operator {
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

    class NSum : public Operator {
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

    class DSum : public Operator {
        /*
         * Drastic Sum
         */
    public:

        std::string name() const {
            return "DSUM";
        }

        scalar compute(scalar a, scalar b) const {
            if (Op::IsEq(Op::Min(a, b), 0.0)) {
                return Op::Max(a, b);
            }
            return 1.0;
        }
    };

    class ESum : public Operator {
        /*
         * Einstein Sum
         */
    public:

        std::string name() const {
            return "ESUM";
        }

        scalar compute(scalar a, scalar b) const {
            return (a + b) / (1 + a * b);
        }
    };

    class HSum : public Operator {
        /*
         * Hamacher Sum
         */
    public:

        std::string name() const {
            return "HSUM";
        }

        scalar compute(scalar a, scalar b) const {
            return (a + b - 2 * a * b) / (1 - a * b);
        }
    };

} // namespace fl

#endif /* FL_OPERATOR_H_ */
