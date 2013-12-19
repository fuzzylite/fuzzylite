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

#include "fl/imex/DataExporter.h"

#include "fl/Engine.h"
#include "fl/variable/Variable.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include <fstream>
#include <vector>

namespace fl {

    DataExporter::DataExporter(const std::string& separator, int resolution)
    : _separator(separator), _resolution(resolution) {

    }

    DataExporter::~DataExporter() {
    }

    std::string DataExporter::name() const {
        return "DataExporter";
    }

    void DataExporter::setSeparator(const std::string& separator) {
        this->_separator = separator;
    }

    std::string DataExporter::getSeparator() const {
        return this->_separator;
    }

    void DataExporter::setResolution(int resolution) {
        this->_resolution = resolution;
    }

    int DataExporter::getResolution() const {
        return this->_resolution;
    }

    std::string DataExporter::toString(const Engine* mutableEngine) const {
        Engine* engine = const_cast<Engine*> (mutableEngine);
        std::ostringstream result;
        toWriter(engine, result, _separator, _resolution);
        return result.str();
    }

    template <typename T>
    void DataExporter::toWriter(Engine* engine, T& writer,
            const std::string& separator, int resolution) const {
        std::vector<std::string> variables;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            variables.push_back(engine->getInputVariable(i)->getName());
        }
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            variables.push_back(engine->getOutputVariable(i)->getName());
        }
        writer << Op::join(variables, separator) << "\n";

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
                scalar range = inputVariable->getMaximum() - inputVariable->getMinimum();
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

    template FL_EXPORT void DataExporter::toWriter(Engine* engine, std::ofstream& writer,
            const std::string& separator, int resolution) const;
    template FL_EXPORT void DataExporter::toWriter(Engine* engine, std::ostringstream& writer,
            const std::string& separator, int resolution) const;
}
