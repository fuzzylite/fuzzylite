/*
 Copyright © 2010-2016 by FuzzyLite Limited.
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

    /**

      The Console class is a command-line tool that helps to utilize the
      `fuzzylite` library.

      @author Juan Rada-Vilela, Ph.D.
      @since 4.0

     */
    class FL_API Console {
    public:

        /**
          A command-line option given by key, value and description
         */
        struct Option {
            std::string key, value, description;

            explicit Option(const std::string& key = "", const std::string& value = "", const std::string& description = "") :
            key(key), value(value), description(description) {
            }
        };

        /**Keyword for input file*/
        static const std::string KW_INPUT_FILE;
        /**Keyword for input file format*/
        static const std::string KW_INPUT_FORMAT;
        /**Keyword for output file*/
        static const std::string KW_OUTPUT_FILE;
        /**Keyword for output file format*/
        static const std::string KW_OUTPUT_FORMAT;
        /**Keyword for built-in example*/
        static const std::string KW_EXAMPLE;
        /**Keyword for number of decimals*/
        static const std::string KW_DECIMALS;
        /**Keyword for file containing input data*/
        static const std::string KW_DATA_INPUT_FILE;
        /**Keyword for number of values to generate*/
        static const std::string KW_DATA_VALUES;
        /**Keyword for the scope of the number of values to generate*/
        static const std::string KW_DATA_VALUES_SCOPE;
        /**Keyword for exporting headers in FLD*/
        static const std::string KW_DATA_EXPORT_HEADER;
        /**Keyword for exporting input values in FLD*/
        static const std::string KW_DATA_EXPORT_INPUTS;

        /**
          Creates a new Mamdani Engine based on the SimpleDimmer example
          @return a new Mamdani Engine based on the SimpleDimmer example
         */
        static Engine* mamdani();
        /**
          Creates a new TakagiSugeno Engine based on the Approximation example of @f$sin(x)/x@f$
          @return a new TakagiSugeno Engine based on the Approximation example of @f$sin(x)/x@f$
         */
        static Engine* takagiSugeno();

    protected:
        virtual std::map<std::string, std::string> parse(int argc, const char* argv[]);
        virtual void process(const std::map<std::string, std::string>& options);

        virtual void process(const std::string& input, std::ostream& writer,
                const std::string& inputFormat, const std::string& outputFormat,
                const std::map<std::string, std::string>& options);

        virtual int readCharacter();
        virtual void interactive(std::ostream& writer, Engine* engine);
        virtual std::string interactiveHelp();

        virtual void exportAllExamples(const std::string& from, const std::string& to);
        virtual void exportAllExamples(const std::string& from, const std::string& to,
                const std::string& examplesPath, const std::string& outputPath);
#ifdef FL_CPP11
        static void benchmarkExamples(const std::string& path, int runs);
#endif

    public:
        /**
          Returns a string representation of the usage of the command-line tool
          @return a string representation of the usage of the command-line tool
         */
        virtual std::string usage();
        /**
          Returns a vector of the options available from the command line
          @return a vector of the options available from the command line
         */
        virtual std::vector<Option> availableOptions();

        static int main(int argc, const char* argv[]);
    };
}
#endif  /* FL_CONSOLE_H */

