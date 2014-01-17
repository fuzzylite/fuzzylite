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

    std::string FldExporter::toString(const Engine* mutableEngine) const {
        return toString(mutableEngine, 1024);
    }

    std::string FldExporter::toString(const Engine* mutableEngine, int maximumNumberOfResults) const {
        Engine* engine = const_cast<Engine*> (mutableEngine);
        std::ostringstream result;
        toWriter(engine, result, _separator, maximumNumberOfResults);
        return result.str();
    }

    template <typename T>
    void FldExporter::toWriter(Engine* engine, T& writer,
            const std::string& separator, int maximum) const {
        std::vector<std::string> variables;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            variables.push_back(engine->getInputVariable(i)->getName());
        }
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            variables.push_back(engine->getOutputVariable(i)->getName());
        }
        writer << Op::join(variables, separator) << "\n";

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
                scalar range = inputVariable->range();
                scalar inputValue = inputVariable->getMinimum()
                        + sampleValues.at(i) * range / resolution;
                inputVariable->setInputValue(inputValue);
                values.push_back(Op::str(inputValue));
            }

            engine->process();

            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* outputVariable = engine->getOutputVariable(i);
                values.push_back(Op::str(outputVariable->defuzzify()));
            }

            writer << Op::join(values, separator) << "\n";
            writer.flush();

            overflow = Op::increment(sampleValues, minSampleValues, maxSampleValues);
        }
    }

    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostream& writer,
            const std::string& separator, int resolution) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ofstream& writer,
            const std::string& separator, int resolution) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostringstream& writer,
            const std::string& separator, int resolution) const;

    std::string FldExporter::toString(const Engine* mutableEngine, const std::string& inputData) const {
        if (inputData.empty()) {
            throw fl::Exception("[export error] input data is empty", FL_AT);
        }
        Engine* engine = const_cast<Engine*> (mutableEngine);
        std::ostringstream writer;

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            writer << engine->getOutputVariable(i)->getName();
            if (i + 1 < engine->numberOfOutputVariables()) {
                writer << _separator;
            }
        }
        writer << "\n";

        std::istringstream reader(inputData);
        std::string line;
        int lineNumber = 0;
        while (std::getline(reader, line)) {
            ++lineNumber;
            std::vector<scalar> inputValues;
            parse(line, inputValues);
            if (inputValues.empty()) continue;
            if ((int) inputValues.size() != engine->numberOfInputVariables()) {
                std::ostringstream ex;
                ex << "[export error] engine has <" << engine->numberOfInputVariables() << "> "
                        "input variables, but input data provides <" << inputValues.size() << "> values "
                        "at line <" << lineNumber << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }
            toWriter(engine, writer, _separator, inputValues);
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
    void FldExporter::toWriter(Engine* engine, T& writer, const std::string& separator,
            const std::vector<scalar>& inputValues) const {
        for (std::size_t i = 0; i < inputValues.size(); ++i) {
            engine->getInputVariable(i)->setInputValue(inputValues.at(i));
        }
        engine->process();
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            writer << fl::Op::str(engine->getOutputVariable(i)->defuzzify());
            if (i + 1 < engine->numberOfOutputVariables()) {
                writer << separator;
            }
        }
    }

    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostream& writer,
            const std::string& separator, const std::vector<scalar>& inputValues) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ofstream& writer,
            const std::string& separator, const std::vector<scalar>& inputValues) const;
    template FL_EXPORT void FldExporter::toWriter(Engine* engine, std::ostringstream& writer,
            const std::string& separator, const std::vector<scalar>& inputValues) const;


}
