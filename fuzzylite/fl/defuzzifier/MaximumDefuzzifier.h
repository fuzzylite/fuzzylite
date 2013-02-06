/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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

    class FL_EXPORT MaximumDefuzzifier : public Defuzzifier {
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

    class FL_EXPORT MeanOfMaximum : public MaximumDefuzzifier {
    public:

        MeanOfMaximum(int divisions = FL_DIVISIONS);
    };

    class FL_EXPORT SmallestOfMaximum : public MaximumDefuzzifier {
    public:

        SmallestOfMaximum(int divisions = FL_DIVISIONS);
    };

    class FL_EXPORT LargestOfMaximum : public MaximumDefuzzifier {
    public:

        LargestOfMaximum(int divisions = FL_DIVISIONS);
    };

}
#endif /* FL_MAXIMUMDEFUZZIFIER_H */
