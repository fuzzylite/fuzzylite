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
 * File:   SNorm.h
 * Author: jcrada
 *
 * Created on 21 December 2012, 9:34 AM
 */

#ifndef FL_SNORM_H
#define	FL_SNORM_H

#include "fl/operator/Operator.h"

namespace fl {

    class FL_EXPORT SNorm : public Operator {
        //Dummy class FL_EXPORT
    };

    class FL_EXPORT Maximum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT AlgebraicSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT BoundedSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT NormalizedSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT DrasticSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT EinsteinSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

    class FL_EXPORT HamacherSum : public SNorm {
    public:
        std::string className() const;
        scalar compute(scalar a, scalar b) const;
    };

}
#endif	/* FL_SNORM_H */

