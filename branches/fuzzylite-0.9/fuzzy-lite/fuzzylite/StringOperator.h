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
 * File:   String.h
 * Author: jcrada
 *
 * Created on November 5, 2009, 12:23 PM
 */

#ifndef _STRINGOPERATOR_H
#define	_STRINGOPERATOR_H

#include <fuzzylite/defs.h>
#include <fuzzylite/flScalar.h>
#include <fuzzylite/FuzzyExceptions.h>

#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
namespace fl {

    class StringOperator {
    protected:

        static bool ascendantSorter(const std::string &left, const std::string &right) {
            for (std::string::const_iterator lit = left.begin(), rit = right.begin();
                    lit != left.end() && rit != right.end();
                    ++lit, ++rit) {
                if (tolower(*lit) < tolower(*rit))
                    return true;
                else if (tolower(*lit) > tolower(*rit))
                    return false;
            }
            if (left.size() < right.size()) {
                return true;
            }
            return false;
        }

        static bool descendantSorter(const std::string &left, const std::string &right) {
            for (std::string::const_iterator lit = left.begin(), rit = right.begin();
                    lit != left.end() && rit != right.end();
                    ++lit, ++rit) {
                if (tolower(*lit) < tolower(*rit))
                    return false;
                else if (tolower(*lit) > tolower(*rit))
                    return true;
            }
            if (left.size() < right.size()) {
                return false;
            }
            return true;
        }
    public:

        FL_INLINE static void LeftTrim(std::string& text) {
            text.erase(text.begin(), text.begin() + text.find_first_not_of(" "));
        }

        FL_INLINE static void RightTrim(std::string& text) {
            text.erase(text.find_last_not_of(" ") + 1);
        }

        FL_INLINE static void Trim(std::string& text) {
            RightTrim(text);
            LeftTrim(text);
        }

        FL_INLINE static std::string Substring(const std::string& text,
                const std::string substring) throw (OutOfRangeException) {
            int index = text.find_last_of(substring) - substring.size();
            OutOfRangeException::CheckArray(FL_AT, index, text.size());
            return text.substr(index);
        }

        FL_INLINE static std::string IntToString(int value) {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }

        FL_INLINE static std::string ScalarToString(flScalar value, int precision = 3) {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision) <<  value;
            return ss.str();
        }

        FL_INLINE static void sort(std::vector<std::string>& vector, bool asc = true) {
            std::sort(vector.begin(), vector.end(), (asc ? ascendantSorter : descendantSorter));
        }
    };
}

#endif	/* _STRINGOPERATOR_H */

