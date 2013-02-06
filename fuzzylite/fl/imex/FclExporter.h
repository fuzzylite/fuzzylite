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
 * FclExporter.h
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#ifndef FL_FCLEXPORTER_H
#define FL_FCLEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;

    class FL_EXPORT FclExporter : public Exporter {
    protected:
        virtual std::string toFcl(const TNorm* tnorm) const;
        virtual std::string toFcl(const SNorm* snorm) const;
        virtual std::string toFcl(const Defuzzifier* defuzzifier) const;
        virtual std::string toFcl(const Term* term) const;

    public:
        FclExporter();
        virtual ~FclExporter();

        virtual std::string name() const;
        virtual std::string toString(const Engine* engine) const;
    };

}
#endif /* FL_FCLEXPORTER_H */
