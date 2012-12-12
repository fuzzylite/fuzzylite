/*
 * Discrete.cpp
 *
 *  Created on: 5/12/2012
 *      Author: jcrada
 */

#include "fl/term/Discrete.h"

#include "fl/engine/Operator.h"

#include <sstream>

namespace fl {

    Discrete::Discrete(const std::string& name)
            : Term(name) {
    }

    Discrete::Discrete(const std::string& name,
            const std::vector<scalar>& x,
            const std::vector<scalar>& y)
            : Term(name), x(x), y(y) {
    }
    Discrete::Discrete(const std::string& name,
            const std::vector<std::pair<scalar, scalar> >& xy)
            : Term(name) {
        for (std::size_t i = 0; i < xy.size(); ++i) {
            x.push_back(xy[i].first);
            y.push_back(xy[i].second);
        }
    }

    Discrete::~Discrete() {
    }

    scalar Discrete::minimum() const {
        if (x.size() > 0) return x[0];
        return -std::numeric_limits<scalar>::infinity();
    }

    scalar Discrete::maximum() const {
        if (x.size() > 0) return x[x.size() - 1];
        return std::numeric_limits<scalar>::infinity();
    }

    scalar Discrete::membership(scalar mu) const {
        scalar lowerApprox = -std::numeric_limits<scalar>::infinity();
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
                upper = i;
                break;
            }
        }
//        return ((y[upper] - y[lower]) / (x[upper] - x[lower])) *
//                (mu - x[lower]) + y[lower];
        return Op::Scale(mu, x[lower], x[upper], y[lower], y[upper]);
    }

    std::string Discrete::toString() const {
        std::stringstream ss;
        ss << "Discrete (";
        for (std::size_t i = 0; i < x.size(); ++i) {
            ss << x[i] << " " << y[i];
            if (i < x.size() - 1) ss << ", ";
        }
        ss << ")";
        return ss.str();
    }

} /* namespace fl */
