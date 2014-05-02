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
 * File:   DataExporter.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 9:54 AM
 */

#include "fl/imex/FldExporter.h"

#include "fl/Engine.h"
#include "fl/variable/Variable.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include <cmath>
#include <fstream>
#include <vector>

namespace fl {

    FldExporter::FldExporter(const std::string& separator)
    : _separator(separator) {

    }

    FldExporter::~FldExporter() {
    }

    std::string FldExporter::name() const {
        return "FldExporter";
    }

    void FldExporter::setSeparator(const std::string& separator) {
        this->_separator = separator;
    }

    std::string FldExporter::getSeparator() const {
        return this->_separator;
    }

    std::string FldExporter::header(const Engine* engine) const {
        std::string inputHeader = header(engine->constInputVariables());
        std::string outputHeader = header(engine->constOutputVariables());
        std::ostringstream result;
        result << "@Engine: " << engine->getName() << ";";
        if (not inputHeader.empty()) {
            result << _separator << inputHeader;
        }
        if (not outputHeader.empty()) {
            result << _separator << outputHeader;
        }
        return result.str();
    }

    std::string FldExporter::header(const std::vector<InputVariable*>& inputVariables) const {
        std::vector<std::string> result;
        for (std::size_t i = 0; i < inputVariables.size(); ++i) {
            InputVariable* inputVariable = inputVariables.at(i);
            if (inputVariable->isEnabled()) {
                result.push_back("@InputVariable: " + inputVariable->getName() + ";");
            }
        }
        return fl::Op::join(result, _separator);
    }

    std::string FldExporter::header(const std::vector<OutputVariable*>& outputVariables) const {
        std::vector<std::string> result;
        for (std::size_t i = 0; i < outputVariables.size(); ++i) {
            OutputVariable* outputVariable = outputVariables.at(i);
            if (outputVariable->isEnabled()) {
                result.push_back("@OutputVariable: " + outputVariable->getName() + ";");
            }
        }
        return fl::Op::join(result, _separator);
    }

    std::string FldExporter::toString(const Engine* mutableEngine) const {
        return toString(mutableEngine, 1024);
    }

    std::string FldExporter::toString(const Engine* mutableEngine, int maximumNumberOfResults,
            bool includeHeaders, bool includeInputValues) const {
        Engine* engine = const_cast<Engine*> (mutableEngine);
        std::ostringstream result;
        if (includeHeaders) {
            if (includeInputValues) {
                result << "#" << header(engine) << "\n";
            } else {
                result << "#" << header(engine->constOutputVariables()) << "\n";
            }
        }
        toWriter(engine, result, maximumNumberOfResults, _separator, includeInputValues);
        return result.str();
    }

    template <typename T>
    void FldExporter::toWriter(Engine* engine, T& writer, int maximum,
            const std::string& separator, bool includeInputValues) const {
        int resolution = -1 + (int) std::max(1.0, std::pow(
                maximum, 1.0 / engine->numberOfInputVariables()));
        std::vector<int> sampleValues, minSampleValues, maxSampleValues;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            sampleValues.push_back(0);
            minSampleValues.push_back(0);
            maxSampleValues.push_back(resolution);
        }

        engine->restart();

        bool overflow = false;
        while (not overflow) {
            std::vector<std::string> values;

            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                InputVariable* inputVariable = engine->getInputVariable(i);
                if (inputVariable->isEnabled()) {
                    scalar inputValue = inputVariable->getMinimum()
                            + sampleValues.at(i) * inputVariable->range() / resolution;
                    inputVariable->setInputValue(inputValue);
                    if (includeInputValues) {
                        values.push_back(Op::str(inputValue));
                    }
                }
            }

            engine->process();

            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* outputVariable = engine->getOutputVariable(i);
                outputVariable->defuzzify();
                if (outputVariable->isEnabled()) {
                    values.push_back(Op::str(outputVariable->getOutputValue()));
                }
            }

            writer << Op::join(values, separator) << "\n";
            writer.flush();

            overflow = Op::increment(sampleValues, minSampleValues, maxSampleValues);
        }
    }

    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostream& writer,
            int maximumNumberOfResults, const std::string& separator, bool includeInputValues) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ofstream& writer,
            int maximumNumberOfResults, const std::string& separator, bool includeInputValues) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostringstream& writer,
            int maximumNumberOfResults, const std::string& separator, bool includeInputValues) const;

    std::string FldExporter::toString(const Engine* mutableEngine, const std::string& inputData,
            bool headers, bool includeInputValues) const {
        Engine* engine = const_cast<Engine*> (mutableEngine);
        std::ostringstream writer;
        if (headers) {
            if (includeInputValues) {
                writer << "#" << header(engine) << "\n";
            } else {
                writer << "#" << header(engine->constOutputVariables()) << "\n";
            }
        }
        std::istringstream reader(inputData);
        std::string line;
        int lineNumber = 0;
        while (std::getline(reader, line)) {
            ++lineNumber;
            std::vector<scalar> inputValues;
            parse(Op::trim(line), inputValues);
            if (inputValues.empty()) continue;
            if ((int) inputValues.size() != engine->numberOfInputVariables()) {
                std::ostringstream ex;
                ex << "[export error] engine has <" << engine->numberOfInputVariables() << "> "
                        "input variables, but input data provides <" << inputValues.size() << "> values "
                        "at line <" << lineNumber << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }
            toWriter(engine, writer, inputValues, _separator, includeInputValues);
            writer << "\n";
            writer.flush();
        }
        return writer.str();
    }

    void FldExporter::parse(const std::string& x, std::vector<scalar>& inputValues) const {
        if (x.empty() or x.at(0) == '#') return;
        std::istringstream tokenizer(x);
        std::string token;
        while (tokenizer >> token) {
            inputValues.push_back(fl::Op::toScalar(token));
        }
    }

    template <typename T>
    void FldExporter::toWriter(Engine* engine, T& writer, const std::vector<scalar>& inputValues,
            const std::string& separator, bool includeInputValues) const {
        std::vector<scalar> values;
        for (std::size_t i = 0; i < inputValues.size(); ++i) {
            InputVariable* inputVariable = engine->getInputVariable(i);
            if (inputVariable->isEnabled()) {
                scalar inputValue = inputValues.at(i);
                inputVariable->setInputValue(inputValue);
                if (includeInputValues) {
                    values.push_back(inputValue);
                }
            }
        }

        engine->process();

        if (engine->numberOfOutputVariables() > 0) writer << separator;
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            outputVariable->defuzzify();
            if (outputVariable->isEnabled()) {
                values.push_back(outputVariable->getOutputValue());
            }
        }
        writer << Op::join(values, separator);
    }

    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostream& writer,
            const std::vector<scalar>& inputValues, const std::string& separator, bool includeInputvalues) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ofstream& writer,
            const std::vector<scalar>& inputValues, const std::string& separator, bool includeInputvalues) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostringstream& writer,
            const std::vector<scalar>& inputValues, const std::string& separator, bool includeInputvalues) const;

    FldExporter* FldExporter::clone() const {
        return new FldExporter(*this);
    }

}
