/*
 * MaximumDefuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAXIMUMDEFUZZIFIER_H_
#define FL_MAXIMUMDEFUZZIFIER_H_

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
        MaximumDefuzzifier(Type type, int divisions = FL_DEFAULT_DIVISIONS);

        std::string className() const;
        scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;

        void setType(Type type);
        Type getType() const;
    };

    class MeanOfMaximum : public MaximumDefuzzifier {
    public:

        MeanOfMaximum(int divisions = FL_DEFAULT_DIVISIONS)
        : MaximumDefuzzifier(MEAN, divisions) {
        }
    };

    class SmallestOfMaximum : public MaximumDefuzzifier {
    public:

        SmallestOfMaximum(int divisions = FL_DEFAULT_DIVISIONS)
        : MaximumDefuzzifier(SMALLEST, divisions) {
        }
    };

    class LargestOfMaximum : public MaximumDefuzzifier {
    public:

        LargestOfMaximum(int divisions = FL_DEFAULT_DIVISIONS)
        : MaximumDefuzzifier(LARGEST, divisions) {
        }
    };

}
#endif /* FL_MAXIMUMDEFUZZIFIER_H_ */
