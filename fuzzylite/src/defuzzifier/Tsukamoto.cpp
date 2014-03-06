// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

#include "fl/defuzzifier/Tsukamoto.h"

#include "fl/term/Activated.h"

#include "fl/term/Ramp.h"
#include "fl/term/Sigmoid.h"
#include "fl/term/SShape.h"
#include "fl/term/ZShape.h"

namespace fl {

    scalar Tsukamoto::tsukamoto(const Activated* term, scalar minimum, scalar maximum) {
        const Term* monotonic = term->getTerm();
        const Ramp* ramp = NULL;
        const Sigmoid* sigmoid = NULL;
        const SShape* sshape = NULL;
        const ZShape* zshape = NULL;
        scalar w = term->getDegree();
        scalar z = fl::nan; //result;
        bool isTsukamoto = true;
        if ((ramp = dynamic_cast<const Ramp*> (monotonic))) {
            z = Op::scale(w, 0, 1, ramp->getStart(), ramp->getEnd());

        } else if ((sigmoid = dynamic_cast<const Sigmoid*> (monotonic))) {
            if (Op::isEq(w, 1.0)) {
                if (Op::isGE(sigmoid->getSlope(), 0.0)) {
                    z = maximum;
                } else {
                    z = minimum;
                }

            } else if (Op::isEq(w, 0.0)) {
                if (Op::isGE(sigmoid->getSlope(), 0.0)) {
                    z = minimum;
                } else {
                    z = maximum;
                }
            } else {
                scalar a = sigmoid->getSlope();
                scalar b = sigmoid->getInflection();
                z = b + (std::log(1.0 / w - 1.0) / -a);
            }

        } else if ((sshape = dynamic_cast<const SShape*> (monotonic))) {
            scalar difference = sshape->getEnd() - sshape->getStart();
            scalar a = sshape->getStart() + std::sqrt(w * difference * difference / 2.0);
            scalar b = sshape->getEnd() + std::sqrt(difference * difference * (w - 1.0) / -2.0);
            if (std::fabs(w - monotonic->membership(a)) <
                    std::fabs(w - monotonic->membership(b))) {
                z = a;
            } else {
                z = b;
            }

        } else if ((zshape = dynamic_cast<const ZShape*> (monotonic))) {
            scalar difference = zshape->getEnd() - zshape->getStart();
            scalar a = zshape->getStart() + std::sqrt(difference * difference * (w - 1.0) / -2.0);
            scalar b = zshape->getEnd() + std::sqrt(w * difference * difference / 2.0);
            if (std::fabs(w - monotonic->membership(a)) <
                    std::fabs(w - monotonic->membership(b))) {
                z = a;
            } else {
                z = b;
            }
        } else {
            isTsukamoto = false;
        }

        if (isTsukamoto) {
            //Compare difference between estimated and true value
            scalar fz = monotonic->membership(z);
            if (not Op::isEq(w, fz, 1e-2)) {
                FL_DBG("[tsukamoto warning] difference <" << Op::str(std::abs(w - fz)) << "> "
                        "might suggest an inaccurate computation of z because it is "
                        "expected w=f(z) in " << monotonic->className() <<
                        " term <" << monotonic->getName() << ">, but "
                        "w=" << term->getDegree() << " "
                        "f(z)=" << fz << " and "
                        "z=" << Op::str(z));
            }
        } else {
            // else fallback to the regular Takagi-Sugeno or inverse Tsukamoto (according to term)
            z = monotonic->membership(term->getDegree());
        }
        return z;
    }

}
