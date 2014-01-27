/* 
 * File:   Console.h
 * Author: jcrada
 *
 * Created on 30 November 2013, 10:54 AM
 */

#ifndef FL_CONSOLE_H
#define	FL_CONSOLE_H

#include <string>
#include <map>
#include <vector>

namespace fl {
    class Engine;

    class Console {
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

