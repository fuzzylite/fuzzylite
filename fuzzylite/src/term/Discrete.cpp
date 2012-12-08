/*
 * Discrete.cpp
 *
 *  Created on: 5/12/2012
 *      Author: jcrada
 */

#include "Discrete.h"

#include "../engine/Operator.h"

#include <sstream>

namespace fl {

    Discrete::Discrete(const std::string& name,
            const std::vector<scalar>& x,
            const std::vector<scalar>& y)
            : Term(name), x(x), y(y) {
    }

    Discrete::~Discrete() {
    }

    scalar Discrete::minimum() const {
        return x[0];
    }

    scalar Discrete::maximum() const {
        return x[x.size() - 1];
    }

    scalar Discrete::membership(scalar mu) const {
        scalar lowerApprox = -std::numeric_limits<scalar>::infinity();
        scalar upperApprox = std::numeric_limits<scalar>::infinity();
        int lower = -1, upper = -1;

        for (std::size_t i = 0; i < x.size(); ++i) {
            if (Op::IsEq(x[i], mu)) return y[i];
            //approximate on the left
            if (Op::IsLt(x[i], mu) and Op::IsGt(x[i], lowerApprox)) {
                lowerApprox = x[i];
                lower = i;
            }
            //get the immediate next one on the right
            if (Op::IsGt(x[i], mu)) {
                upperApprox = x[i];
                upper = i;
                break;
            }
        }
        return ((y[upper] - y[lower]) / (x[upper] - x[lower])) *
                (mu - x[lower]) + y[lower];
    }

    std::string Discrete::toString() const {
        std::stringstream ss;
        ss << "Discrete (";
        for (std::size_t i = 0; i < x.size(); ++i) {
            ss << x[i] << ":" << y[i];
            if (i < x.size() - 1) ss << ", ";
        }
        ss << ")";
        return ss.str();
    }

} /* namespace fl */
