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
 * Activated.h
 *
 *  Created on: 27/01/2014
 *      Author: jcrada
 */

#ifndef FL_ACTIVATED_H
#define FL_ACTIVATED_H

#include "fl/term/Term.h"

namespace fl {
    class TNorm;

    class FL_EXPORT Activated : public Term {
    protected:
        const Term* _term;
        scalar _degree;
        const TNorm* _activation;

    public:
        Activated(const Term* term = NULL, scalar degree = 1.0,
                const TNorm* activationOperator = NULL);

        virtual ~Activated();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;
        virtual std::string toString() const;

        virtual void setTerm(const Term* term);
        virtual const Term* getTerm() const;

        virtual void setDegree(scalar degree);
        virtual scalar getDegree() const;

        virtual void setActivation(const TNorm* activation);
        virtual const TNorm* getActivation() const;

        virtual Activated* copy() const;
    };

}
#endif /* FL_ACTIVATED_H */
