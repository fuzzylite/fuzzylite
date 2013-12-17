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
 * File:   IntegralDefuzzifier.h
 * Author: jcrada
 *
 * Created on 2 November 2013, 9:18 AM
 */

#ifndef FL_INTEGRALDEFUZZIFIER_H
#define	FL_INTEGRALDEFUZZIFIER_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {
    //TODO: check  http://en.wikipedia.org/wiki/Adaptive_quadrature    

    class FL_EXPORT IntegralDefuzzifier : public Defuzzifier {
    protected:
        int _resolution;
    public:

        static int defaultResolution() {
            return 200;
        }

        IntegralDefuzzifier(int resolution = defaultResolution());
        virtual ~IntegralDefuzzifier();

        virtual void setResolution(int resolution);
        virtual int getResolution() const;


    };

}

#endif	/* INTEGRALDEFUZZIFIER_H */

