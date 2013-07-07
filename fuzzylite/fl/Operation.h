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
        template <typename T>
        static T min(T a, T b);

        template <typename T>
        static T max(T a, T b);

        template <typename T>
        static bool isInf(T x);

        template <typename T>
        static bool isNan(T x);

        //Is less than

        static bool isLt(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        static bool isLE(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        //Is equal

        static bool isEq(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        //Is greater than

        static bool isGt(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        static bool isGE(scalar a, scalar b, scalar tolerance = FL_PRECISION);

        static scalar scale(scalar x, scalar fromMin, scalar fromMax,
                scalar toMin, scalar toMax);

        static scalar plus(scalar a, scalar b);
        static scalar minus(scalar a, scalar b);
        static scalar multiplies(scalar a, scalar b);
        static scalar divides(scalar a, scalar b);
        static scalar modulus(scalar a, scalar b);
        static scalar logical_and(scalar a, scalar b);
        static scalar logical_or(scalar a, scalar b);
        static scalar negate(scalar a);

        static int isValidForName(int character);


        static std::string findReplace(const std::string& str, const std::string& find,
                const std::string& replace, bool replaceAll = true);

        static std::vector<std::string> split(const std::string& str,
                const std::string& delimiter = " ", bool ignoreEmpty = true);

        static std::string trim(const std::string& text);

        static std::string format(const std::string& text, int matchesChar(int),
                const std::string& replacement = "");

        static scalar toScalar(const std::string& x, bool quiet = false,
                scalar alternative = fl::nan) throw (fl::Exception);

        static bool isNumeric(const std::string& x);

        template <typename T>
        static std::string str(T x, int decimals = FL_DECIMALS);

        template <typename T>
        static std::string str(const std::vector<T>& x,
                const std::string& separator = ", ");

        template <typename T>
        static std::string str(int items, const std::string& separator, T first, ...);
    };

    typedef Operation Op;
}
#endif	/* FL_OPERATION_H */

