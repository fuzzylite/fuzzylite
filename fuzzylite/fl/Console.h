// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

/* 
 * File:   Console.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 10:54 AM
 */

#ifndef FL_CONSOLE_H
#define	FL_CONSOLE_H

#include "fl/fuzzylite.h"

#include <string>
#include <map>
#include <vector>

namespace fl {
    class Engine;

    class FL_EXPORT Console {
    public:

        struct Option {
            std::string key, value, description;

            Option(const std::string& key = "", const std::string& value = "", const std::string& description = "") :
            key(key), value(value), description(description) {
            }
        };

        static const std::string KW_INPUT_FILE;
        static const std::string KW_INPUT_FORMAT;
        static const std::string KW_OUTPUT_FILE;
        static const std::string KW_OUTPUT_FORMAT;
        static const std::string KW_EXAMPLE;
        static const std::string KW_DECIMALS;
        static const std::string KW_DATA_INPUT;
        static const std::string KW_DATA_MAXIMUM;
        static const std::string KW_DATA_SHOW_HEADERS;
        static const std::string KW_DATA_SHOW_INPUTS;

        static Engine* mamdani();
        static Engine* takagiSugeno();

    protected:
        static std::map<std::string, std::string> parse(int argc, char** argv);
        static void process(const std::map<std::string, std::string>& options);

        template <typename T>
        static void process(const std::string& input, T& writer,
                const std::string& inputFormat, const std::string& outputFormat,
                const std::map<std::string, std::string>& options);

        static void exportAllExamples(const std::string& from, const std::string& to);

    public:
        static std::string usage();
        static std::vector<Option> availableOptions();
        static int main(int argc, char** argv);
    };

}

#endif	/* FL_CONSOLE_H */

