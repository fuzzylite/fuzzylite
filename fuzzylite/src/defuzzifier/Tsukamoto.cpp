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

#include "fl/defuzzifier/Tsukamoto.h"

#include "fl/term/Thresholded.h"

#include "fl/term/Ramp.h"
#include "fl/term/Sigmoid.h"
#include "fl/term/SShape.h"
#include "fl/term/ZShape.h"

namespace fl {

    scalar Tsukamoto::tsukamoto(const Thresholded* term, scalar minimum, scalar maximum) {
        const Term* monotonic = term->getTerm();
        const Ramp* ramp = NULL;
        const Sigmoid* sigmoid = NULL;
        const SShape* sshape = NULL;
        const ZShape* zshape = NULL;
        scalar w = term->getThreshold();
        scalar z = fl::nan; //result;
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
        }
        
        if (not Op::isNan(z)) {
            //Compare difference between estimated and true value
            scalar fz = monotonic->membership(z);
            if (not Op::isEq(w, fz, 1e-2)) {
                FL_DBG("[tsukamoto warning] difference <" << Op::str(std::abs(w-fz)) << "> "
                        "might suggest an inaccurate computation of z because it is "
                        "expected w=f(z) in " << monotonic->className() <<
                        " term <" << monotonic->getName() << ">, but "
                        "w=" << term->getThreshold() << " "
                        "f(z)=" << fz << " and "
                        "z=" << Op::str(z));
            }
        } else {
//            else if it is not a Tsukamoto controller, then fallback to the inverse Tsukamoto
            z = monotonic->membership(term->getThreshold());
        }
        return z;
    }

}
