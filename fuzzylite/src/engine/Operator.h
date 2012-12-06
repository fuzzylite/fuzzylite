/*
 * Operator.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_OPERATOR_H_
#define FL_OPERATOR_H_

#include "../scalar.h"

#include <string>
#include <algorithm>
#include <cmath>

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
            return !IsEq(a, b, tolerance) && a < b;
        }

        //Is less than
        static bool IsLE(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return IsEq(a, b, tolerance) || a < b;
        }

        //Is equal
        static bool IsEq(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return std::fabs(a - b) < FL_EPSILON;
        }

        //Is greater than
        static bool IsGt(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return !IsEq(a, b, tolerance) && a > b;
        }

        static bool IsGE(scalar a, scalar b, scalar tolerance = FL_EPSILON) {
            return IsEq(a, b, tolerance) || a > b;
        }

        static int FindReplace(std::string& str, const std::string& find,
                const std::string& replace, bool all) {
            if (find.length() == 0) return 0;
            int result = 0;
            size_t index = -abs(find.length() - replace.length());
            do {
                index = str.find(find, index + abs(find.length() - replace.length()));
                if (index != std::string::npos) {
                    str.replace(index, find.length(), replace);
                    ++result;
                }
            } while (all && index != std::string::npos);
            return result;
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
