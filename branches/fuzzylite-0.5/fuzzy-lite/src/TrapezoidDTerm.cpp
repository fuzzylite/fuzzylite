#include "TrapezoidDTerm.h"

namespace fuzzy_lite {

    TrapezoidDTerm::TrapezoidDTerm(flScalar a, flScalar b, flScalar c, flScalar d) :
    DiscreteTerm() {
        addCoord(a, 0.0);
        addCoord(b, 1.0);
        addCoord(c, 1.0);
        addCoord(d, 0.0);
    }

    TrapezoidDTerm::TrapezoidDTerm(const std::string& name, flScalar a, flScalar b, flScalar c, flScalar d) :
    DiscreteTerm(name) {
        addCoord(a, 0.0);
        addCoord(b, 1.0);
        addCoord(c, 1.0);
        addCoord(d, 0.0);
    }

    TrapezoidDTerm::TrapezoidDTerm(const std::string& name, flScalar a, flScalar d, flScalar simmetry) {
        addCoord(a, 0.0);
        addCoord((a + d) * simmetry, 1.0);
        addCoord((a + d) * (1 - simmetry), 1.0);
        addCoord(d, 0.0);
    }

    TrapezoidDTerm::~TrapezoidDTerm() {
    }

}
