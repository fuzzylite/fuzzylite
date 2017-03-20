/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/Console.h"

#include "fl/Headers.h"

#include <fstream>

#ifdef FL_UNIX
#include <termios.h>
#include <unistd.h>
#elif defined(FL_WINDOWS)
#include <conio.h>
#endif


namespace fl {
    const std::string Console::KW_INPUT_FILE = "-i";
    const std::string Console::KW_INPUT_FORMAT = "-if";
    const std::string Console::KW_OUTPUT_FILE = "-o";
    const std::string Console::KW_OUTPUT_FORMAT = "-of";
    const std::string Console::KW_EXAMPLE = "-example";
    const std::string Console::KW_DECIMALS = "-decimals";
    const std::string Console::KW_DATA_INPUT_FILE = "-d";
    const std::string Console::KW_DATA_VALUES = "-values";
    const std::string Console::KW_DATA_VALUES_SCOPE = "-scope";

    const std::string Console::KW_DATA_EXPORT_HEADER = "-dheader";
    const std::string Console::KW_DATA_EXPORT_INPUTS = "-dinputs";

    Console::Option::Option(const std::string& key, const std::string& value, const std::string& description) :
    key(key), value(value), description(description) { }

    std::vector<Console::Option> Console::availableOptions() {
        std::vector<Console::Option> options;
        options.push_back(Option(KW_INPUT_FILE, "inputfile", "file to import your engine from"));
        options.push_back(Option(KW_INPUT_FORMAT, "format", "format of the file to import (fll | fis | fcl)"));
        options.push_back(Option(KW_OUTPUT_FILE, "outputfile", "file to export your engine to"));
        options.push_back(Option(KW_OUTPUT_FORMAT, "format", "format of the file to export (fll | fld | cpp | java | fis | fcl)"));
        options.push_back(Option(KW_EXAMPLE, "letter", "if not inputfile, built-in example to use as engine: (m)amdani or (t)akagi-sugeno"));
        options.push_back(Option(KW_DECIMALS, "number", "number of decimals to write floating-poing values"));
        options.push_back(Option(KW_DATA_INPUT_FILE, "file", "if exporting to fld, FLD file of input values to evaluate your engine on"));
        options.push_back(Option(KW_DATA_VALUES, "number", "if exporting to fld without datafile, number of results to export within scope (default: EachVariable)"));
        options.push_back(Option(KW_DATA_VALUES_SCOPE, "scope", "if exporting to fld without datafile, scope of " + KW_DATA_VALUES + ": [EachVariable|AllVariables]"));
        options.push_back(Option(KW_DATA_EXPORT_HEADER, "boolean", "if true and exporting to fld, include headers"));
        options.push_back(Option(KW_DATA_EXPORT_INPUTS, "boolean", "if true and exporting to fld, include input values"));
        return options;
    }

    std::string Console::usage() {
        std::vector<Console::Option> options = availableOptions();
        std::ostringstream ss;

        ss << "========================================\n";
        ss << "fuzzylite: a fuzzy logic control library\n";
        ss << "version: " << fuzzylite::version() << "\n";
        ss << "author: " << fuzzylite::author() << "\n";
        ss << "license: " << fuzzylite::license() << "\n";
        ss << "========================================\n\n";
        ss << "usage: fuzzylite inputfile outputfile\n";
        ss << "   or: fuzzylite benchmark engine.fll input.fld runs [output.tsv]\n";
        ss << "   or: fuzzylite benchmarks fllFiles.txt fldFiles.txt runs [output.tsv]\n";
        ss << "   or: fuzzylite ";
        for (std::size_t i = 0; i < options.size(); ++i) {
            ss << "[" << options.at(i).key << " " << options.at(i).value << "] ";
        }
        ss << "\n\nwhere:\n";
        for (std::size_t i = 0; i < options.size(); ++i) {
            std::string spacedKey(12, ' ');
            std::string key = options.at(i).key;
            std::copy(key.begin(), key.end(), spacedKey.begin());

            std::string spacedValue(13, ' ');
            std::string value = options.at(i).value;
            std::copy(value.begin(), value.end(), spacedValue.begin());

            std::string description = options.at(i).description;

            ss << spacedKey << spacedValue << description << "\n";
        }

        ss << "\n";
        ss << "Visit " << fuzzylite::website() << " for more information.\n\n";
        ss << "Copyright (C) 2010-2017 by FuzzyLite Limited.\n";
        ss << "All rights reserved.";

        return ss.str();
    }

    std::map<std::string, std::string> Console::parse(int argc, const char* argv[]) {
        if ((argc - 1) % 2 != 0) {
            throw Exception("[option error] incomplete number of parameters [key value]", FL_AT);
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
            std::vector<Console::Option> validOptions = availableOptions();

            for (std::map<std::string, std::string>::const_iterator it = options.begin();
                    it != options.end(); ++it) {
                bool isValid = false;
                for (std::size_t i = 0; i < validOptions.size(); ++i) {
                    std::string key = validOptions.at(i).key;
                    if (key == it->first) {
                        isValid = true;
                        break;
                    }
                }
                if (not isValid) {
                    throw Exception("[option error] option <" + it->first + "> not recognized", FL_AT);
                }
            }
        }
        return options;
    }

    void Console::process(const std::map<std::string, std::string>& options) {
        std::map<std::string, std::string>::const_iterator it;

        it = options.find(KW_DECIMALS);
        if (it != options.end()) {
            fuzzylite::setDecimals((int) Op::toScalar(it->second));
        }

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
                throw Exception("[option error] example <" + example + "> not available", FL_AT);
            }
            inputFormat = "fll";
            textEngine << FllExporter().toString(engine);
            delete engine;

        } else {
            it = options.find(KW_INPUT_FILE);
            if (it == options.end()) {
                throw Exception("[option error] no input file specified", FL_AT);
            }
            std::string inputFilename = it->second;
            std::ifstream inputFile(inputFilename.c_str());
            if (not inputFile.is_open()) {
                throw Exception("[file error] file <" + inputFilename + "> could not be opened", FL_AT);
            }
            std::string line;
            while (std::getline(inputFile, line)) {
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
                    throw Exception("[format error] unspecified format of input file", FL_AT);
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
                throw Exception("[format error] unspecified format of output file", FL_AT);
            }
        }


        if (outputFilename.empty()) {
            process(textEngine.str(), std::cout, inputFormat, outputFormat, options);
        } else {
            std::ofstream writer(outputFilename.c_str());
            if (not writer.is_open()) {
                throw Exception("[file error] file <" + outputFilename + "> could not be created", FL_AT);
            }
            process(textEngine.str(), writer, inputFormat, outputFormat, options);
            writer.flush();
            writer.close();
        }
    }

    void Console::process(const std::string& input, std::ostream& writer,
            const std::string& inputFormat, const std::string& outputFormat,
            const std::map<std::string, std::string>& options) {
        FL_unique_ptr<Importer> importer;
        FL_unique_ptr<Exporter> exporter;
        FL_unique_ptr<Engine> engine;

        if ("fll" == inputFormat) {
            importer.reset(new FllImporter);
        } else if ("fcl" == inputFormat) {
            importer.reset(new FclImporter);
        } else if ("fis" == inputFormat) {
            importer.reset(new FisImporter);
        } else {
            throw Exception("[import error] format <" + inputFormat + "> "
                    "not supported", FL_AT);
        }

        engine.reset(importer->fromString(input));

        if ("fld" == outputFormat) {
            std::map<std::string, std::string>::const_iterator it;

            FldExporter fldExporter;
            fldExporter.setSeparator(" ");
            bool exportHeaders = true;
            if ((it = options.find(KW_DATA_EXPORT_HEADER)) != options.end()) {
                exportHeaders = ("true" == it->second);
            }
            fldExporter.setExportHeader(exportHeaders);
            bool exportInputValues = true;
            if ((it = options.find(KW_DATA_EXPORT_INPUTS)) != options.end()) {
                exportInputValues = ("true" == it->second);
            }
            fldExporter.setExportInputValues(exportInputValues);
            if ((it = options.find(KW_DATA_INPUT_FILE)) != options.end()) {
                std::ifstream dataFile(it->second.c_str());
                if (not dataFile.is_open()) {
                    throw Exception("[export error] file <" + it->second + "> could not be opened", FL_AT);
                }
                try {
                    fldExporter.write(engine.get(), writer, dataFile);
                } catch (std::exception& ex) {
                    FL_IUNUSED(ex);
                    dataFile.close();
                    throw;
                }

            } else {
                if ((it = options.find(KW_DATA_VALUES)) != options.end()) {
                    int values = (int) Op::toScalar(it->second);
                    FldExporter::ScopeOfValues scope = FldExporter::EachVariable;
                    if ((it = options.find(KW_DATA_VALUES_SCOPE)) != options.end()) {
                        if ("AllVariables" == it->second)
                            scope = FldExporter::AllVariables;
                        else if ("EachVariable" == it->second)
                            scope = FldExporter::EachVariable;
                        else throw Exception("[export error] unknown scope of values <"
                                + it->second + ">", FL_AT);
                    }
                    fldExporter.write(engine.get(), writer, values, scope);
                } else {
                    std::ostringstream buffer;
                    buffer << "#FuzzyLite Interactive Console (press H for help)\n";
                    buffer << fldExporter.header(engine.get()) << "\n";
                    bool showCout = &writer != &std::cout;
                    writer << buffer.str();
                    if (showCout) std::cout << buffer.str();
                    else writer.flush();
                    interactive(writer, engine.get());
                }
            }
        } else {
            if ("fll" == outputFormat) {
                exporter.reset(new FllExporter);
            } else if ("fcl" == outputFormat) {
                exporter.reset(new FclExporter);
            } else if ("fis" == outputFormat) {
                exporter.reset(new FisExporter);
            } else if ("cpp" == outputFormat) {
                exporter.reset(new CppExporter);
            } else if ("java" == outputFormat) {
                exporter.reset(new JavaExporter);
            } else throw Exception("[export error] format <" + outputFormat + "> "
                    "not supported", FL_AT);
            writer << exporter->toString(engine.get());
        }
    }

    int Console::readCharacter() {
        int ch = 0;
#ifdef FL_UNIX
        struct termios oldt, newt;
        ::tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        ::tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = ::getchar();
        ::tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#elif defined(FL_WINDOWS)
        ch = ::_getch();
#endif
        return ch;
    }

    void Console::interactive(std::ostream& writer, Engine* engine) {
        std::ostringstream buffer;
        buffer << ">";
        bool showCout = &writer != &std::cout;
        const std::string space("\t");
        std::vector<scalar> inputValues;
        std::ostringstream inputValue;
        int ch = 0;
        do {
            writer << buffer.str();
            if (showCout) std::cout << buffer.str();
            else writer.flush();
            buffer.str("");

            ch = readCharacter();

            if (ch == EOF) {
                break;
            }

            if (std::isspace(ch)) {
                scalar value = engine->getInputVariable(inputValues.size())->getValue();
                try {
                    value = Op::toScalar(inputValue.str());
                } catch (std::exception& ex) {
                    FL_IUNUSED(ex);
                    buffer << "[" << Op::str(value) << "]";
                }
                buffer << space;
                inputValue.str("");
                inputValues.push_back(value);
                if (inputValues.size() == engine->inputVariables().size()) {
                    ch = 'P'; //fall through to process;
                } else continue;
            }

            if (not std::isgraph(ch)) continue;

            switch (ch) {
                default:
                    inputValue << char(ch);
                    buffer << char(ch);
                    break;
                case 'r':
                case 'R': engine->restart();
                    buffer << "#[Restart]";
                    continue; //fall through
                case 'd':
                case 'D': inputValues.clear();
                    buffer << "#[Discard]\n>";
                    inputValue.str("");
                    break;
                case 'p':
                case 'P': //Process
                {
                    inputValue.str("");

                    for (std::size_t i = 0; i < inputValues.size(); ++i) {
                        InputVariable* inputVariable = engine->inputVariables().at(i);
                        inputVariable->setValue(inputValues.at(i));
                    }
                    std::vector<scalar> missingInputs;
                    for (std::size_t i = inputValues.size(); i < engine->inputVariables().size(); ++i) {
                        InputVariable* inputVariable = engine->inputVariables().at(i);
                        missingInputs.push_back(inputVariable->getValue());
                    }
                    inputValues.clear();
                    buffer << Op::join(missingInputs, space);
                    if (not missingInputs.empty()) buffer << space;
                    buffer << "=" << space;
                    try {
                        engine->process();
                        std::vector<scalar> outputValues;
                        for (std::size_t i = 0; i < engine->outputVariables().size(); ++i) {
                            OutputVariable* outputVariable = engine->outputVariables().at(i);
                            outputVariable->defuzzify();
                            outputValues.push_back(outputVariable->getValue());
                        }
                        buffer << Op::join(outputValues, space) << "\n>";

                    } catch (std::exception& ex) {
                        buffer << "#[Error: " << ex.what() << "]";
                    }
                    break;
                }
                case 'q':
                case 'Q': buffer << "#[Quit]\n";
                    break;
                case 'h':
                case 'H': buffer << "\n>" << interactiveHelp() << "\n>";
                    inputValue.str("");
                    break;
            }
        } while (not (ch == 'Q' or ch == 'q' or ch == 4));
        writer << std::endl;
    }

    std::string Console::interactiveHelp() {
        return
        "#Special Keys\n"
        "#=============\n"
        "#\tR\tRestart engine and discard current inputs\n"
        "#\tD\tDiscard current inputs\n"
        "#\tP\tProcess engine\n"
        "#\tQ\tQuit interactive console\n"
        "#\tH\tShow this help\n"
        "#=============\n";
    }

    Engine* Console::mamdani() {
        Engine* engine = new Engine;
        engine->setName("simple-dimmer");
        engine->setDescription("");

        InputVariable* ambient = new InputVariable;
        ambient->setName("ambient");
        ambient->setDescription("");
        ambient->setEnabled(true);
        ambient->setRange(0.000, 1.000);
        ambient->setLockValueInRange(false);
        ambient->addTerm(new Triangle("DARK", 0.000, 0.250, 0.500));
        ambient->addTerm(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
        ambient->addTerm(new Triangle("BRIGHT", 0.500, 0.750, 1.000));
        engine->addInputVariable(ambient);

        OutputVariable* power = new OutputVariable;
        power->setName("power");
        power->setDescription("");
        power->setEnabled(true);
        power->setRange(0.000, 2.000);
        power->setLockValueInRange(false);
        power->setAggregation(new Maximum);
        power->setDefuzzifier(new Centroid(200));
        power->setDefaultValue(fl::nan);
        power->setLockPreviousValue(false);
        power->addTerm(new Triangle("LOW", 0.000, 0.500, 1.000));
        power->addTerm(new Triangle("MEDIUM", 0.500, 1.000, 1.500));
        power->addTerm(new Triangle("HIGH", 1.000, 1.500, 2.000));
        engine->addOutputVariable(power);

        RuleBlock* ruleBlock = new RuleBlock;
        ruleBlock->setName("");
        ruleBlock->setDescription("");
        ruleBlock->setEnabled(true);
        ruleBlock->setConjunction(fl::null);
        ruleBlock->setDisjunction(fl::null);
        ruleBlock->setImplication(new Minimum);
        ruleBlock->setActivation(new General);
        ruleBlock->addRule(Rule::parse("if ambient is DARK then power is HIGH", engine));
        ruleBlock->addRule(Rule::parse("if ambient is MEDIUM then power is MEDIUM", engine));
        ruleBlock->addRule(Rule::parse("if ambient is BRIGHT then power is LOW", engine));
        engine->addRuleBlock(ruleBlock);

        return engine;
    }

    Engine* Console::takagiSugeno() {
        Engine* engine = new Engine;
        engine->setName("approximation");
        engine->setDescription("approximation of sin(x)/x");

        InputVariable* inputX = new InputVariable;
        inputX->setName("inputX");
        inputX->setDescription("value of x");
        inputX->setEnabled(true);
        inputX->setRange(0.000, 10.000);
        inputX->setLockValueInRange(false);
        inputX->addTerm(new Triangle("NEAR_1", 0.000, 1.000, 2.000));
        inputX->addTerm(new Triangle("NEAR_2", 1.000, 2.000, 3.000));
        inputX->addTerm(new Triangle("NEAR_3", 2.000, 3.000, 4.000));
        inputX->addTerm(new Triangle("NEAR_4", 3.000, 4.000, 5.000));
        inputX->addTerm(new Triangle("NEAR_5", 4.000, 5.000, 6.000));
        inputX->addTerm(new Triangle("NEAR_6", 5.000, 6.000, 7.000));
        inputX->addTerm(new Triangle("NEAR_7", 6.000, 7.000, 8.000));
        inputX->addTerm(new Triangle("NEAR_8", 7.000, 8.000, 9.000));
        inputX->addTerm(new Triangle("NEAR_9", 8.000, 9.000, 10.000));
        engine->addInputVariable(inputX);

        OutputVariable* outputFx = new OutputVariable;
        outputFx->setName("outputFx");
        outputFx->setDescription("value of the approximation of x");
        outputFx->setEnabled(true);
        outputFx->setRange(-1.000, 1.000);
        outputFx->setLockValueInRange(false);
        outputFx->setAggregation(fl::null);
        outputFx->setDefuzzifier(new WeightedAverage("Automatic"));
        outputFx->setDefaultValue(fl::nan);
        outputFx->setLockPreviousValue(true);
        outputFx->addTerm(new Constant("f1", 0.840));
        outputFx->addTerm(new Constant("f2", 0.450));
        outputFx->addTerm(new Constant("f3", 0.040));
        outputFx->addTerm(new Constant("f4", -0.180));
        outputFx->addTerm(new Constant("f5", -0.190));
        outputFx->addTerm(new Constant("f6", -0.040));
        outputFx->addTerm(new Constant("f7", 0.090));
        outputFx->addTerm(new Constant("f8", 0.120));
        outputFx->addTerm(new Constant("f9", 0.040));
        engine->addOutputVariable(outputFx);

        OutputVariable* trueValue = new OutputVariable;
        trueValue->setName("trueValue");
        trueValue->setDescription("value of f(x)=sin(x)/x");
        trueValue->setEnabled(true);
        trueValue->setRange(-1.060, 1.000);
        trueValue->setLockValueInRange(false);
        trueValue->setAggregation(fl::null);
        trueValue->setDefuzzifier(new WeightedAverage("Automatic"));
        trueValue->setDefaultValue(fl::nan);
        trueValue->setLockPreviousValue(true);
        trueValue->addTerm(Function::create("fx", "sin(inputX)/inputX", engine));
        engine->addOutputVariable(trueValue);

        OutputVariable* difference = new OutputVariable;
        difference->setName("difference");
        difference->setDescription("error e=f(x) - f'(x)");
        difference->setEnabled(true);
        difference->setRange(-1.000, 1.000);
        difference->setLockValueInRange(false);
        difference->setAggregation(fl::null);
        difference->setDefuzzifier(new WeightedAverage("Automatic"));
        difference->setDefaultValue(fl::nan);
        difference->setLockPreviousValue(false);
        difference->addTerm(Function::create("error", "outputFx-trueValue", engine));
        engine->addOutputVariable(difference);

        RuleBlock* ruleBlock = new RuleBlock;
        ruleBlock->setName("");
        ruleBlock->setDescription("");
        ruleBlock->setEnabled(true);
        ruleBlock->setConjunction(fl::null);
        ruleBlock->setDisjunction(fl::null);
        ruleBlock->setImplication(new AlgebraicProduct);
        ruleBlock->setActivation(new General);
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_1 then outputFx is f1", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_2 then outputFx is f2", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_3 then outputFx is f3", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_4 then outputFx is f4", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_5 then outputFx is f5", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_6 then outputFx is f6", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_7 then outputFx is f7", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_8 then outputFx is f8", engine));
        ruleBlock->addRule(Rule::parse("if inputX is NEAR_9 then outputFx is f9", engine));
        ruleBlock->addRule(Rule::parse("if inputX is any then trueValue is fx and difference is error", engine));
        engine->addRuleBlock(ruleBlock);

        return engine;
    }

    Engine* Console::hybrid() {
        Engine* engine = new Engine;
        engine->setName("tipper");
        engine->setDescription("(service and food) -> (tip)");

        InputVariable* service = new InputVariable;
        service->setName("service");
        service->setDescription("quality of service");
        service->setEnabled(true);
        service->setRange(0.000, 10.000);
        service->setLockValueInRange(true);
        service->addTerm(new Trapezoid("poor", 0.000, 0.000, 2.500, 5.000));
        service->addTerm(new Triangle("good", 2.500, 5.000, 7.500));
        service->addTerm(new Trapezoid("excellent", 5.000, 7.500, 10.000, 10.000));
        engine->addInputVariable(service);

        InputVariable* food = new InputVariable;
        food->setName("food");
        food->setDescription("quality of food");
        food->setEnabled(true);
        food->setRange(0.000, 10.000);
        food->setLockValueInRange(true);
        food->addTerm(new Trapezoid("rancid", 0.000, 0.000, 2.500, 7.500));
        food->addTerm(new Trapezoid("delicious", 2.500, 7.500, 10.000, 10.000));
        engine->addInputVariable(food);

        OutputVariable* mTip = new OutputVariable;
        mTip->setName("mTip");
        mTip->setDescription("tip based on Mamdani inference");
        mTip->setEnabled(true);
        mTip->setRange(0.000, 30.000);
        mTip->setLockValueInRange(false);
        mTip->setAggregation(new Maximum);
        mTip->setDefuzzifier(new Centroid(100));
        mTip->setDefaultValue(fl::nan);
        mTip->setLockPreviousValue(false);
        mTip->addTerm(new Triangle("cheap", 0.000, 5.000, 10.000));
        mTip->addTerm(new Triangle("average", 10.000, 15.000, 20.000));
        mTip->addTerm(new Triangle("generous", 20.000, 25.000, 30.000));
        engine->addOutputVariable(mTip);

        OutputVariable* tsTip = new OutputVariable;
        tsTip->setName("tsTip");
        tsTip->setDescription("tip based on Takagi-Sugeno inference");
        tsTip->setEnabled(true);
        tsTip->setRange(0.000, 30.000);
        tsTip->setLockValueInRange(false);
        tsTip->setAggregation(fl::null);
        tsTip->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
        tsTip->setDefaultValue(fl::nan);
        tsTip->setLockPreviousValue(false);
        tsTip->addTerm(new Constant("cheap", 5.000));
        tsTip->addTerm(new Constant("average", 15.000));
        tsTip->addTerm(new Constant("generous", 25.000));
        engine->addOutputVariable(tsTip);

        RuleBlock* mamdaniRuleBlock = new RuleBlock;
        mamdaniRuleBlock->setName("mamdani");
        mamdaniRuleBlock->setDescription("Mamdani inference");
        mamdaniRuleBlock->setEnabled(true);
        mamdaniRuleBlock->setConjunction(new AlgebraicProduct);
        mamdaniRuleBlock->setDisjunction(new AlgebraicSum);
        mamdaniRuleBlock->setImplication(new Minimum);
        mamdaniRuleBlock->setActivation(new General);
        mamdaniRuleBlock->addRule(Rule::parse("if service is poor or food is rancid then mTip is cheap", engine));
        mamdaniRuleBlock->addRule(Rule::parse("if service is good then mTip is average", engine));
        mamdaniRuleBlock->addRule(Rule::parse("if service is excellent or food is delicious then mTip is generous with 0.5", engine));
        mamdaniRuleBlock->addRule(Rule::parse("if service is excellent and food is delicious then mTip is generous with 1.0", engine));
        engine->addRuleBlock(mamdaniRuleBlock);

        RuleBlock* takagiSugenoRuleBlock = new RuleBlock;
        takagiSugenoRuleBlock->setName("takagiSugeno");
        takagiSugenoRuleBlock->setDescription("Takagi-Sugeno inference");
        takagiSugenoRuleBlock->setEnabled(true);
        takagiSugenoRuleBlock->setConjunction(new AlgebraicProduct);
        takagiSugenoRuleBlock->setDisjunction(new AlgebraicSum);
        takagiSugenoRuleBlock->setImplication(fl::null);
        takagiSugenoRuleBlock->setActivation(new General);
        takagiSugenoRuleBlock->addRule(Rule::parse("if service is poor or food is rancid then tsTip is cheap", engine));
        takagiSugenoRuleBlock->addRule(Rule::parse("if service is good then tsTip is average", engine));
        takagiSugenoRuleBlock->addRule(Rule::parse("if service is excellent or food is delicious then tsTip is generous with 0.5", engine));
        takagiSugenoRuleBlock->addRule(Rule::parse("if service is excellent and food is delicious then tsTip is generous with 1.0", engine));
        engine->addRuleBlock(takagiSugenoRuleBlock);

        return engine;
    }

    void Console::exportAllExamples(const std::string& from, const std::string& to) {
        Console::exportAllExamples(from, to, "./", "/tmp/");
    }

    void Console::exportAllExamples(const std::string& from, const std::string& to,
            const std::string& sourcePath, const std::string& targetPath) {
        std::vector<std::string> examples;
        examples.push_back("mamdani/AllTerms");
        examples.push_back("mamdani/SimpleDimmer");
        examples.push_back("mamdani/Laundry");
        examples.push_back("mamdani/ObstacleAvoidance");
        examples.push_back("mamdani/SimpleDimmerChained");
        examples.push_back("mamdani/SimpleDimmerInverse");
        examples.push_back("mamdani/matlab/mam21");
        examples.push_back("mamdani/matlab/mam22");
        examples.push_back("mamdani/matlab/shower");
        examples.push_back("mamdani/matlab/tank");
        examples.push_back("mamdani/matlab/tank2");
        examples.push_back("mamdani/matlab/tipper");
        examples.push_back("mamdani/matlab/tipper1");
        examples.push_back("mamdani/octave/investment_portfolio");
        examples.push_back("mamdani/octave/mamdani_tip_calculator");
        examples.push_back("takagi-sugeno/approximation");
        examples.push_back("takagi-sugeno/ObstacleAvoidance");
        examples.push_back("takagi-sugeno/SimpleDimmer");
        examples.push_back("takagi-sugeno/matlab/fpeaks");
        examples.push_back("takagi-sugeno/matlab/invkine1");
        examples.push_back("takagi-sugeno/matlab/invkine2");
        examples.push_back("takagi-sugeno/matlab/juggler");
        examples.push_back("takagi-sugeno/matlab/membrn1");
        examples.push_back("takagi-sugeno/matlab/membrn2");
        examples.push_back("takagi-sugeno/matlab/slbb");
        examples.push_back("takagi-sugeno/matlab/slcp");
        examples.push_back("takagi-sugeno/matlab/slcp1");
        examples.push_back("takagi-sugeno/matlab/slcpp1");
        examples.push_back("takagi-sugeno/matlab/sltbu_fl");
        examples.push_back("takagi-sugeno/matlab/sugeno1");
        examples.push_back("takagi-sugeno/matlab/tanksg");
        examples.push_back("takagi-sugeno/matlab/tippersg");
        examples.push_back("takagi-sugeno/octave/cubic_approximator");
        examples.push_back("takagi-sugeno/octave/heart_disease_risk");
        examples.push_back("takagi-sugeno/octave/linear_tip_calculator");
        examples.push_back("takagi-sugeno/octave/sugeno_tip_calculator");
        examples.push_back("tsukamoto/tsukamoto");
        examples.push_back("hybrid/tipper");
        examples.push_back("hybrid/ObstacleAvoidance");

        FL_unique_ptr<Importer> importer;
        if (from == "fll") importer.reset(new FllImporter);
        else if (from == "fis") importer.reset(new FisImporter);
        else if (from == "fcl") importer.reset(new FclImporter);
        else throw Exception("[examples error] unrecognized format <" + from + "> to import", FL_AT);

        FL_unique_ptr<Exporter> exporter;
        if (to == "fll") exporter.reset(new FllExporter);
        else if (to == "fld") exporter.reset(new FldExporter(" "));
        else if (to == "fcl") exporter.reset(new FclExporter);
        else if (to == "fis") exporter.reset(new FisExporter);
        else if (to == "cpp") exporter.reset(new CppExporter);
        else if (to == "java") exporter.reset(new JavaExporter);
        else if (to == "R") exporter.reset(new RScriptExporter());
        else throw Exception("[examples error] unrecognized format <" + to + "> to export", FL_AT);

        std::vector<std::pair<Exporter*, Importer*> > tests;
        tests.push_back(std::pair<Exporter*, Importer*>(new FllExporter, new FllImporter));
        tests.push_back(std::pair<Exporter*, Importer*>(new FisExporter, new FisImporter));
        tests.push_back(std::pair<Exporter*, Importer*>(new FclExporter, new FclImporter));
        for (std::size_t i = 0; i < examples.size(); ++i) {
            std::string example = examples.at(i);
            FL_LOG((i + 1) << "/" << examples.size());
            FL_LOG("Importing from: " << sourcePath << "/" << example << "." << from);
            std::ostringstream ss;
            std::string input = sourcePath + "/" + example + "." + from;
            std::ifstream source(input.c_str());
            if (source.is_open()) {
                std::string line;
                while (source.good()) {
                    std::getline(source, line);
                    ss << line << "\n";
                }
                source.close();
            } else throw Exception("[examples error] file not found: " + input, FL_AT);

            FL_unique_ptr<Engine> engine(importer->fromString(ss.str()));

            for (std::size_t t = 0; t < tests.size(); ++t) {
                if ("mamdani/Laundry" == example
                        or "mamdani/SimpleDimmerInverse" == example
                        or "mamdani/SimpleDimmerChained" == example
                        or "hybrid/tipper" == example
                        or "hybrid/ObstacleAvoidance" == example) {
                    if (tests.at(t).second->name() == FisImporter().name()) {
                        continue;
                    }
                }
                
                std::string exported = tests.at(t).first->toString(engine.get());
                FL_unique_ptr<Engine> engineFromExport(tests.at(t).second->fromString(exported));
                std::string imported = tests.at(t).first->toString(engineFromExport.get());

                if (exported != imported) {
                    std::ostringstream msg;
                    msg << "[imex error] different results <"
                            << tests.at(t).first->name() << "," << tests.at(t).second->name() << "> "
                            "at " << example << "." << from << ":\n";
                    msg << "<Engine A>\n" << exported << "\n\n" <<
                            "================================\n\n" <<
                            "<Engine B>\n" << imported;
                    throw Exception(msg.str(), FL_AT);
                }
            }

            std::string output = targetPath + "/" + example + "." + to;
            std::ofstream target(output.c_str());
            FL_LOG("Exporting to: " << output << "\n");
            if (target.is_open()) {
                if (to == "cpp") {
                    target << "#include <fl/Headers.h>\n\n"
                            << "int main(int argc, char** argv){\n"
                            << exporter->toString(engine.get())
                            << "\n}\n";
                } else if (to == "java") {
                    std::string className = example.substr(example.find_last_of('/') + 1);
                    target << "import com.fuzzylite.*;\n"
                            << "import com.fuzzylite.activation.*\n"
                            << "import com.fuzzylite.defuzzifier.*;\n"
                            << "import com.fuzzylite.factory.*;\n"
                            << "import com.fuzzylite.hedge.*;\n"
                            << "import com.fuzzylite.imex.*;\n"
                            << "import com.fuzzylite.norm.*;\n"
                            << "import com.fuzzylite.norm.s.*;\n"
                            << "import com.fuzzylite.norm.t.*;\n"
                            << "import com.fuzzylite.rule.*;\n"
                            << "import com.fuzzylite.term.*;\n"
                            << "import com.fuzzylite.variable.*;\n\n"
                            << "public class " << Op::validName(className) << "{\n"
                            << "public static void main(String[] args){\n"
                            << exporter->toString(engine.get())
                            << "\n}\n}\n";
                } else if (to == "R") {
                    RScriptExporter* rScript = dynamic_cast<RScriptExporter*> (exporter.get());
                    InputVariable* a = engine->getInputVariable(0);
                    InputVariable* b = engine->getInputVariable(1 % engine->numberOfInputVariables());
                    std::string pathToDF = example.substr(example.find_last_of('/') + 1) + ".fld";
                    rScript->writeScriptImportingDataFrame(engine.get(), target,
                            a, b, pathToDF, engine->outputVariables());
                } else {
                    target << exporter->toString(engine.get());
                }
                target.close();
            }
            Engine copyConstructor(*engine.get());
            FL_IUNUSED(copyConstructor);
            Engine assignmentOperator = *engine.get();
            FL_IUNUSED(assignmentOperator);
        }
        FL_LOG("Please, make sure the output contains the following structure:\n"
                "mkdir -p " << targetPath << "mamdani/matlab; "
                "mkdir -p " << targetPath << "mamdani/octave; "
                "mkdir -p " << targetPath << "takagi-sugeno/matlab; "
                "mkdir -p " << targetPath << "takagi-sugeno/octave; "
                "mkdir -p " << targetPath << "tsukamoto; "
                "mkdir -p " << targetPath << "hybrid;");
        for (std::size_t i = 0; i < tests.size(); ++i) {
            delete tests.at(i).first;
            delete tests.at(i).second;
        }
    }

    void Console::benchmark(const std::string& fllFile, const std::string& fldFile,
            int runs, std::ofstream* writer) const {
        FL_unique_ptr<Engine> engine(FllImporter().fromFile(fllFile));
        std::ifstream reader(fldFile.c_str());
        if (not reader.is_open()) {
            throw Exception("File <" + fldFile + "> could not be opened");
        }
        Benchmark benchmark(engine->getName(), engine.get());
        benchmark.prepare(reader);
        if (writer) {
            FL_LOG("\tEvaluating on " << benchmark.getExpected().size() <<
                    " values read from " << fldFile << " ...");
        }
        for (int i = 0; i < runs; ++i) {
            benchmark.runOnce();
        }
        if (writer) {
            FL_LOG("\tMean(t)=" << Op::mean(benchmark.getTimes()) << " nanoseconds");
            *writer << benchmark.format(benchmark.results(),
                    Benchmark::Horizontal, Benchmark::Body) << "\n";
        } else {
            FL_LOGP(benchmark.format(benchmark.results(),
                    Benchmark::Horizontal, Benchmark::Body));
        }
    }

    void Console::benchmarks(const std::string& fllFileList,
            const std::string& fldFileList, int runs, std::ofstream* writer) const {
        std::vector<std::string> fllFiles, fldFiles;

        {
            std::ifstream fllReader(fllFileList.c_str());
            if (not fllReader.is_open()) {
                throw Exception("[error] file <" + fllFileList + "> could not be opened");
            }
            std::ifstream fldReader(fldFileList.c_str());
            if (not fldReader.is_open()) {
                throw Exception("[error] file <" + fldFileList + "> could not be opened");
            }

            std::string fllLine, fldLine;
            while (std::getline(fllReader, fllLine) and std::getline(fldReader, fldLine)) {
                fllLine = Op::trim(fllLine);
                fldLine = Op::trim(fldLine);
                if (fllLine.empty() or fllLine[0] == '#')
                    continue;
                fllFiles.push_back(fllLine);
                fldFiles.push_back(fldLine);
            }
        }

        if (writer) {
            *writer << Op::join(Benchmark().header(runs, true), "\t") << "\n";
        } else {
            FL_LOGP(Op::join(Benchmark().header(runs, true), "\t"));
        }

        for (std::size_t i = 0; i < fllFiles.size(); ++i) {
            if (writer) {
                FL_LOG("Benchmark " << (i + 1) << "/" << fllFiles.size() << ": "
                        << fllFiles.at(i));
            }
            benchmark(fllFiles.at(i), fldFiles.at(i), runs, writer);
        }
    }

    int Console::main(int argc, const char* argv[]) {
        fuzzylite::setLogging(true);

        Console console;
        if (argc <= 2) {
            FL_LOGP(console.usage() << "\n");
            return EXIT_SUCCESS;
        }

        const std::string firstArgument(argv[1]);
        if (firstArgument == "export-examples") {
            std::string path = ".";
            if (argc > 2) {
                path = std::string(argv[2]);
            }
            std::string outputPath = "/tmp/";
            if (argc > 3) {
                outputPath = std::string(argv[3]);
            }
            FL_LOG("Origin=" << path);
            FL_LOG("Target=" << outputPath);
            fuzzylite::setDecimals(3);
            try {
                FL_LOG("Processing fll->fll");
                console.exportAllExamples("fll", "fll", path, outputPath);
                FL_LOG("Processing fll->fcl");
                console.exportAllExamples("fll", "fcl", path, outputPath);
                FL_LOG("Processing fll->fis");
                console.exportAllExamples("fll", "fis", path, outputPath);
                FL_LOG("Processing fll->cpp");
                console.exportAllExamples("fll", "cpp", path, outputPath);
                FL_LOG("Processing fll->java");
                console.exportAllExamples("fll", "java", path, outputPath);
                FL_LOG("Processing fll->R");
                console.exportAllExamples("fll", "R", path, outputPath);
                fuzzylite::setDecimals(9);
                FL_LOG("Processing fll->fld");
                console.exportAllExamples("fll", "fld", path, outputPath);
                FL_LOG("Origin=" << path);
                FL_LOG("Target=" << outputPath);
            } catch (std::exception& ex) {
                FL_LOGP(ex.what() << "\n");
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;

        } else if (firstArgument == "benchmark") {
            if (argc < 5) {
                FL_LOG("[error] not enough parameters");
                return EXIT_FAILURE;
            }
            std::string fllFile(argv[2]);
            std::string fldFile(argv[3]);
            try {
                int runs = (int) Op::toScalar(argv[4]);
                if (argc > 5) {
                    std::string filename(argv[5]);
                    std::ofstream outputFile;
                    outputFile.open(filename.c_str());
                    if (not outputFile.is_open()) {
                        FL_LOG("[error] cannot create file <" << filename << ">");
                        return EXIT_FAILURE;
                    }
                    outputFile << Op::join(Benchmark().header(runs, true), "\t") << "\n";
                    console.benchmark(fllFile, fldFile, runs, &outputFile);
                } else {
                    FL_LOGP(Op::join(Benchmark().header(runs, true), "\t"));
                    console.benchmark(fllFile, fldFile, runs, fl::null);
                }
            } catch (std::exception& ex) {
                FL_LOGP(ex.what() << "\n");
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;

        } else if (firstArgument == "benchmarks") {
            if (argc < 5) {
                FL_LOG("[error] not enough parameters");
                return EXIT_FAILURE;
            }
            std::string fllFiles(argv[2]);
            std::string fldFiles(argv[3]);
            try {
                int runs = (int) Op::toScalar(argv[4]);
                if (argc > 5) {
                    std::string filename(argv[5]);
                    std::ofstream outputFile;
                    outputFile.open(filename.c_str());
                    if (not outputFile.is_open()) {
                        FL_LOG("[error] cannot create file <" << filename << ">");
                        return EXIT_FAILURE;
                    }
                    console.benchmarks(fllFiles, fldFiles, runs, &outputFile);
                } else {
                    console.benchmarks(fllFiles, fldFiles, runs, fl::null);
                }
            } catch (std::exception& ex) {
                FL_LOGP(ex.what() << "\n");
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }

        //MAIN:
        try {
            std::map<std::string, std::string> options = console.parse(argc, argv);
            console.process(options);

        } catch (std::exception& ex) {
            FL_LOGP(ex.what() << "\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

}
