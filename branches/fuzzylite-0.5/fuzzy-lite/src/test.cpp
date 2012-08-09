#include "test.h"
#include "FuzzyLite.h"
#include <limits>
namespace fuzzy_lite {

    void Test::main(int args, char** argv) {
        FL_LOG("Error(float): " << std::numeric_limits<float>::epsilon() );
        FL_LOG("Error(double): " << std::numeric_limits<double>::epsilon() );
        DiscreteTerm x("test");
        x.addCoord(0, 0);
        x.addCoord(22.5, 0.9);
        x.addCoord(27.5, 0.9);
        x.addCoord(47.5, 0.1);
        x.addCoord(72.5, 0.1);
        x.addCoord(75, 0);
        flScalar x1, x2, x3, x4, ix;
        flScalar y1, y2, y3, y4, iy;
        for (int i = 2; i < x.numberOfCoords() - 3; ++i) {
            x1 = x.getCoordX(i);
            y1 = x.getCoordY(i);
            x2 = x.getCoordX(i + 1);
            y2 = x.getCoordY(i + 1);

            x3 = 25;
            x4 = 25;
            y3 = 0;
            y4 = 1;
            FL_LOG("L1={(" << x1 << "," << y1 <<")-(" <<x2 <<"," << y2<< ")}");
            FL_LOG("L2={(" << x3 << "," << y3 <<")-(" <<x4 <<"," << y4<< ")}");
            if (DiscreteTerm::Intersects(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy)) {
                FL_LOG("Intersects AT: (" << ix <<", " << iy);
            }
            FL_LOG("FALSE Intersects AT: (" << ix <<", " << iy);
            FL_LOG("-----------------------")
        }
        //    output = {}
        //    src / GuiGrapher.cpp [320] : defussi : 28.3257
    }

}

