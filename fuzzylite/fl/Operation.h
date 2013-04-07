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
 * File:   Operation.h
 * Author: jcrada
 *
 * Created on 1 February 2013, 6:10 PM
 */

#ifndef FL_OPERATION_H
#define	FL_OPERATION_H

#include "fl/fuzzylite.h"

#include "fl/Exception.h"

#include <string>
#include <vector>
#include <limits>

namespace fl {

    class FL_EXPORT Operation {
    public:

        static scalar min(scalar a, scalar b);

        static scalar max(scalar a, scalar b);

        static bool isInf(scalar x);

        static bool isNan(scalar x);

        //Is less than

        static bool isLt(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        static bool isLE(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        //Is equal

        static bool isEq(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        //Is greater than

        static bool isGt(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        static bool isGE(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        static scalar scale(scalar x, scalar fromMin, scalar fromMax, scalar toMin, scalar toMax);

        static std::string findReplace(const std::string& str, const std::string& find,
                const std::string& replace, bool replaceAll = true);

        static std::vector<std::string> split(const std::string& str,
                const std::string& delimiter = " ", bool ignoreEmpty = true);

        static std::string leftTrim(const std::string& text);

        static std::string rightTrim(const std::string& text);

        static std::string trim(const std::string& text);

        static scalar toScalar(const std::string& x, bool quiet = false,
                scalar alternative = std::numeric_limits<scalar>::quiet_NaN())
        throw (fl::Exception);

        static std::string str(int x);
        static std::string str(scalar x, int precision = FL_DECIMALS);

        static std::string str(int items, scalar* x, const std::string& separator = ",",
                int precision = FL_DECIMALS);

        template <typename T>
        static std::string str(const std::vector<T>& x,
                const std::string& separator = ", ", int precision = FL_DECIMALS);
    };

    typedef Operation Op;
}
#endif	/* FL_OPERATION_H */

