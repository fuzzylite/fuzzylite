/*
 * MaximumDefuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAXIMUMDEFUZZIFIER_H
#define FL_MAXIMUMDEFUZZIFIER_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {

    class MaximumDefuzzifier : public Defuzzifier {
    public:

        enum Type {
            SMALLEST, LARGEST, MEAN
        };
    protected:
        Type _type;
    public:
        MaximumDefuzzifier(Type type, int divisions = FL_DIVISIONS);
        virtual ~MaximumDefuzzifier();
        
        virtual std::string className() const;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;

        virtual void setType(Type type);
        virtual Type getType() const;
    };

    class MeanOfMaximum : public MaximumDefuzzifier {
    public:

        MeanOfMaximum(int divisions = FL_DIVISIONS);
    };

    class SmallestOfMaximum : public MaximumDefuzzifier {
    public:

        SmallestOfMaximum(int divisions = FL_DIVISIONS);
    };

    class LargestOfMaximum : public MaximumDefuzzifier {
    public:

        LargestOfMaximum(int divisions = FL_DIVISIONS);
    };

}
#endif /* FL_MAXIMUMDEFUZZIFIER_H */
