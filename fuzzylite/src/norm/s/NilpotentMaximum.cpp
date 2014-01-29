
#include "fl/norm/s/NilpotentMaximum.h"



namespace fl {

    std::string NilpotentMaximum::className() const {
        return "NilpotentMaximum";
    }

    scalar NilpotentMaximum::compute(scalar a, scalar b) const {
        if (Op::isLt(a + b, 1.0)) {
            return std::max(a, b);
        }
        return 1.0;
    }

    SNorm* NilpotentMaximum::constructor() {
        return new NilpotentMaximum;
    }

}

