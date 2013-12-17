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
 * File:   Constant.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 8:21 PM
 */

#ifndef FL_CONSTANT_H
#define	FL_CONSTANT_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Constant : public Term {
    protected:
        scalar _value;

    public:
        Constant(const std::string& name = "", 
                scalar value = fl::nan);
        virtual ~Constant();
        
        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);
        
        virtual scalar membership(scalar x) const;

        virtual void setValue(scalar value);
        virtual scalar getValue() const;
        
        virtual Constant* copy() const;
        
        static Term* constructor();
    };
}

#endif	/* FL_CONSTANT_H */

