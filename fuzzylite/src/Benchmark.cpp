/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.
 */

#include "fl/Benchmark.h"

#include "fl/Engine.h"
#include "fl/Operation.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include <vector>

#ifdef FL_CPP11
#include <chrono>
#endif

namespace fl {

    Benchmark::Benchmark(const std::string& name, Engine* engine)
    : _name(name), _engine(engine) {

    }

    Benchmark::~Benchmark() {

    }

    void Benchmark::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Benchmark::getName() const {
        return this->_name;
    }

    void Benchmark::setEngine(Engine* engine) {
        this->_engine = engine;
    }

    Engine* Benchmark::getEngine() const {
        return this->_engine;
    }

    void Benchmark::setExpected(const std::vector<std::vector<scalar> >& expected) {
        this->_expected = expected;
    }

    const std::vector<std::vector<scalar> >& Benchmark::getExpected() const {
        return this->_expected;
    }

    void Benchmark::setObtained(const std::vector<std::vector<scalar> >& obtained) {
        this->_obtained = obtained;
    }

    const std::vector<std::vector<scalar> >& Benchmark::getObtained() const {
        return this->_obtained;
    }

    void Benchmark::setNanoSeconds(const std::vector<scalar> nanoSeconds) {
        this->_nanoSeconds = nanoSeconds;
    }

    const std::vector<scalar>& Benchmark::getNanoSeconds() const {
        return this->_nanoSeconds;
    }

    void Benchmark::setErrorThreshold(scalar errorThreshold) {
        this->_errorThreshold = errorThreshold;
    }

    scalar Benchmark::getErrorThreshold() const {
        return this->_errorThreshold;
    }

    void Benchmark::prepare(int values, FldExporter::ScopeOfValues scope) {
        int resolution;
        if (scope == FldExporter::AllVariables)
            resolution = -1 + (int) std::max(1.0, std::pow(
                values, 1.0 / _engine->numberOfInputVariables()));
        else //if (scope == EachVariable) 
            resolution = values - 1;

        std::vector<int> sampleValues, minSampleValues, maxSampleValues;
        for (std::size_t i = 0; i < _engine->numberOfInputVariables(); ++i) {
            sampleValues.push_back(0);
            minSampleValues.push_back(0);
            maxSampleValues.push_back(resolution);
        }

        _expected = std::vector<std::vector<scalar> >();
        do {
            std::vector<scalar> expectedValues(_engine->numberOfInputVariables());
            for (std::size_t i = 0; i < _engine->numberOfInputVariables(); ++i) {
                InputVariable* inputVariable = _engine->getInputVariable(i);
                expectedValues.at(i) = inputVariable->getMinimum()
                        + sampleValues.at(i) * inputVariable->range() / std::max(1, resolution);
            }
            _expected.push_back(expectedValues);
        } while (Op::increment(sampleValues, minSampleValues, maxSampleValues));
    }

    void Benchmark::prepare(std::istream& reader) {
        _expected = std::vector<std::vector<scalar> >();
        std::string line;
        int lineNumber = 0;
        while (std::getline(reader, line)) {
            ++lineNumber;
            line = Op::trim(line);
            if (line.empty() or line.at(0) == '#')
                continue; //comments are ignored, blank lines are retained
            std::vector<scalar> expectedValues;
            if (lineNumber == 1) { //automatic detection of header.
                try {
                    expectedValues = Op::toScalars(line);
                } catch (std::exception&) {
                    continue;
                }
            } else {
                expectedValues = Op::toScalars(line);
            }
            _expected.push_back(expectedValues);
        }
    }

    void Benchmark::run(int times) {
        _nanoSeconds = std::vector<scalar>(times, fl::nan);

        const std::size_t offset(_engine->inputVariables().size());
        for (int t = 0; t < times; ++t) {
            _obtained = std::vector<std::vector<scalar> >();
            _engine->restart();

#ifdef FL_CPP11
            auto start = std::chrono::high_resolution_clock::now();
#endif

            for (std::size_t evaluation = 0; evaluation < _expected.size(); ++evaluation) {
                const std::vector<scalar>& expectedValues = _expected.at(evaluation);
                std::vector<scalar> obtainedValues(
                        _engine->numberOfInputVariables() + _engine->numberOfOutputVariables());

                if (expectedValues.size() < _engine->inputVariables().size()) {
                    std::ostringstream ex;
                    ex << "[benchmark error] the number of input values given <" <<
                            expectedValues.size() << "> at line <" << (evaluation + 1) << "> "
                            "must be at least the same number of input variables "
                            "<" << _engine->inputVariables().size() << "> in the engine";
                    throw Exception(ex.str());
                }
                for (std::size_t i = 0; i < _engine->inputVariables().size(); ++i) {
                    _engine->inputVariables().at(i)->setValue(expectedValues.at(i));
                    obtainedValues.at(i) = expectedValues.at(i);
                }

                _engine->process();

                for (std::size_t i = 0; i < _engine->outputVariables().size(); ++i) {
                    obtainedValues.at(i + offset) = _engine->outputVariables().at(i)->getValue();
                }

                _obtained.push_back(obtainedValues);
            }

#ifdef FL_CPP11
            auto end = std::chrono::high_resolution_clock::now();
            _nanoSeconds.at(t) = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
#endif
        }
    }

    void Benchmark::reset() {
        this->_obtained.clear();
        this->_nanoSeconds.clear();
    }

    double Benchmark::meanSquareError() const {
        if (_expected.empty() or _expected.front().size() != _engine->variables().size()) {
            return fl::nan;
        }
        std::vector<scalar> sumOfSquares(_engine->numberOfOutputVariables(), 0);

        const int offset = _engine->numberOfInputVariables();
        for (std::size_t i = 0; i < _expected.size(); ++i) {
            const std::vector<scalar>& e = _expected.at(i);
            const std::vector<scalar>& o = _obtained.at(i);

            for (std::size_t y = 0; y < _engine->numberOfOutputVariables(); ++y) {
                scalar squareDifference = (e.at(offset + y) - o.at(offset + y))
                        * (e.at(offset + y) - o.at(offset + y));
                if (Op::isFinite(squareDifference)) {
                    sumOfSquares.at(y) += squareDifference;
                }
            }
        }

        scalar mse = 0.0;
        for (std::size_t i = 0; i < sumOfSquares.size(); ++i) {
            mse += sumOfSquares.at(i);
        }
        mse /= (_engine->numberOfOutputVariables() * _expected.size());
        return mse;
    }

    int Benchmark::numberOfErrors() const {
        if (_expected.empty() or _expected.front().size() != _engine->variables().size()) {
            return -1;
        }
        std::vector<int> errors(_engine->numberOfOutputVariables(), 0);

        const int offset = _engine->numberOfInputVariables();
        for (std::size_t i = 0; i < _expected.size(); ++i) {
            const std::vector<scalar>& e = _expected.at(i);
            const std::vector<scalar>& o = _obtained.at(i);

            for (std::size_t y = 0; y < _engine->numberOfOutputVariables(); ++y) {
                if (not Op::isEq(e.at(y + offset), o.at(y + offset), _errorThreshold)) {
                    errors.at(y)++;
                }
            }
        }

        int result = 0;
        for (std::size_t i = 0; i < errors.size(); ++i) {
            result += errors.at(i);
        }
        return result;
    }

    std::string Benchmark::header(int runs, const std::string& delimiter) const {
        std::vector<std::string> result;
        result.push_back("library");
        result.push_back("name");
        result.push_back("inputs");
        result.push_back("outputs");
        result.push_back("evaluations");
        result.push_back("errors");
        result.push_back("mse");
        result.push_back("mean_ns");
        result.push_back("sd_ns");
        for (int i = 0; i < runs; ++i) {
            result.push_back("t" + Op::str(i + 1));
        }
        return Op::join(result, delimiter);
    }

    std::string Benchmark::results(const std::string& delimiter) {
        std::vector<std::string> result;
        result.push_back(fuzzylite::library());
        result.push_back(_name);
        result.push_back(Op::str(_engine->numberOfInputVariables()));
        result.push_back(Op::str(_engine->numberOfOutputVariables()));
        result.push_back(Op::str(_expected.size()));

        result.push_back(Op::str(meanSquareError()));

        result.push_back(Op::str(Op::mean(_nanoSeconds)));
        result.push_back(Op::str(Op::standardDeviation(_nanoSeconds)));

        for (std::size_t i = 0; i < _nanoSeconds.size(); ++i) {
            result.push_back(Op::str(_nanoSeconds.at(i)));
        }
        return Op::join(result, delimiter);
    }
}
