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
 * File:   Linear.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 3:42 PM
 */

#ifndef FL_LINEAR_H
#define	FL_LINEAR_H

#include "fl/term/Term.h"
#include "fl/variable/InputVariable.h"

namespace fl {

    class FL_EXPORT Linear : public Term {
    public:
        std::vector<scalar> coefficients;
        std::vector<const InputVariable*> inputVariables;
        Linear(const std::string& name = "",
                const std::vector<scalar>& coefficients = std::vector<scalar>(),
                const std::vector<InputVariable*>& inputVariables = std::vector<InputVariable*>());
        virtual ~Linear();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        //Warning: this method is unsafe, make sure you use it correctly.
        template <typename T>
        static Linear* create(const std::string& name, const std::vector<InputVariable*>& inputVariables,
                T firstCoefficient, ...);

        virtual scalar membership(scalar x) const;

        virtual void set(const std::vector<scalar>& coefficients,
                const std::vector<InputVariable*>& inputVariables) throw (fl::Exception);
        virtual void set(const std::vector<scalar>& coefficients,
                const std::vector<const InputVariable*>& inputVariables) throw (fl::Exception);

        virtual Linear* copy() const;

        static Term* constructor();
    };

}

#endif	/* FL_LINEAR_H */

