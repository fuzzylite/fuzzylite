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
    const std::string Console::KW_EXAMPLE = "-ex";
    const std::string Console::KW_DATA_MAXIMUM = "-max";
    const std::string Console::KW_DATA_SEPARATOR = "-sep";

    std::string Console::usage() {
        std::vector<std::pair<std::string, std::string> > options; //not a map to keep order;
        options.push_back(std::pair<std::string, std::string>(KW_INPUT_FILE, "inputfile"));
        options.push_back(std::pair<std::string, std::string>(KW_INPUT_FORMAT, "fll,fis,fcl"));
        options.push_back(std::pair<std::string, std::string>(KW_OUTPUT_FILE, "outputfile"));
        options.push_back(std::pair<std::string, std::string>(KW_OUTPUT_FORMAT, "fll,fis,fcl,cpp,java,fld"));
        options.push_back(std::pair<std::string, std::string>(KW_EXAMPLE, "(m)amdani,(t)akagi-sugeno"));
        options.push_back(std::pair<std::string, std::string>(KW_DATA_MAXIMUM, "maximum"));
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
        if ((argc - 1) % 2 != 0) {
            throw fl::Exception("[option error] incomplete number of parameters [key value]", FL_AT);
        }
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
        } else {
            std::map<std::string, std::string> valid;
            valid[KW_INPUT_FILE] = "inputfile";
            valid[KW_INPUT_FORMAT] = "fll,fis,fcl";
            valid[KW_OUTPUT_FILE] = "outputfile";
            valid[KW_OUTPUT_FORMAT] = "fll,fis,fcl,cpp,java,fld";
            valid[KW_EXAMPLE] = "(m)amdani,(t)akagi-sugeno";
            valid[KW_DATA_MAXIMUM] = "maximum";
            valid[KW_DATA_SEPARATOR] = "separator";
            for (std::map<std::string, std::string>::const_iterator it = options.begin();
                    it != options.end(); ++it) {
                if (valid.find(it->first) == valid.end()) {
                    throw fl::Exception("[option error] option <" + it->first + "> not supported", FL_AT);
                }
            }
        }
        return options;
    }

    void Console::process(const std::map<std::string, std::string>& options) {
        std::map<std::string, std::string>::const_iterator it;

        std::string example;
        std::string inputFormat;
        std::ostringstream textEngine;

        it = options.find(KW_EXAMPLE);

        bool isExample = (it != options.end());

        if (isExample) {
            example = it->second;
            Engine* engine;
            if (example == "m" or example == "mamdani") {
                engine = mamdani();
            } else if (example == "t" or example == "ts" or example == "takagi-sugeno") {
                engine = takagiSugeno();
            } else {
                throw fl::Exception("[option error] example <" + example + "> not available", FL_AT);
            }
            inputFormat = "fll";
            textEngine << FllExporter().toString(engine);
            delete engine;

        } else {
            it = options.find(KW_INPUT_FILE);
            if (it == options.end()) {
                throw fl::Exception("[option error] no input file specified", FL_AT);
            }
            std::string inputFilename = it->second;
            std::ifstream inputFile(inputFilename.c_str());
            if (not inputFile.is_open()) {
                throw fl::Exception("[file error] file <" + inputFilename + "> could not be opened", FL_AT);
            }
            std::string line;
            while (inputFile.good()) {
                std::getline(inputFile, line);
                textEngine << line << std::endl;
            }
            inputFile.close();

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
        if ("fll" == inputFormat) {
            importer = new FllImporter;
        } else if ("fcl" == inputFormat) {
            importer = new FclImporter;
        } else if ("fis" == inputFormat) {
            importer = new FisImporter;
        } else {
            throw fl::Exception("[import error] format <" + inputFormat + "> "
                    "not supported", FL_AT);
        }

        Engine* engine = NULL;
        try {
            engine = importer->fromString(input);
        } catch (fl::Exception& ex) {
            if (engine) delete engine;
            delete importer;
            throw ex;
        }

        Exporter* exporter;
        if ("fll" == outputFormat) {
            exporter = new FllExporter;
        } else if ("fcl" == outputFormat) {
            exporter = new FclExporter;
        } else if ("fis" == outputFormat) {
            exporter = new FisExporter;
        } else if ("c++" == outputFormat or "cpp" == outputFormat) {
            exporter = new CppExporter;
        } else if ("java" == outputFormat) {
            exporter = new JavaExporter;
        } else if ("fld" == outputFormat) {
            std::string separator = " ";
            std::map<std::string, std::string>::const_iterator it;
            it = options.find(KW_DATA_SEPARATOR);
            if (it != options.end()) {
                separator = it->second;
            }

            it = options.find(KW_DATA_MAXIMUM);
            if (it != options.end()) {
                int maximum = (int) Op::toScalar(it->second);
                exporter = new FldExporter(separator, maximum);
            } else {
                exporter = new FldExporter(separator);
            }
        } else {
            throw fl::Exception("[export error] format <" + outputFormat + "> "
                    "not supported", FL_AT);
        }

        try {
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

    Engine* Console::mamdani() {
        Engine* engine = new Engine("simple-dimmer");

        InputVariable* ambient = new InputVariable("Ambient", 0, 1);
        ambient->addTerm(new Triangle("DARK", .0, .25, .5));
        ambient->addTerm(new Triangle("MEDIUM", .25, .5, .75));
        ambient->addTerm(new Triangle("BRIGHT", .5, .75, 1));
        engine->addInputVariable(ambient);


        OutputVariable* power = new OutputVariable("Power", 0, 2);
        power->setDefaultValue(fl::nan);
        power->addTerm(new Triangle("LOW", 0.0, 0.5, 1));
        power->addTerm(new Triangle("MEDIUM", 0.5, 1, 1.5));
        power->addTerm(new Triangle("HIGH", 1, 1.5, 2));
        engine->addOutputVariable(power);

        RuleBlock* ruleblock = new RuleBlock();
        ruleblock->addRule(Rule::parse("if Ambient is DARK then Power is HIGH", engine));
        ruleblock->addRule(Rule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
        ruleblock->addRule(Rule::parse("if Ambient is BRIGHT then Power is LOW", engine));

        engine->addRuleBlock(ruleblock);

        engine->configure("", "", "Minimum", "Maximum", "Centroid");

        return engine;
    }

    Engine* Console::takagiSugeno() {
        Engine* engine = new Engine("approximation of sin(x)/x");

        fl::InputVariable* inputX = new fl::InputVariable("inputX");
        inputX->setRange(0, 10);
        inputX->addTerm(new fl::Triangle("NEAR_1", 0, 1, 2));
        inputX->addTerm(new fl::Triangle("NEAR_2", 1, 2, 3));
        inputX->addTerm(new fl::Triangle("NEAR_3", 2, 3, 4));
        inputX->addTerm(new fl::Triangle("NEAR_4", 3, 4, 5));
        inputX->addTerm(new fl::Triangle("NEAR_5", 4, 5, 6));
        inputX->addTerm(new fl::Triangle("NEAR_6", 5, 6, 7));
        inputX->addTerm(new fl::Triangle("NEAR_7", 6, 7, 8));
        inputX->addTerm(new fl::Triangle("NEAR_8", 7, 8, 9));
        inputX->addTerm(new fl::Triangle("NEAR_9", 8, 9, 10));
        engine->addInputVariable(inputX);


        fl::OutputVariable* outputFx = new fl::OutputVariable("outputFx");
        outputFx->setRange(-1, 1);
        outputFx->setDefaultValue(fl::nan);
        outputFx->setLockValidOutput(true); //To use its value with diffFx
        outputFx->addTerm(new Constant("f1", 0.84));
        outputFx->addTerm(new Constant("f2", 0.45));
        outputFx->addTerm(new Constant("f3", 0.04));
        outputFx->addTerm(new Constant("f4", -0.18));
        outputFx->addTerm(new Constant("f5", -0.19));
        outputFx->addTerm(new Constant("f6", -0.04));
        outputFx->addTerm(new Constant("f7", 0.09));
        outputFx->addTerm(new Constant("f8", 0.12));
        outputFx->addTerm(new Constant("f9", 0.04));
        engine->addOutputVariable(outputFx);

        fl::OutputVariable* trueFx = new fl::OutputVariable("trueFx");
        trueFx->setRange(fl::nan, fl::nan);
        trueFx->setLockValidOutput(true); //To use its value with diffFx
        trueFx->addTerm(fl::Function::create("fx", "sin(inputX)/inputX"));
        engine->addOutputVariable(trueFx);

        fl::OutputVariable* diffFx = new fl::OutputVariable("diffFx");
        diffFx->addTerm(fl::Function::create("diff", "fabs(outputFx-trueFx)", engine));
        diffFx->setRange(fl::nan, fl::nan);
        //        diffFx->setLockValidOutput(true); //To use in input diffPreviousFx
        engine->addOutputVariable(diffFx);

        fl::RuleBlock* block = new fl::RuleBlock();
        block->addRule(fl::Rule::parse("if inputX is NEAR_1 then outputFx = f1", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_2 then outputFx = f2", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_3 then outputFx = f3", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_4 then outputFx = f4", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_5 then outputFx = f5", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_6 then outputFx = f6", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_7 then outputFx = f7", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_8 then outputFx = f8", engine));
        block->addRule(fl::Rule::parse("if inputX is NEAR_9 then outputFx = f9", engine));
        block->addRule(fl::Rule::parse("if inputX is any then trueFx = fx and diffFx = diff", engine));
        engine->addRuleBlock(block);

        engine->configure("", "", "AlgebraicProduct", "", "WeightedAverage");

        return engine;
    }

    int Console::main(int argc, char** argv) {
        (void) argc;
        (void) argv;
        if (argc <= 1) {
            std::cout << usage() << std::endl;
            return EXIT_SUCCESS;
        }

        try {
            std::map<std::string, std::string> options = parse(argc, argv);
            process(options);
        } catch (fl::Exception& ex) {
            std::cout << ex.getWhat() << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }


}