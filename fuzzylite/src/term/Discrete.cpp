/*
 * Discrete.cpp
 *
 *  Created on: 5/12/2012
 *      Author: jcrada
 */

#include "fl/term/Discrete.h"

#include "fl/operator/Operator.h"

#include <sstream>
#include <cstdarg>
namespace fl {

    Discrete::Discrete(const std::string& name)
    : Term(name) { }

//    Discrete::Discrete(const std::string& name, int argc, ...) throw (fl::Exception)
//    : Term(name) {
//        if (argc % 2 != 0) {
//            throw fl::Exception("[discrete term] constructor expected even number "
//                    "of variable arguments (x,y)*, but passed <" + fl::Op::str((scalar) argc, 0) + "> arguments", FL_AT);
//        }
//        va_list args;
//        va_start(args, argc);
//        bool xTurn = true;
//        for (int i = 0; i < argc; ++i) {
//            if (xTurn) x.push_back(va_arg(args, scalar));
//            else y.push_back(va_arg(args, scalar));
//            xTurn = not xTurn;
//        }
//        va_end(args);
//    }

    Discrete::Discrete(const std::string& name,
            const std::vector<scalar>& x,
            const std::vector<scalar>& y)
    : Term(name), x(x), y(y) { }

    Discrete::Discrete(const std::string& name,
            const std::vector<std::pair<scalar, scalar> >& xy)
    : Term(name) {
        for (std::size_t i = 0; i < xy.size(); ++i) {
            x.push_back(xy[i].first);
            y.push_back(xy[i].second);
        }
    }

    Discrete::~Discrete() { }

    std::string Discrete::className() const {
        return "Discrete";
    }

    Discrete* Discrete::copy() const {
        return new Discrete(*this);
    }

    scalar Discrete::membership(scalar mu) const {
        scalar lowerApprox = -std::numeric_limits<scalar>::infinity();
        int lower = -1, upper = -1;

        for (std::size_t i = 0; i < x.size(); ++i) {
            if (Op::isEq(x[i], mu)) return y[i];
            //approximate on the left
            if (Op::isLt(x[i], mu) and Op::isGt(x[i], lowerApprox)) {
                lowerApprox = x[i];
                lower = i;
            }
            //get the immediate next one on the right
            if (Op::isGt(x[i], mu)) {
                upper = i;
                break;
            }
        }
        //        return ((y[upper] - y[lower]) / (x[upper] - x[lower])) *
        //                (mu - x[lower]) + y[lower];
        return Op::scale(mu, x[lower], x[upper], y[lower], y[upper]);
    }

    std::string Discrete::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (";
        for (std::size_t i = 0; i < x.size(); ++i) {
            ss << x[i] << " " << y[i];
            if (i < x.size() - 1) ss << ", ";
        }
        ss << ")";
        return ss.str();
    }

}
