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

    class Operator {
    public:
        Operator() {
        }
        virtual ~Operator() {
        }

        virtual std::string name() const = 0;
        virtual scalar compute(scalar a, scalar b) const = 0;

        static scalar Min(scalar a, scalar b) {
            return a < b ? a : b;
        }

        static scalar Max(scalar a, scalar b) {
            return a > b ? a : b;
        }

        static bool IsInf(scalar x) {
            return isinf(x);
        }
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
            return Operator::Min(a, b);
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
            return Operator::Max(0, a + b - 1);
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
            return Operator::Max(a, b);
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
            return Operator::Min(1, a + b);
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
            return a + b / Operator::Max(1, Operator::Max(a, b));
        }
    };

} // namespace fl

#endif /* FL_OPERATOR_H_ */
