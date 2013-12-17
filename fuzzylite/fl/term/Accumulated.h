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
 * Accumulated.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_ACCUMULATED_H
#define FL_ACCUMULATED_H

#include "fl/term/Term.h"

#include <vector>

namespace fl {

    class SNorm;

    class FL_EXPORT Accumulated : public Term {
    protected:
        std::vector<const Term*> _terms;
        scalar _minimum, _maximum;
        const SNorm* _accumulation;
    public:
        Accumulated(const std::string& name = "",
                scalar minimum = -fl::inf,
                scalar maximum = fl::inf,
                const SNorm* accumulation = NULL);
        virtual ~Accumulated();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);
        
        virtual Accumulated* copy() const;

        virtual scalar membership(scalar x) const;
        

        virtual void setMinimum(scalar minimum);
        virtual scalar getMinimum() const;

        virtual void setMaximum(scalar maximum);
        virtual scalar getMaximum() const;

        virtual void setAccumulation(const SNorm* accumulation);
        virtual const SNorm* getAccumulation() const;

        /**
         * Operations for std::vector _terms
         */
        virtual void addTerm(const Term* term);
        virtual const Term* getTerm(int index) const;
        virtual const Term* removeTerm(int index);
        virtual int numberOfTerms() const;
        virtual const std::vector<const Term*>& terms() const;
        virtual bool isEmpty() const;
        virtual void clear();
    };

}
#endif /* FL_ACCUMULATED_H */
