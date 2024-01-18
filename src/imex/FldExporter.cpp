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

#include "fl/imex/FldExporter.h"

#include "fl/Engine.h"
#include "fl/Operation.h"
#include "fl/variable/Variable.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include <fstream>

namespace fl {

    FldExporter::FldExporter(const std::string& separator) : Exporter(),
    _separator(separator), _exportHeaders(true),
    _exportInputValues(true), _exportOutputValues(true) { }

    FldExporter::~FldExporter() { }

    std::string FldExporter::name() const {
        return "FldExporter";
    }

    void FldExporter::setSeparator(const std::string& separator) {
        this->_separator = separator;
    }

    std::string FldExporter::getSeparator() const {
        return this->_separator;
    }

    void FldExporter::setExportHeader(bool exportHeaders) {
        this->_exportHeaders = exportHeaders;
    }

    bool FldExporter::exportsHeader() const {
        return this->_exportHeaders;
    }

    void FldExporter::setExportInputValues(bool exportInputValues) {
        this->_exportInputValues = exportInputValues;
    }

    bool FldExporter::exportsInputValues() const {
        return this->_exportInputValues;
    }

    void FldExporter::setExportOutputValues(bool exportOutputValues) {
        this->_exportOutputValues = exportOutputValues;
    }

    bool FldExporter::exportsOutputValues() const {
        return this->_exportOutputValues;
    }

    std::string FldExporter::header(const Engine* engine) const {
        std::vector<std::string> result;
        if (_exportInputValues) {
            for (std::size_t i = 0; i < engine->numberOfInputVariables(); ++i) {
                InputVariable* inputVariable = engine->getInputVariable(i);
                result.push_back(inputVariable->getName());
            }
        }
        if (_exportOutputValues) {
            for (std::size_t i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* outputVariable = engine->getOutputVariable(i);
                result.push_back(outputVariable->getName());
            }
        }
        return Op::join(result, _separator);
    }

    std::string FldExporter::toString(const Engine* engine) const {
        return toString(const_cast<Engine*> (engine), 1024, AllVariables);
    }

    std::string FldExporter::toString(Engine* engine, int values, ScopeOfValues scope) const {
        return toString(engine, values, scope, engine->inputVariables());
    }

    std::string FldExporter::toString(Engine* engine, int values, ScopeOfValues scope,
            const std::vector<InputVariable*>& activeVariables) const {
        std::ostringstream result;
        write(engine, result, values, scope, activeVariables);
        return result.str();
    }

    std::string FldExporter::toString(Engine* engine, std::istream& reader) const {
        std::ostringstream writer;
        if (_exportHeaders) writer << header(engine) << "\n";
        std::string line;
        int lineNumber = 0;
        while (std::getline(reader, line)) {
            ++lineNumber;
            line = Op::trim(line);
            if (not line.empty() and line.at(0) == '#')
                continue; //comments are ignored, blank lines are retained
            std::vector<scalar> inputValues;
            if (lineNumber == 1) { //automatic detection of header.
                try {
                    inputValues = parse(line);
                } catch (std::exception&) {
                    continue;
                }
            } else {
                inputValues = parse(line);
            }

            write(engine, writer, inputValues, engine->inputVariables());
        }
        return writer.str();
    }

    void FldExporter::toFile(const std::string& path, Engine* engine, int values, ScopeOfValues scope) const {
        toFile(path, engine, values, scope, engine->inputVariables());
    }

    void FldExporter::toFile(const std::string& path, Engine* engine, int values, ScopeOfValues scope,
            const std::vector<InputVariable*>& activeVariables) const {
        std::ofstream writer(path.c_str());
        if (not writer.is_open()) {
            throw Exception("[file error] file <" + path + "> could not be created", FL_AT);
        }
        write(engine, writer, values, scope, activeVariables);
        writer.close();
    }

    void FldExporter::toFile(const std::string& path, Engine* engine, std::istream& reader) const {
        std::ofstream writer(path.c_str());
        if (not writer.is_open()) {
            throw Exception("[file error] file <" + path + "> could not be created", FL_AT);
        }
        if (_exportHeaders) writer << header(engine) << "\n";

        std::string line;
        int lineNumber = 0;
        while (std::getline(reader, line)) {
            ++lineNumber;
            line = Op::trim(line);
            if (not line.empty() and line.at(0) == '#')
                continue; //comments are ignored, blank lines are retained
            std::vector<scalar> inputValues;
            if (lineNumber == 1) { //automatic detection of header.
                try {
                    inputValues = parse(line);
                } catch (std::exception&) {
                    continue;
                }
            } else {
                inputValues = parse(line);
            }

            write(engine, writer, inputValues, engine->inputVariables());
        }
        writer.close();
    }

    std::vector<scalar> FldExporter::parse(const std::string& values) const {
        std::vector<scalar> inputValues;
        if (not (values.empty() or values.at(0) == '#')) {
            inputValues = Op::toScalars(values);
        }
        return inputValues;
    }

    void FldExporter::write(Engine* engine, std::ostream& writer, int values, ScopeOfValues scope) const {
        write(engine, writer, values, scope, engine->inputVariables());
    }

    void FldExporter::write(Engine* engine, std::ostream& writer,
            int values, ScopeOfValues scope,
            const std::vector<InputVariable*>& activeVariables) const {
        if (_exportHeaders) writer << header(engine) << "\n";

        if (activeVariables.size() != engine->inputVariables().size()) {
            std::ostringstream ex;
            ex << "[exporter error] number of active variables "
                    "<" << activeVariables.size() << ">"
                    << "must match the number of input variables in the engine "
                    "<" << engine->inputVariables().size() << ">";
            throw Exception(ex.str(), FL_AT);
        }

        int resolution;
        if (scope == AllVariables)
            resolution = -1 + (int) std::max(1.0, std::pow(
                values, 1.0 / engine->numberOfInputVariables()));
        else //if (scope == EachVariable)
            resolution = values - 1;

        std::vector<int> sampleValues, minSampleValues, maxSampleValues;
        for (std::size_t i = 0; i < engine->numberOfInputVariables(); ++i) {
            sampleValues.push_back(0);
            minSampleValues.push_back(0);
            if (engine->inputVariables().at(i) == activeVariables.at(i))
                maxSampleValues.push_back(resolution);
            else maxSampleValues.push_back(0);
        }

        std::vector<scalar> inputValues(engine->numberOfInputVariables());
        do {
            for (std::size_t i = 0; i < engine->numberOfInputVariables(); ++i) {
                InputVariable* inputVariable = engine->getInputVariable(i);
                if (inputVariable == activeVariables.at(i)) {
                    inputValues.at(i) = inputVariable->getMinimum()
                            + sampleValues.at(i) * inputVariable->range() / std::max(1, resolution);
                } else {
                    inputValues.at(i) = inputVariable->getValue();
                }
            }
            write(engine, writer, inputValues, activeVariables);
        } while (Op::increment(sampleValues, minSampleValues, maxSampleValues));
    }

    void FldExporter::write(Engine* engine, std::ostream& writer, std::istream& reader) const {
        if (_exportHeaders) writer << header(engine) << "\n";

        std::string line;
        std::size_t lineNumber = 0;
        while (std::getline(reader, line)) {
            ++lineNumber;
            line = Op::trim(line);
            if (not line.empty() and line.at(0) == '#')
                continue; //comments are ignored, blank lines are retained
            std::vector<scalar> inputValues;
            if (lineNumber == 1) { //automatic detection of header.
                try {
                    inputValues = parse(line);
                } catch (std::exception&) {
                    continue;
                }
            } else {
                inputValues = parse(line);
            }
            try {
                write(engine, writer, inputValues, engine->inputVariables());
            } catch (Exception& ex) {
                ex.append(" writing line <" + Op::str(lineNumber) + ">");
                throw;
            }
        }
    }

    void FldExporter::write(Engine* engine, std::ostream& writer,
            const std::vector<scalar>& inputValues) const {
        write(engine, writer, inputValues, engine->inputVariables());
    }

    void FldExporter::write(Engine* engine, std::ostream& writer,
            const std::vector<scalar>& inputValues,
            const std::vector<InputVariable*>& activeVariables) const {
        if (inputValues.empty()) {
            writer << "\n";
            return;
        }
        if (inputValues.size() < engine->numberOfInputVariables()) {
            std::ostringstream ex;
            ex << "[export error] engine has <" << engine->numberOfInputVariables() << "> "
                    "input variables, but input data provides <" << inputValues.size() << "> values";
            throw Exception(ex.str(), FL_AT);
        }
        if (activeVariables.size() != engine->inputVariables().size()) {
            std::ostringstream ex;
            ex << "[exporter error] number of active variables <" << activeVariables.size() << "> "
                    "must match the number of input variables in the engine <" << engine->inputVariables().size() << ">";
            throw Exception(ex.str(), FL_AT);
        }

        std::vector<scalar> values;
        for (std::size_t i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* inputVariable = engine->getInputVariable(i);
            scalar inputValue;
            if (inputVariable == activeVariables.at(i)) {
                inputValue = inputValues.at(i);
            } else {
                inputValue = inputVariable->getValue();
            }
            inputVariable->setValue(inputValue);
            if (_exportInputValues) values.push_back(inputValue);
        }

        engine->process();

        for (std::size_t i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            if (_exportOutputValues)
                values.push_back(outputVariable->getValue());
        }

        writer << Op::join(values, _separator) << "\n";
    }

    FldExporter* FldExporter::clone() const {
        return new FldExporter(*this);
    }

}
