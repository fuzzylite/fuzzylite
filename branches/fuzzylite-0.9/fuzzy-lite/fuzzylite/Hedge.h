/*   Copyright 2010 Juan Rada-Vilela

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
 * File:   Hedge.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 2:55 PM
 */

#ifndef _HEDGE_H
#define	_HEDGE_H

#include <string>
#include <math.h>

#include <fuzzylite/flScalar.h>
#include <fuzzylite/defs.h>

namespace fl {

    class Hedge {
    private:
        std::string _name;
    public:

        Hedge(const std::string& name) : _name(name) {
        }

        virtual ~Hedge() {
        }

        virtual std::string name() const {
            return _name;
        }

        virtual flScalar hedge(flScalar mu) = 0;
    };

    class HedgeNot : public Hedge {
    public:

        HedgeNot() : Hedge("not") {
        }

        FL_INLINE virtual flScalar hedge(flScalar mu) {
            return 1 - mu;
        }
    };

    class HedgeSomewhat : public Hedge {
    public:

        HedgeSomewhat() : Hedge("somewhat") {
        };

        FL_INLINE virtual flScalar hedge(flScalar mu) {
            return sqrt(mu);
        }
    };

    class HedgeVery : public Hedge {
    public:

        HedgeVery() : Hedge("very") {
        }

        FL_INLINE virtual flScalar hedge(flScalar mu) {
            return mu * mu;
        }
    };
}

#endif	/* _HEDGE_H */

