/* 
 * File:   DiscreteTerm.h
 * Author: jcrada
 *
 * Created on November 2, 2009, 1:45 PM
 */

#ifndef _DISCRETETERM_H
#define	_DISCRETETERM_H

#include <vector>
#include "flScalar.h"
#include "ContinuousTerm.h"
#include "FuzzyOperator.h"
#include "FuzzyException.h"
#include "FuzzyExceptions.h"


namespace fuzzy_lite {
    class FuzzyOperator;

    class DiscreteTerm {
    private:
        std::string _name;
        std::vector<flScalar> _x;
        std::vector<flScalar> _y;
        bool _from_infinity;
        bool _to_infinity;
        flScalar _intersect_x;
        flScalar _intersect_y;

    public:
        DiscreteTerm();
        DiscreteTerm(const std::string& name);
        virtual ~DiscreteTerm();

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual void addCoord(flScalar x, flScalar y);
        virtual void removeCoord(int index) throw (OutOfRangeException);
        virtual void changeCoord(int index, flScalar x, flScalar y) throw (OutOfRangeException);
        virtual void insertCoord(int index, flScalar x, flScalar y) throw (OutOfRangeException);
        virtual flScalar getCoordX(int index) const throw (OutOfRangeException);
        virtual flScalar getCoordY(int index) const throw (OutOfRangeException);
        virtual void copyTo(DiscreteTerm& term) const;
        virtual void clear();
        virtual int numberOfCoords() const;

        virtual flScalar minimum() const;
        virtual flScalar maximum() const;
        virtual bool isEmpty() const;

        virtual void intersectionOnAggregate(flScalar& ix, flScalar& iy) const;

        virtual void setFromInfinity(bool from_infinity);
        virtual bool isFromInfinity() const;
        virtual void setToInfinity(bool to_infinity);
        virtual bool isToInfinity() const;

        virtual flScalar membership(flScalar crisp) const;
        virtual flScalar area() const;
        virtual void centroid(flScalar& x, flScalar& y) const;

        virtual void aggregate(const DiscreteTerm& term);
        virtual void modulate(flScalar degree, const FuzzyOperator& fuzzy_op, flScalar prob = 1.0);

        virtual void optimize();

        virtual std::string toString() const;

        static DiscreteTerm* Discretize(const ContinuousTerm& term);
        static bool Intersects(
                flScalar x1, flScalar y1, flScalar x2, flScalar y2,
                flScalar x3, flScalar y3, flScalar x4, flScalar y4,
                flScalar& ix, flScalar& iy);
        static void test0();
        static void test1();
        static void main(int argc, char** argv);

    };



}

#endif	/* _DISCRETETERM_H */

