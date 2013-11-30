/* 
 * File:   Console.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 10:55 AM
 */

#include "fl/Console.h"

#include "fl/Headers.h"

#include <vector>
#include <utility>
#include <stdlib.h>
#include <fstream>


namespace fl {
    const std::string Console::KW_INPUT_FILE = "-i";
    const std::string Console::KW_INPUT_FORMAT = "-if";
    const std::string Console::KW_OUTPUT_FILE = "-o";
    const std::string Console::KW_OUTPUT_FORMAT = "-of";
    const std::string Console::KW_DATA_RESOLUTION = "-res";
    const std::string Console::KW_DATA_SEPARATOR = "-sep";

    std::string Console::usage() {
        std::vector<std::pair<std::string, std::string> > options;
        options.push_back(std::pair<std::string, std::string>(KW_INPUT_FILE, "inputfile"));
        options.push_back(std::pair<std::string, std::string>(KW_INPUT_FORMAT, "fis,fcl"));
        options.push_back(std::pair<std::string, std::string>(KW_OUTPUT_FILE, "outputfile"));
        options.push_back(std::pair<std::string, std::string>(KW_OUTPUT_FORMAT, "fis,fcl,cpp,java,dat"));
        options.push_back(std::pair<std::string, std::string>(KW_DATA_RESOLUTION, "resolution"));
        options.push_back(std::pair<std::string, std::string>(KW_DATA_SEPARATOR, "separator"));

        std::ostringstream ss;
        ss << "========================================\n";
        ss << "fuzzylite: a fuzzy logic control library\n";
        ss << "version: " << fuzzylite::longVersion() << "\n";
        ss << "author: " << fuzzylite::author() << "\n";
        ss << "========================================\n";
        ss << "usage: fuzzylite inputfile outputfile\n";
        ss << "   or: fuzzylite ";
        for (std::size_t i = 0; i < options.size(); ++i) {
            ss << "[" << options.at(i).first << "] ";
        }
        ss << "\n\n";
        ss << "where: ";
        for (std::size_t i = 0; i < options.size(); ++i) {
            ss << "[" << options.at(i).first << " " << options.at(i).second << "] \n       ";
        }
        ss << "\n";
        ss << "Visit http://www.fuzzylite.com for more information.";
        return ss.str();
    }

    std::map<std::string, std::string> Console::parse(int argc, char** argv) {
        std::map<std::string, std::string> options;
        for (int i = 1; i < argc - 1; i += 2) {
            std::string key = std::string(argv[i]);
            std::string value = std::string(argv[i + 1]);
            options[key] = value;
        }
        if (options.size() == 1) {
            std::map<std::string, std::string>::const_iterator it = options.begin();
            if (it->first.at(0) != '-') {
                options[KW_INPUT_FILE] = it->first;
                options[KW_OUTPUT_FILE] = it->second;
            }
        }
        return options;
    }

    void Console::process(const std::map<std::string, std::string>& options) {
        std::map<std::string, std::string>::const_iterator it = options.find(KW_INPUT_FILE);
        if (it == options.end()) {
            throw fl::Exception("[option error] no input file specified", FL_AT);
        }
        std::string inputFilename = it->second;
        std::ifstream inputFile(inputFilename.c_str());
        if (not inputFile.is_open()) {
            throw fl::Exception("[file error] file <" + inputFilename + "> could not be opened", FL_AT);
        }
        std::ostringstream textEngine;
        std::string line;
        while (inputFile.good()) {
            std::getline(inputFile, line);
            textEngine << line << std::endl;
        }
        inputFile.close();


        std::string inputFormat;
        it = options.find(KW_INPUT_FORMAT);
        if (it != options.end()) {
            inputFormat = it->second;
        } else {
            std::size_t extensionIndex = inputFilename.find_last_of(".");
            if (extensionIndex != std::string::npos) {
                inputFormat = inputFilename.substr(extensionIndex + 1);
            } else {
                throw fl::Exception("[format error] unspecified format of input file", FL_AT);
            }
        }

        std::string outputFilename;
        it = options.find(KW_OUTPUT_FILE);
        if (it != options.end()) {
            outputFilename = it->second;
        }

        std::string outputFormat;
        it = options.find(KW_OUTPUT_FORMAT);
        if (it != options.end()) {
            outputFormat = it->second;
        } else {
            std::size_t extensionIndex = outputFilename.find_last_of(".");
            if (extensionIndex != std::string::npos) {
                outputFormat = outputFilename.substr(extensionIndex + 1);
            } else {
                throw fl::Exception("[format error] unspecified format of output file", FL_AT);
            }
        }


        if (outputFilename.empty()) {
            process(textEngine.str(), std::cout, inputFormat, outputFormat, options);
        } else {
            std::ofstream writer(outputFilename.c_str());
            if (not writer.is_open()) {
                throw fl::Exception("[file error] file <" + outputFilename + "> could not be created", FL_AT);
            }
            process(textEngine.str(), writer, inputFormat, outputFormat, options);
            writer.flush();
            writer.close();
        }
    }

    template <typename T>
    void Console::process(const std::string& input, T& writer,
            const std::string& inputFormat, const std::string& outputFormat,
            const std::map<std::string, std::string>& options) {
        Importer* importer;
        if ("fcl" == inputFormat) {
            importer = new FclImporter;
        } else if ("fis" == inputFormat) {
            importer = new FisImporter;
        } else {
            throw fl::Exception("[import error] format <" + inputFormat + "> "
                    "not supported", FL_AT);
        }

        Exporter* exporter;
        if ("fcl" == outputFormat) {
            exporter = new FclExporter;
        } else if ("fis" == outputFormat) {
            exporter = new FisExporter;
        } else if ("c++" == outputFormat or "cpp" == outputFormat) {
            exporter = new CppExporter;
        } else if ("java" == outputFormat) {
            exporter = new JavaExporter;
        } else if ("dat" == outputFormat) {
            std::string separator = " ";
            std::map<std::string, std::string>::const_iterator it;
            it = options.find(KW_DATA_SEPARATOR);
            if (it != options.end()) {
                separator = it->second;
            }

            int resolution = 100;
            it = options.find(KW_DATA_RESOLUTION);
            if (it != options.end()) {
                resolution = (int) Op::toScalar(it->second);
            }
            exporter = new DataExporter(separator, resolution);
        } else {
            throw fl::Exception("[export error] format <" + outputFormat + "> "
                    "not supported", FL_AT);
        }

        Engine* engine = NULL;
        try {
            engine = importer->fromString(input);
            writer << exporter->toString(engine);
        } catch (fl::Exception& ex) {
            if (engine) delete engine;
            delete importer;
            delete exporter;
            throw ex;
        }
    }

    template void Console::process(const std::string& input, std::ostringstream& writer,
            const std::string& inputFormat, const std::string& outputFormat,
            const std::map<std::string, std::string>& options);

    template void Console::process(const std::string& input, std::ofstream& writer,
            const std::string& inputFormat, const std::string& outputFormat,
            const std::map<std::string, std::string>& options);

    int Console::main(int argc, char** argv) {
        (void) argc;
        (void) argv;
        if (argc <= 1) {
            std::cout << usage() << std::endl;
            return EXIT_SUCCESS;
        }

        std::map<std::string, std::string> options = parse(argc, argv);
        try {
            process(options);
        } catch (fl::Exception& ex) {
            std::cout << ex.getWhat() << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }


}