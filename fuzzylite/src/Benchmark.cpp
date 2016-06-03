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

    Benchmark::Benchmark(const std::string& name, Engine* engine, scalar errorThreshold)
    : _name(name), _engine(engine), _errorThreshold(errorThreshold) {

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

    void Benchmark::setTimes(const std::vector<scalar> nanoSeconds) {
        this->_times = nanoSeconds;
    }

    const std::vector<scalar>& Benchmark::getTimes() const {
        return this->_times;
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

    std::vector<scalar> Benchmark::run(int times) {
        std::vector<scalar> runTimes(times, fl::nan);
        const std::size_t offset(_engine->inputVariables().size());
        for (int t = 0; t < times; ++t) {
            _obtained = std::vector<std::vector<scalar> >(_expected.size());
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

                _obtained.at(evaluation) = obtainedValues;
            }

#ifdef FL_CPP11
            auto end = std::chrono::high_resolution_clock::now();
            runTimes.at(t) = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
#endif
        }
        _times.insert(_times.end(), runTimes.begin(), runTimes.end());
        return runTimes;
    }

    void Benchmark::reset() {
        this->_obtained.clear();
        this->_times.clear();
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

    std::string Benchmark::stringOf(TimeUnit unit) {
        if (unit == NanoSeconds) return "nanoseconds";
        if (unit == MicroSeconds) return "microseconds";
        if (unit == MilliSeconds) return "milliseconds";
        if (unit == Seconds) return "seconds";
        if (unit == Minutes) return "minutes";
        if (unit == Hours) return "hours";
        return "undefined";
    }

    scalar Benchmark::factorOf(TimeUnit unit) {
        if (unit == NanoSeconds) return 1.0;
        else if (unit == MicroSeconds) return 1.0e-3;
        else if (unit == MilliSeconds) return 1.0e-6;
        else if (unit == Seconds) return 1.0e-9;
        else if (unit == Minutes) return 1.0e-9 / 60;
        else if (unit == Hours) return 1.0e-9 / 3600;
        return fl::nan;
    }

    scalar Benchmark::convert(scalar x, TimeUnit from, TimeUnit to) {
        return x * factorOf(to) / factorOf(from);
    }

    std::vector<Benchmark::Result> Benchmark::results(TimeUnit unit, bool includeTimes) const {
        std::vector<scalar> time = _times;

        std::vector<Result> result;
        result.push_back(Result("library", fuzzylite::library()));
        result.push_back(Result("name", _name));
        result.push_back(Result("inputs", Op::str(_engine->numberOfInputVariables())));
        result.push_back(Result("outputs", Op::str(_engine->numberOfOutputVariables())));
        result.push_back(Result("runs", Op::str(_times.size())));
        result.push_back(Result("evaluations", Op::str(_expected.size())));
        result.push_back(Result("errors", Op::str(numberOfErrors())));
        result.push_back(Result("mse", Op::str(meanSquareError())));
        result.push_back(Result("units", stringOf(unit)));
        if (unit == NanoSeconds) {
            result.push_back(Result("sum(t)", Op::str((long)convert(Op::sum(time), NanoSeconds, unit))));
        }else{
            result.push_back(Result("sum(t)", Op::str(convert(Op::sum(time), NanoSeconds, unit))));
        }
        result.push_back(Result("mean(t)", Op::str(convert(Op::mean(time), NanoSeconds, unit))));
        result.push_back(Result("sd(t)", Op::str(convert(Op::standardDeviation(time), NanoSeconds, unit))));

        if (includeTimes) {
            for (std::size_t i = 0; i < time.size(); ++i) {
                if (unit == NanoSeconds) {
                    result.push_back(Result("t" + Op::str(i + 1), Op::str((long) time.at(i))));
                } else {
                    result.push_back(Result("t" + Op::str(i + 1),
                            Op::str(convert(time.at(i), NanoSeconds, unit))));
                }
            }
        }
        return result;
    }

    std::string Benchmark::format(std::vector<Result> results, TableShape shape,
            TableContents contents, const std::string& delimiter) const {
        std::ostringstream os;

        if (shape == Vertical) {
            for (std::size_t i = 0; i < results.size(); ++i) {
                Result pair = results.at(i);
                if (contents bitand Header) {
                    os << pair.first;
                }
                if (contents == HeaderAndBody) {
                    os << delimiter;
                }
                if (contents bitand Body) {
                    os << pair.second;
                }
                if (i + 1 < results.size()) os << "\n";
            }

        } else if (shape == Horizontal) {
            std::ostringstream header;
            std::ostringstream body;
            for (std::size_t i = 0; i < results.size(); ++i) {
                Result pair = results.at(i);
                if (contents bitand Header) {
                    header << pair.first;
                    if (i + 1 < results.size()) header << delimiter;
                }
                if (contents bitand Body) {
                    body << pair.second;
                    if (i + 1 < results.size()) body << delimiter;
                }
            }
            if (contents bitand Header) os << header.str();
            if (contents == HeaderAndBody) os << "\n";
            if (contents bitand Body) os << body.str();
        }
        return os.str();
    }
}
