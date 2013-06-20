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

/*
 * Discrete.cpp
 *
 *  Created on: 5/12/2012
 *      Author: jcrada
 */

#include "fl/term/Discrete.h"

#include <cstdarg>

namespace fl {

    Discrete::Discrete(const std::string& name)
    : Term(name) { }

    Discrete::Discrete(const std::string& name,
            const std::vector<scalar>& x,
            const std::vector<scalar>& y)
    : Term(name), x(x), y(y) { }

    Discrete::Discrete(const std::string& name,
            const std::vector<std::pair<scalar, scalar> >& xy)
    : Term(name) {
        for (std::size_t i = 0; i < xy.size(); ++i) {
            x.push_back(xy.at(i).first);
            y.push_back(xy.at(i).second);
        }
    }

    Discrete::~Discrete() { }

    template <typename T>
    Discrete* Discrete::create(const std::string& name, int argc,
            T x1, T y1, ...) throw (fl::Exception) {
        if (argc % 2 != 0 or argc < 2) {
            throw fl::Exception("[discrete term] expected an even number of "
                    "parameters matching (x,y)+, but passed "
                    "<" + fl::Op::str(argc) + "> arguments", FL_AT);
        }
        std::vector<scalar> x, y;
        x.push_back(x1);
        y.push_back(y1);
        va_list args;
        va_start(args, y1);
        bool xNext = true;
        for (int i = 0; i < argc - 2; ++i) {
            if (xNext) x.push_back((scalar) va_arg(args, T));
            else y.push_back((scalar) va_arg(args, T));
            xNext = not xNext;
        }
        va_end(args);
        return new Discrete(name, x, y);
    }

    template FL_EXPORT Discrete* Discrete::create(const std::string& name, int argc,
            scalar x1, scalar y1, ...) throw (fl::Exception);
    template FL_EXPORT Discrete* Discrete::create(const std::string& name, int argc,
            int x1, int y1, ...) throw (fl::Exception);

    std::string Discrete::className() const {
        return "Discrete";
    }

    Discrete* Discrete::copy() const {
        return new Discrete(*this);
    }

    scalar Discrete::membership(scalar _x_) const {
        if (fl::Op::isNan(_x_)) return fl::nan;
        if (x.empty() or y.empty()) return 0.0;
        if (x.size() != y.size()) {
            std::ostringstream ex;
            ex << "[term error] discrete term has vectors x["
                    << x.size() << "] and y[" << y.size() << "] with different sizes";
            throw fl::Exception(ex.str(), FL_AT);
        }

        /*                ______________________
         *               /                      \
         *              /                        \
         * ____________/                          \____________
         *            x[0]                      x[n-1]
         */


        if (fl::Op::isLE(_x_, x.front())) return y.front();
        if (fl::Op::isGE(_x_, x.back())) return y.back();

        int lower = -1, upper = -1;

        for (std::size_t i = 0; i < x.size(); ++i) {
            if (Op::isEq(x.at(i), _x_)) return y.at(i);
            //approximate on the left
            if (Op::isLt(x.at(i), _x_)) {
                lower = i;
            }
            //get the immediate next one on the right
            if (Op::isGt(x.at(i), _x_)) {
                upper = i;
                break;
            }
        }
        if (upper < 0) upper = x.size() - 1;
        if (lower < 0) lower = 0;

        return Op::scale(_x_, x.at(lower), x.at(upper), y.at(lower), y.at(upper));
    }

    std::string Discrete::toString() const {
        std::ostringstream ss;
        ss << className() << " (";
        for (std::size_t i = 0; i < x.size(); ++i) {
            ss << fl::Op::str(x.at(i)) << " " << fl::Op::str(y.at(i));
            if (i < x.size() - 1) ss << ", ";
        }
        ss << ")";
        return ss.str();
    }

}
