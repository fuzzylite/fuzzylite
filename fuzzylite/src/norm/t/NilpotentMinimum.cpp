
#include "fl/norm/t/NilpotentMinimum.h"

namespace fl {

    std::string NilpotentMinimum::className() const {
        return "NilpotentMinimum";
    }

    scalar NilpotentMinimum::compute(scalar a, scalar b) const {
        if (Op::isGt(a + b, 1.0)) {
            return Op::min(a, b);
        }
        return 0;
    }

    TNorm* NilpotentMinimum::constructor() {
        return new NilpotentMinimum;
    }


}

