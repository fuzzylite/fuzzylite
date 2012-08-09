#include "TriangularDTerm.h"

namespace fuzzy_lite {

    TriangularDTerm::TriangularDTerm(flScalar a, flScalar b, flScalar c) :
    DiscreteTerm() {
        addCoord(a, flScalar(0.0));
        addCoord(b, flScalar(1.0));
        addCoord(c, flScalar(0.0));
    };

    TriangularDTerm::TriangularDTerm(const std::string& name, flScalar a, flScalar c) :
    DiscreteTerm(name) {
        addCoord(a, flScalar(0.0));
        addCoord((c + a) / 2, flScalar(1.0));
        addCoord(c, flScalar(0.0));
    };

    TriangularDTerm::TriangularDTerm(const std::string& name, flScalar a, flScalar b, flScalar c) :
    DiscreteTerm(name) {
        addCoord(a, flScalar(0.0));
        addCoord(b, flScalar(1.0));
        addCoord(c, flScalar(0.0));
    };

    TriangularDTerm::~TriangularDTerm() {
    }

}