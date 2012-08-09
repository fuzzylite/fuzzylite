/* 
 * File:   String.h
 * Author: jcrada
 *
 * Created on November 5, 2009, 12:23 PM
 */

#ifndef _STRINGOPERATOR_H
#define	_STRINGOPERATOR_H

#include "defs.h"

#include "FuzzyExceptions.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
namespace fuzzy_lite {

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

