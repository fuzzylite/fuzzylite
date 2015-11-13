/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_CONSOLE_H
#define FL_CONSOLE_H

#include "fl/fuzzylite.h"

#include <map>
#include <string>
#include <vector>

namespace fl {
    class Engine;

    class FL_API Console {
    public:

        struct Option {
            std::string key, value, description;

            explicit Option(const std::string& key = "", const std::string& value = "", const std::string& description = "") :
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
        static const std::string KW_DATA_EXPORT_HEADER;
        static const std::string KW_DATA_EXPORT_INPUTS;

        static Engine* mamdani();
        static Engine* takagiSugeno();

    protected:
        static std::map<std::string, std::string> parse(int argc, char** argv);
        static void process(const std::map<std::string, std::string>& options);

        static void process(const std::string& input, std::ostream& writer,
                const std::string& inputFormat, const std::string& outputFormat,
                const std::map<std::string, std::string>& options);

        static int readCharacter();
        static void interactive(std::ostream& writer, Engine* engine);
        static std::string interactiveHelp();

        static void exportAllExamples(const std::string& from, const std::string& to);
        static void exportAllExamples(const std::string& from, const std::string& to,
                const std::string& examplesPath, const std::string& outputPath);
#ifdef FL_CPP11
        static void benchmarkExamples(const std::string& path, int runs);
#endif

    public:
        static std::string usage();
        static std::vector<Option> availableOptions();

        static int main(int argc, char** argv);
    };

}

#endif  /* FL_CONSOLE_H */

