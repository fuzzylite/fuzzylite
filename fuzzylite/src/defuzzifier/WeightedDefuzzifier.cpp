#include "fl/defuzzifier/WeightedDefuzzifier.h"

#include "fl/term/Activated.h"
#include "fl/term/Concave.h"
#include "fl/term/Constant.h"
#include "fl/term/Function.h"
#include "fl/term/Linear.h"
#include "fl/term/Ramp.h"
#include "fl/term/Sigmoid.h"
#include "fl/term/SShape.h"
#include "fl/term/ZShape.h"

namespace fl {

    WeightedDefuzzifier::WeightedDefuzzifier(Type type) : _type(type) {

    }

    WeightedDefuzzifier::~WeightedDefuzzifier() {

    }

    void WeightedDefuzzifier::setType(Type type) {
        this->_type = type;
    }

    WeightedDefuzzifier::Type WeightedDefuzzifier::getType() const {
        return this->_type;
    }

    WeightedDefuzzifier::Type WeightedDefuzzifier::inferType(const Term* term) const {
        return (dynamic_cast<const Constant*> (term)
                or dynamic_cast<const Linear*> (term)
                or dynamic_cast<const Function*> (term))
                ? TakagiSugeno : Tsukamoto;
    }

    bool WeightedDefuzzifier::isMonotonic(const Term* term) const {
        return (dynamic_cast<const Concave*> (term)) or
                (dynamic_cast<const Ramp*> (term)) or
                (dynamic_cast<const Sigmoid*> (term)) or
                (dynamic_cast<const SShape*> (term)) or
                (dynamic_cast<const ZShape*> (term));
    }

    /**
     * Instead of computing y=f(x), the goal of Tsukamoto is to find x=f(w), 
     * where f is monotonic.
     */
    scalar WeightedDefuzzifier::tsukamoto(const Term* monotonic, scalar activationDegree,
            scalar minimum, scalar maximum) const {
        scalar w = activationDegree;
        scalar z = fl::nan; //result;
        bool isTsukamoto = true;
        if (const Ramp * ramp = dynamic_cast<const Ramp*> (monotonic)) {
            z = Op::scale(w, 0, 1, ramp->getStart(), ramp->getEnd());

        } else if (const Sigmoid * sigmoid = dynamic_cast<const Sigmoid*> (monotonic)) {
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

        } else if (const SShape * sshape = dynamic_cast<const SShape*> (monotonic)) {
            scalar difference = sshape->getEnd() - sshape->getStart();
            scalar a = sshape->getStart() + std::sqrt(w * difference * difference / 2.0);
            scalar b = sshape->getEnd() + std::sqrt(difference * difference * (w - 1.0) / -2.0);
            if (std::fabs(w - monotonic->membership(a)) <
                    std::fabs(w - monotonic->membership(b))) {
                z = a;
            } else {
                z = b;
            }

        } else if (const ZShape * zshape = dynamic_cast<const ZShape*> (monotonic)) {
            scalar difference = zshape->getEnd() - zshape->getStart();
            scalar a = zshape->getStart() + std::sqrt(difference * difference * (w - 1.0) / -2.0);
            scalar b = zshape->getEnd() + std::sqrt(w * difference * difference / 2.0);
            if (std::fabs(w - monotonic->membership(a)) <
                    std::fabs(w - monotonic->membership(b))) {
                z = a;
            } else {
                z = b;
            }

        } else if (const Concave * concave = dynamic_cast<const Concave*> (monotonic)) {
            scalar i = concave->getInflection();
            scalar e = concave->getEnd();
            z = (i - e) / concave->membership(w) + 2 * e - i;
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
                        "w=" << w << " "
                        "f(z)=" << fz << " and "
                        "z=" << Op::str(z));
            }
        } else {
            // else fallback to the regular Takagi-Sugeno or inverse Tsukamoto (according to term)
            z = monotonic->membership(w);
        }
        return z;
    }


}
