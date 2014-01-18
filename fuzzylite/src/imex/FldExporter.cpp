/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
        std::string inputHeader = header(engine->inputVariables());
        std::string outputHeader = header(engine->outputVariables());
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

    std::string FldExporter::toString(const Engine* mutableEngine, int maximumNumberOfResults) const {
        Engine* engine = const_cast<Engine*> (mutableEngine);
        std::ostringstream result;
        result << "#" << header(engine) << "\n";
        toWriter(engine, result, maximumNumberOfResults, _separator);
        return result.str();
    }

    template <typename T>
    void FldExporter::toWriter(Engine* engine, T& writer, int maximum,
            const std::string& separator) const {
        int resolution = -1 + (int) std::max(1.0, std::pow(
                maximum, 1.0 / engine->numberOfInputVariables()));
        std::vector<int> sampleValues, minSampleValues, maxSampleValues;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            sampleValues.push_back(0);
            minSampleValues.push_back(0);
            maxSampleValues.push_back(resolution);
        }

        bool overflow = false;
        while (not overflow) {
            std::vector<std::string> values;

            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                InputVariable* inputVariable = engine->getInputVariable(i);
                if (not inputVariable->isEnabled()) continue;

                scalar inputValue = inputVariable->getMinimum()
                        + sampleValues.at(i) * inputVariable->range() / resolution;
                inputVariable->setInputValue(inputValue);

                values.push_back(Op::str(inputValue));
            }

            engine->process();

            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* outputVariable = engine->getOutputVariable(i);
                if (not outputVariable->isEnabled()) continue;

                values.push_back(Op::str(outputVariable->defuzzify()));
            }

            writer << Op::join(values, separator) << "\n";
            writer.flush();

            overflow = Op::increment(sampleValues, minSampleValues, maxSampleValues);
        }
    }

    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostream& writer,
            int maximumNumberOfResults, const std::string& separator) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ofstream& writer,
            int maximumNumberOfResults, const std::string& separator) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostringstream& writer,
            int maximumNumberOfResults, const std::string& separator) const;

    std::string FldExporter::toString(const Engine* mutableEngine, const std::string& inputData) const {
        Engine* engine = const_cast<Engine*> (mutableEngine);
        std::ostringstream writer;
        writer << "#" << header(engine) << "\n";

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
            toWriter(engine, writer, inputValues, _separator);
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
            const std::string& separator) const {
        std::vector<scalar> values;
        for (std::size_t i = 0; i < inputValues.size(); ++i) {
            InputVariable* inputVariable = engine->getInputVariable(i);
            if (not inputVariable->isEnabled()) continue;

            scalar inputValue = inputValues.at(i);
            inputVariable->setInputValue(inputValue);
            values.push_back(inputValue);
        }

        engine->process();

        if (engine->numberOfOutputVariables() > 0) writer << separator;
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            if (not outputVariable->isEnabled()) continue;

            values.push_back(outputVariable->defuzzify());
        }
        writer << Op::join(values, separator);
    }

    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostream& writer,
            const std::vector<scalar>& inputValues, const std::string& separator) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ofstream& writer,
            const std::vector<scalar>& inputValues, const std::string& separator) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostringstream& writer,
            const std::vector<scalar>& inputValues, const std::string& separator) const;


}
