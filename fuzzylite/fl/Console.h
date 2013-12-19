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

namespace fl {
    class Engine;

    class Console {
    public:
        static const std::string KW_INPUT_FILE;
        static const std::string KW_INPUT_FORMAT;
        static const std::string KW_OUTPUT_FILE;
        static const std::string KW_OUTPUT_FORMAT;
        static const std::string KW_EXAMPLE;
        static const std::string KW_DATA_RESOLUTION_VARIABLE;
        static const std::string KW_DATA_MAXIMUM;
        static const std::string KW_DATA_SEPARATOR;

        static Engine* mamdani();
        static Engine* takagiSugeno();
        
    protected:
        static std::map<std::string, std::string> parse(int argc, char** argv);
        static void process(const std::map<std::string, std::string>& options);

        template <typename T>
        static void process(const std::string& input, T& writer,
                const std::string& inputFormat, const std::string& outputFormat,
                const std::map<std::string, std::string>& options);

    public:
        static std::string usage();
        static int main(int argc, char** argv);
    };

}

#endif	/* FL_CONSOLE_H */

