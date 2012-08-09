#include "DiscreteTerm.h"

#include "defs.h"
#include "FuzzyOperator.h"
#include "TriangularDTerm.h"
#include "TrapezoidDTerm.h"
#include "StringOperator.h"
#include "FuzzyExceptions.h"
#include "FuzzyAction.h"

#include <sstream>
#include <math.h>

namespace fuzzy_lite {

    DiscreteTerm::DiscreteTerm() : _from_infinity(false), _to_infinity(false),
    _intersect_x(NAN), _intersect_y(NAN) {

    }

    DiscreteTerm::DiscreteTerm(const std::string& name) : _name(name),
    _from_infinity(false), _to_infinity(false), _intersect_x(NAN), _intersect_y(NAN) {

    }

    DiscreteTerm::~DiscreteTerm() {
    }

    void DiscreteTerm::setName(const std::string& name) {
        this->_name = name;
    }

    std::string DiscreteTerm::getName() const {
        return this->_name;
    }

    void DiscreteTerm::addCoord(flScalar x, flScalar y) {
        _x.push_back(x);
        _y.push_back(y);
    }

    void DiscreteTerm::removeCoord(int index) throw (OutOfRangeException) {
        OutOfRangeException::CheckArray(FL_AT, index, numberOfCoords());
        _x.erase(_x.begin() + index);
        _y.erase(_y.begin() + index);
    }

    void DiscreteTerm::changeCoord(int index, flScalar x, flScalar y) throw (OutOfRangeException) {
        OutOfRangeException::CheckArray(FL_AT, index, numberOfCoords());
        _x[index] = x;
        _y[index] = y;
    }

    void DiscreteTerm::insertCoord(int index, flScalar x, flScalar y) throw (OutOfRangeException) {
        OutOfRangeException::CheckArray(FL_AT, index, numberOfCoords());
        _x.insert(_x.begin() + index, x);
        _y.insert(_y.begin() + index, y);
    }

    flScalar DiscreteTerm::getCoordX(int index) const throw (OutOfRangeException) {
        OutOfRangeException::CheckArray(FL_AT, index, numberOfCoords());
        return _x[index];
    }

    flScalar DiscreteTerm::getCoordY(int index) const throw (OutOfRangeException) {
        OutOfRangeException::CheckArray(FL_AT, index, numberOfCoords());
        return _y[index];
    }

    void DiscreteTerm::copyTo(DiscreteTerm& term) const {
        term.clear();
        for (int i = 0; i < numberOfCoords(); ++i) {
            term.addCoord(_x[i], _y[i]);
        }
        term.setFromInfinity(isFromInfinity());
        term.setToInfinity(isToInfinity());
    }

    FL_INLINE void DiscreteTerm::clear() {
        _x.clear();
        _y.clear();
    }

    FL_INLINE int DiscreteTerm::numberOfCoords() const {
        return _x.size();
    }

    FL_INLINE bool DiscreteTerm::isEmpty() const {
        return !(_x.size() > 0 && _y.size() > 0);
    }

    FL_INLINE void DiscreteTerm::intersectionOnAggregate(flScalar& ix, flScalar& iy) const {
        ix = _intersect_x;
        iy = _intersect_y;
    }

    FL_INLINE flScalar DiscreteTerm::minimum() const {
        return _x.size() > 0 ? _x[0] : NAN;
    }

    FL_INLINE flScalar DiscreteTerm::maximum() const {
        return _x.size() > 0 ? _x[_x.size() - 1] : NAN;
    }

    FL_INLINE void DiscreteTerm::setFromInfinity(bool from_infinity) {
        _from_infinity = from_infinity;
    }

    FL_INLINE bool DiscreteTerm::isFromInfinity() const {
        return _from_infinity;
    }

    FL_INLINE void DiscreteTerm::setToInfinity(bool to_infinity) {
        _to_infinity = to_infinity;
    }

    FL_INLINE bool DiscreteTerm::isToInfinity() const {
        return _to_infinity;
    }
    //
    //    flScalar DiscreteTerm::membership(flScalar crisp) const {
    //        if (isEmpty()) {
    //            FL_LOGW("Term <" + getName() + "> is EMPTY");
    //            return flScalar(0.0);
    //        }
    //        if (isFromInfinity() && crisp < _x[0]) {
    //            return flScalar(1.0);
    //        }
    //        if (isToInfinity() && crisp > _x[_x.size() - 1]) {
    //            return flScalar(1.0);
    //        }
    //
    //        if (isinf(crisp)) {
    //            if (isFromInfinity() && signbit(crisp) == 1) {
    //                return flScalar(1.0);
    //            }
    //            if (isToInfinity() && signbit(crisp) == 0) {
    //                return flScalar(1.0);
    //            }
    //            return flScalar(0.0);
    //        }
    //        flScalar numerator_x, numerator_y, denominator;
    //        flScalar result = flScalar(0.0);
    //        flScalar x, x1, y1, x2, y2; //term lines;
    //        flScalar y, x3, y3, x4, y4; //vertical intersection;
    //        x3 = crisp;
    //        y3 = flScalar(0.0);
    //        x4 = crisp;
    //        y4 = flScalar(1.0);
    //        int index;
    //        for (index = 0; index < numberOfCoords() - 1; ++index) {
    //            x1 = _x[index];
    //            y1 = _y[index];
    //            x2 = _x[index + 1];
    //            y2 = _y[index + 1];
    //
    //            denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    //            if (denominator == flScalar(0.0)) {
    //                continue;
    //            }
    //            numerator_x = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2)*(x3 * y4 - y3 * x4);
    //            numerator_y = (x1 * y2 - y1 * x2)*(y3 - y4) - (y1 - y2)*(x3 * y4 - y3 * x4);
    //            x = numerator_x / denominator;
    //            y = numerator_y / denominator;
    //            if ( y < flScalar(0.0) || y > flScalar(1.0)) {
    //                continue;
    //            }
    //            result = y;
    //            break;
    //        }
    //        return result;
    //    }

    flScalar DiscreteTerm::membership(flScalar crisp) const {
        if (isEmpty()) {
            FL_LOGW("Term <" + getName() + "> is EMPTY");
            return flScalar(0.0);
        }
        if (isFromInfinity() && crisp < _x[0]) {
            return flScalar(1.0);
        }
        if (isToInfinity() && crisp > _x[_x.size() - 1]) {
            return flScalar(1.0);
        }

        if (isinf(crisp)) {
            if (isFromInfinity() && signbit(crisp) == 1) {
                return flScalar(1.0);
            }
            if (isToInfinity() && signbit(crisp) == 0) {
                return flScalar(1.0);
            }
            return flScalar(0.0);
        }
        flScalar ix, x1, y1, x2, y2; //term lines;
        flScalar iy, x3, y3, x4, y4; //vertical intersection;
        x3 = crisp;
        y3 = flScalar(0.0);
        x4 = crisp;
        y4 = flScalar(1.0);
        int index;
        for (index = 0; index < numberOfCoords() - 1; ++index) {
            x1 = _x[index];
            y1 = _y[index];
            x2 = _x[index + 1];
            y2 = _y[index + 1];
            if (Intersects(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy)) {
                return iy;
            }
        }
        return flScalar(0.0);
    }

    flScalar DiscreteTerm::area() const {
        if (isEmpty()) {
            FL_LOGW("Term <" + getName() + "> is EMPTY");
            return flScalar(0.0);
        }
        //This method creates several triangles from the coordinate space.
        flScalar result = flScalar(0.0);
        flScalar tmp = flScalar(0.0);
        int pi = 0;
        flScalar x[3];
        flScalar y[3];
        bool y_is_0 = true;

        x[0] = _x[0];
        y[0] = _y[0];
        x[1] = _x[1];
        y[1] = _y[1];
        pi = 2;

        //Triangulation:
        int i = 1;
        while (pi < numberOfCoords()) {
            ++i;
            if (y_is_0) {
                x[i % 3] = x [(i - 1) % 3 ];
                y[i % 3] = 0;
            } else {
                x[i % 3] = _x [pi];
                y[i % 3] = _y[pi];
                ++pi;
            }

            y_is_0 = !y_is_0;

            //Area of triangle:
            // |Ax(By - Cy) + Bx(Cy - Ay) + Cx(Ay - By)| / 2
            // |x0(y1 - y2) + x1(y2 - y0) + x2(y0 - y1)| / 2

            tmp = (x[0] * (y[1] - y[2]) + x[1] * (y[2] - y[0]) + x[2] * (y[0] - y[1])) / 2;
            //            FL_DEBUG("area: " << FuzzyOperator::Absolute(tmp));
            result += FuzzyOperator::Absolute(tmp);
        }
        return result;
    }

    void DiscreteTerm::centroid(flScalar& centroid_x, flScalar& centroid_y) const {
        if (isEmpty()) {
            FL_LOGW("Term <" + getName() + "> is EMPTY");
            centroid_x = NAN;
            centroid_y = NAN;
            return;
        }
        flScalar sum_area = flScalar(0.0);
        flScalar sum_a_x = flScalar(0.0);
        flScalar sum_a_y = flScalar(0.0);

        flScalar tmp_area = flScalar(0.0);

        int pi = 0;
        flScalar x[3];
        flScalar y[3];
        bool y_is_0 = true;
        x[0] = _x[0];
        y[0] = _y[0];
        x[1] = _x[1];
        y[1] = _y[1];

        pi = 2;
        int i = 1;
        //Triangulation:
        while (pi < numberOfCoords()) {
            ++i;
            if (y_is_0) {
                x[i % 3] = x [(i - 1) % 3 ];
                y[i % 3] = 0;
            } else {
                x[i % 3] = _x [pi];
                y[i % 3] = _y[pi];
                ++pi;
            }

            y_is_0 = !y_is_0;

            //Area of triangle:
            // |Ax(By - Cy) + Bx(Cy - Ay) + Cx(Ay - By)| / 2
            // |x0(y1 - y2) + x1(y2 - y0) + x2(y0 - y1)| / 2
            tmp_area = FuzzyOperator::Absolute(
                    x[0] * (y[1] - y[2]) +
                    x[1] * (y[2] - y[0]) +
                    x[2] * (y[0] - y[1])) / 2;
            //Centroid of a triangle:
            // x = (x0 + x1 + x2)/3; y = (y0 + y1 + y2)/3;
            sum_a_x += tmp_area * (x[0] + x[1] + x[2]) / 3;
            sum_a_y += tmp_area * (y[0] + y[1] + y[2]) / 3;

            sum_area += tmp_area;
        }
        centroid_x = sum_a_x / sum_area;
        centroid_y = sum_a_y / sum_area;
    }

    bool DiscreteTerm::Intersects(
            flScalar x1, flScalar y1, flScalar x2, flScalar y2,
            flScalar x3, flScalar y3, flScalar x4, flScalar y4,
            flScalar& ix, flScalar& iy) {
        flScalar denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if (denominator == flScalar(0.0)) {
            return false;
        }
        flScalar numerator_x = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2)*(x3 * y4 - y3 * x4);
        flScalar numerator_y = (x1 * y2 - y1 * x2)*(y3 - y4) - (y1 - y2)*(x3 * y4 - y3 * x4);
        ix = numerator_x / denominator;
        iy = numerator_y / denominator;
        bool satisfies_l1x =
                FuzzyOperator::isGEq(ix, FuzzyOperator::Min(x1, x2)) &&
                FuzzyOperator::isLEq(ix, FuzzyOperator::Max(x1, x2));
        bool satisfies_l1y =
                FuzzyOperator::isGEq(iy, FuzzyOperator::Min(y1, y2)) &&
                FuzzyOperator::isLEq(iy, FuzzyOperator::Max(y1, y2));

        bool satisfies_l2x =
                FuzzyOperator::isGEq(ix, FuzzyOperator::Min(x3, x4)) &&
                FuzzyOperator::isLEq(ix, FuzzyOperator::Max(x3, x4));
        bool satisfies_l2y =
                FuzzyOperator::isGEq(iy, FuzzyOperator::Min(y3, y4)) &&
                FuzzyOperator::isLEq(iy, FuzzyOperator::Max(y3, y4));
//        FL_LOG("L1 satisfies:" << (satisfies_l1x ? " x" : "") << (satisfies_l1y ? " y" : ""));
//        FL_LOG("L2 satisfies:" << (satisfies_l2x ? " x" : "") << (satisfies_l2y ? " y" : ""));
        return satisfies_l1x && satisfies_l1y && satisfies_l2x && satisfies_l2y;
    }

    void DiscreteTerm::aggregate(const DiscreteTerm& term) {
        if (term.isEmpty()) {
            return;
        }
        if (isEmpty()) {
            term.copyTo(*this);
            return;
        }
        if (term.isFromInfinity()) {
            setFromInfinity(true);
        }
        if (term.isToInfinity()) {
            setToInfinity(true);
        }

        const DiscreteTerm * lo;
        const DiscreteTerm* hi;

        if (minimum() <= term.minimum()) {
            lo = this;
            hi = &term;
        } else {
            lo = &term;
            hi = this;
        }

        DiscreteTerm result(getName());
        flScalar x1, y1, x2, y2; //lo-line;
        flScalar x3, y3, x4, y4; //hi-line;
        flScalar ix, iy;

        int lo_index, hi_index = 0;
        bool intersect;
        result.addCoord(lo->getCoordX(0), lo->getCoordY(0));
        for (lo_index = 0; lo_index < lo->numberOfCoords() - 1; ++lo_index) {
            //            FL_LOG("-------------------------------------");
            intersect = false;
            x1 = lo->_x[lo_index];
            y1 = lo->_y[lo_index];
            x2 = lo->_x[lo_index + 1];
            y2 = lo->_y[lo_index + 1];
            for (int i = hi_index; i < hi->numberOfCoords() - 1; ++i) {
                x3 = hi->_x[i];
                y3 = hi->_y[i];
                x4 = hi->_x[i + 1];
                y4 = hi->_y[i + 1];
                if (Intersects(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy) && ix > x1) { //ix > x1 to avoid changing in a point to point intersection.
                    //                    FL_LOG(lo->getName() << " L" << lo_index << "=(" << x1 << "," << y1 << ")-(" << x2 << "," << y2 << ")")
                    //                    FL_LOG(hi->getName() << "L" << i << "=(" << x3 << "," << y3 << ")-(" << x4 << "," << y4 << ")");
                    //                    FL_LOG("AT: (" << ix << ", " << iy << ")");
                    _intersect_x = ix;
                    _intersect_y = iy;
                    intersect = true;
                    hi_index = i;
                    break;
                }
            }
            if (!intersect) {
                result.addCoord(x2, y2);
            } else {
                result.addCoord(ix, iy);
                result.addCoord(hi->getCoordX(hi_index + 1), hi->getCoordY(hi_index + 1));
                const DiscreteTerm* tmp = lo;
                lo = hi;
                hi = tmp;

                int tmp_index = lo_index;
                lo_index = hi_index;
                hi_index = tmp_index;
            }

        }
        result.copyTo(*this);
        optimize();
    }

    FL_INLINE void DiscreteTerm::modulate(flScalar degree, const FuzzyOperator& fuzzy_op, flScalar prob) {
        if (isEmpty()) {
            FL_LOGW("Term <" + getName() + "> is EMPTY");
            return;
        }
        flScalar numerator_x, numerator_y, denominator;
        flScalar x1, y1, x2, y2; //term lines;
        flScalar x3, y3, x4, y4; //horizontal intersection;
        flScalar x, y; //intersection
        y3 = degree;
        y4 = degree;

        std::vector<flScalar> new_x, new_y;
        for (int i = 0; i < numberOfCoords() - 1; ++i) {
            x1 = _x[i];
            y1 = _y[i];
            x2 = _x[i + 1];
            y2 = _y[i + 1];
            x3 = x1 - 1; //to ensure intersection
            x4 = x2 + 1; //to ensure intersection

            if (y1 <= degree && y2 <= degree) { //no intersection
                new_x.push_back(x1);
                new_y.push_back(y1);
                new_x.push_back(x2);
                new_y.push_back(y2);
                continue;
            }

            denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            if (denominator == flScalar(0.0)) { //parallel lines.
                FL_DEBUG("Parallel: x1=" << x1 << ", x2=" << x2 << ",y=" << degree);
                new_x.push_back(x1);
                new_y.push_back(degree);
                new_x.push_back(x2);
                new_y.push_back(degree);
                continue;
            }
            numerator_x = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2)*(x3 * y4 - y3 * x4);
            numerator_y = (x1 * y2 - y1 * x2)*(y3 - y4) - (y1 - y2)*(x3 * y4 - y3 * x4);
            x = numerator_x / denominator;
            y = numerator_y / denominator;
            //            FL_DEBUG << "x=" << x << "; y=" << y << std::endl;
            //            FL_DEBUG << "x1=" << x1 << ", x2=" << x2 << std::endl;
            //            FL_DEBUG << "y1=" << y1 << ", y2=" << y2 << std::endl;
            if (y1 > degree) {
                new_x.push_back(x);
                new_y.push_back(fuzzy_op.modulate(degree, y1, 1.0));
            } else {
                new_x.push_back(x1);
                new_y.push_back(y1);
            }
            if (y2 > degree) {
                new_x.push_back(x);
                new_y.push_back(fuzzy_op.modulate(degree, y2, 1.0));
            } else {
                new_x.push_back(x2);
                new_y.push_back(y2);
            }
        }
        _x = new_x;
        _y = new_y;

        optimize();

    }

    //    FL_INLINE void DiscreteTerm::modulate(flScalar degree, const FuzzyOperator& fuzzy_op, flScalar prob) {
    //        if (isEmpty()) {
    //            FL_LOGW("Term <" + getName() + "> is EMPTY");
    //            return;
    //        }
    //        flScalar x1, y1, x2, y2; //term lines;
    //        flScalar x3, y3, x4, y4; //horizontal intersection;
    //        flScalar ix, iy; //intersections;
    //        std::vector<flScalar> new_x, new_y;
    //        for (int i = 0; i < numberOfCoords() - 1; ++i) {
    //            x1 = _x[i];
    //            y1 = _y[i];
    //            x2 = _x[i + 1];
    //            y2 = _y[i + 1];
    //            x3 = x1 - 1; //to ensure intersection
    //            y3 = degree;
    //            x4 = x2 + 1; //to ensure intersection
    //            y4 = degree;
    //
    //            if (Intersects(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy)) {
    //                new_x.push_back(ix);
    //                new_y.push_back(fuzzy_op.modulate(degree, iy, 1.0));
    //            } else {
    //                new_x.push_back(x1);
    //                new_y.push_back(y1);
    //                new_x.push_back(x2);
    //                new_y.push_back(y2);
    //            }
    //        }
    //        _x = new_x;
    //        _y = new_y;
    //        optimize();
    //    }

    void DiscreteTerm::optimize() {
        if (numberOfCoords() <= 1) {
            return;
        }
        std::vector<int> removal;
        for (int i = 1; i < numberOfCoords(); ++i) {
            if (FuzzyOperator::isEq(_x[i - 1], _x[i]) && FuzzyOperator::isEq(_y[i - 1], _y[i])) {
                removal.push_back(i);
            }
        }

        for (int i = removal.size() - 1; i >= 0; --i) {
            _x.erase(_x.begin() + removal[i]);
            _y.erase(_y.begin() + removal[i]);
        }
    }

    std::string DiscreteTerm::toString() const {
        std::stringstream result;
        result << getName() << "={ ";
        for (int i = 0; i < numberOfCoords(); ++i) {
            result << "(" << _x[i] << "," << _y[i] << ") ";
        }
        result << "}";
        return result.str();
    }

    DiscreteTerm * DiscreteTerm::Discretize(const ContinuousTerm & term) {
        DiscreteTerm* result = new DiscreteTerm(term.getName());
        int step = (int) (FuzzyOperator::Absolute(term.getMinimumValue()) +
                FuzzyOperator::Absolute(term.getMaximumValue())) / term.getResolution();
        for (int i = term.getMinimumValue(); i <= term.getMaximumValue(); i += step) {
            result->addCoord(i, term.degreeOfMembership(i));
        }
        return result;
    }

    void DiscreteTerm::test0() {

        TriangularDTerm a("a", 0, 1);
        FL_DEBUG(a.toString());
        FL_DEBUG("area: " << a.area());
        flScalar x, y;
        a.centroid(x, y);
        FL_DEBUG("centroid: " << x << "," << y);
        flScalar step = 0.2;
        for (flScalar i = 0; i < 1 + step; i += step) {
            FL_DEBUG("degree (" << i << "): " << a.membership(i));
        }

        TriangularDTerm b("b", 0.5, 1.5);
        FL_DEBUG(b.toString());
        FL_DEBUG("area: " << b.area());
        b.centroid(x, y);
        FL_DEBUG("centroid: " << x << "," << y);
        step = 0.25;
        for (flScalar i = 0.0; i < 1.5 + step; i += step) {
            FL_DEBUG("degree (" << i << "): " << b.membership(i));
        }

        DiscreteTerm b_ = b;
        b_.aggregate(a);
        FL_DEBUG(b_.toString());

        //        FL_DEBUG << "A" << std::endl;
        //        FL_DEBUG << "a_area : " << a.area() << std::endl;
        //        FL_DEBUG << "B" << std::endl;
        //        FL_DEBUG << "b_area : " << b.area() << std::endl;
        FL_DEBUG("R");
        FL_DEBUG("res_area : " << b_.area());
        //        b_.centroid(x,y);
        //        FL_DEBUG << "centroid: " << x << ", " << y << std::endl;

        //        DiscreteTerm c;
        //        c.addCoord(0,0);
        //        c.addCoord(0,1);
        //        c.addCoord(1,1);
        //        c.addCoord(1,0);
        //        FL_DEBUG << "area: " << c.area() << std::endl;
    }

    void DiscreteTerm::test1() {
        FuzzyOperator* op = new FuzzyOperator();
        op->setConsequentModulation(FuzzyOperator::CM_SCALE);
        DiscreteTerm t("t");
        t.addCoord(0, 0);
        t.addCoord(0, 0);
        t.addCoord(0.5, 1);
        t.addCoord(0.5, 1);
        t.addCoord(1, 0);
        t.addCoord(1, 0);
        t.optimize();
        FL_DEBUG(t.toString());
        //        FL_DEBUG << "area: " << t.area() << std::endl;
        //        flScalar x, y;
        //        t.centroid(x, y);
        //        FL_DEBUG << "centroid: " << x << "," << y << std::endl;
        //        flScalar step = 0.25;
        //        for (flScalar i = -1; i < 1 + step; i += step) {
        //            FL_DEBUG << "degree (" << i << "): " << t.degreeOfMembership(i) << std::endl;
        //        }
        t.modulate(0.25, *op);
        FL_DEBUG(t.toString());
        //        FL_DEBUG << "area: " << t.area() << std::endl;
        //        t.centroid(x, y);
        //        FL_DEBUG << "centroid: " << x << "," << y << std::endl;
        //        for (flScalar i = -1; i < 1 + step; i += step) {
        //            FL_DEBUG << "degree (" << i << "): " << t.degreeOfMembership(i) << std::endl;
        //        }



    }

    void DiscreteTerm::main(int argc, char** argv) {

    }
}
