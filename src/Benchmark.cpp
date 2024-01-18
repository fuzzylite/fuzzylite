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

#include "fl/Benchmark.h"

#include "fl/Engine.h"
#include "fl/Operation.h"
#include "fl/rule/Rule.h"
#include "fl/rule/RuleBlock.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#ifdef FL_CPP98
//timing is only available in C++11
#else
#include <chrono>
#endif

namespace fl {

    Benchmark::Benchmark(const std::string& name, Engine* engine, scalar tolerance)
    : _name(name), _engine(engine), _tolerance(tolerance) { }

    Benchmark::~Benchmark() { }

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

    void Benchmark::setTolerance(scalar tolerance) {
        this->_tolerance = tolerance;
    }

    scalar Benchmark::getTolerance() const {
        return this->_tolerance;
    }

    void Benchmark::prepare(int values, FldExporter::ScopeOfValues scope) {
        if (not _engine) {
            throw Exception("[benchmark error] engine not set before preparing for values and scope", FL_AT);
        }
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

    void Benchmark::prepare(std::istream& reader, long numberOfLines) {
        _expected = std::vector<std::vector<scalar> >();
        std::string line;
        int lineNumber = 0;
        while (lineNumber != numberOfLines and std::getline(reader, line)) {
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

    scalar Benchmark::runOnce() {
        return run(1).front();
    }

    std::vector<scalar> Benchmark::run(int times) {
        if (not _engine) {
            throw Exception("[benchmark error] engine not set for benchmark", FL_AT);
        }
        std::vector<scalar> runTimes(times, fl::nan);
        const std::size_t offset(_engine->inputVariables().size());
        for (int t = 0; t < times; ++t) {
            _obtained = std::vector<std::vector<scalar> >(_expected.size(),
                    std::vector<scalar>(_engine->variables().size()));
            _engine->restart();

#ifdef FL_CPP98
            //ignore timing
#else
            auto start = std::chrono::high_resolution_clock::now();
#endif

            for (std::size_t evaluation = 0; evaluation < _expected.size(); ++evaluation) {
                const std::vector<scalar>& expectedValues = _expected[evaluation];
                std::vector<scalar>& obtainedValues = _obtained[evaluation];

                if (expectedValues.size() < _engine->inputVariables().size()) {
                    std::ostringstream ex;
                    ex << "[benchmark error] the number of input values given <" <<
                            expectedValues.size() << "> at line <" << (evaluation + 1) << "> "
                            "must be at least the same number of input variables "
                            "<" << _engine->inputVariables().size() << "> in the engine";
                    throw Exception(ex.str());
                }
                for (std::size_t i = 0; i < _engine->inputVariables().size(); ++i) {
                    _engine->getInputVariable(i)->setValue(expectedValues[i]);
                    obtainedValues[i] = expectedValues[i];
                }

                _engine->process();

                for (std::size_t i = 0; i < _engine->outputVariables().size(); ++i) {
                    obtainedValues[i + offset] = _engine->getOutputVariable(i)->getValue();
                }
            }

#ifdef FL_CPP98
            //ignore timing
#else
            auto end = std::chrono::high_resolution_clock::now();
            runTimes.at(t) = std::chrono::duration<scalar, std::nano>(end - start).count();
#endif
        }
        _times.insert(_times.end(), runTimes.begin(), runTimes.end());
        return runTimes;
    }

    void Benchmark::reset() {
        this->_obtained.clear();
        this->_times.clear();
    }

    bool Benchmark::canComputeErrors() const {
        return not (_engine == fl::null or _expected.empty() or _obtained.empty()
                or _expected.size() != _obtained.size()
                or _expected.front().size() != _obtained.front().size()
                or _expected.front().size() != _engine->variables().size());
    }

    scalar Benchmark::meanSquaredError() const {
        return meanSquaredError(fl::null);
    }

    scalar Benchmark::meanSquaredError(const OutputVariable* outputVariable) const {
        if (not canComputeErrors()) {
            return fl::nan;
        }

        scalar mse = 0.0;
        int errors = 0;
        const std::size_t offset = _engine->numberOfInputVariables();
        for (std::size_t i = 0; i < _expected.size(); ++i) {
            const std::vector<scalar>& e = _expected.at(i);
            const std::vector<scalar>& o = _obtained.at(i);

            for (std::size_t y = 0; y < _engine->numberOfOutputVariables(); ++y) {
                if (outputVariable == fl::null
                        or outputVariable == _engine->getOutputVariable(y)) {
                    scalar difference = e.at(offset + y) - o.at(offset + y);
                    if (Op::isFinite(difference)
                            and not Op::isEq(difference, 0.0, _tolerance)) {
                        mse += difference * difference;
                        ++errors;
                    }
                }
            }
        }

        if (errors > 0) {
            mse /= errors;
        }
        return mse;
    }

    int Benchmark::allErrors() const {
        return allErrors(fl::null);
    }

    int Benchmark::allErrors(const OutputVariable* outputVariable) const {
        return numberOfErrors(All, outputVariable);
    }

    int Benchmark::nonFiniteErrors() const {
        return nonFiniteErrors(fl::null);
    }

    int Benchmark::nonFiniteErrors(const OutputVariable* outputVariable) const {
        return numberOfErrors(NonFinite, outputVariable);
    }

    int Benchmark::accuracyErrors() const {
        return accuracyErrors(fl::null);
    }

    int Benchmark::accuracyErrors(const OutputVariable* outputVariable) const {
        return numberOfErrors(Accuracy, outputVariable);
    }

    int Benchmark::numberOfErrors(ErrorType errorType) const {
        return numberOfErrors(errorType, fl::null);
    }

    int Benchmark::numberOfErrors(ErrorType errorType,
            const OutputVariable* outputVariable) const {
        if (not canComputeErrors()) {
            return -1;
        }

        int errors = 0;
        const std::size_t offset = _engine->numberOfInputVariables();
        for (std::size_t i = 0; i < _expected.size(); ++i) {
            const std::vector<scalar>& e = _expected.at(i);
            const std::vector<scalar>& o = _obtained.at(i);

            for (std::size_t y = 0; y < _engine->numberOfOutputVariables(); ++y) {
                if (outputVariable == fl::null
                        or outputVariable == _engine->getOutputVariable(y)) {
                    if (not Op::isEq(e.at(y + offset), o.at(y + offset), _tolerance)) {
                        scalar difference = e.at(y + offset) - o.at(y + offset);
                        if (errorType == Accuracy and Op::isFinite(difference)) {
                            ++errors;
                        } else if (errorType == NonFinite and not Op::isFinite(difference)) {
                            ++errors;
                        } else if (errorType == All) {
                            ++errors;
                        }
                    }
                }
            }
        }

        return errors;
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

    std::vector<std::string> Benchmark::header(int runs, bool includeErrors) {
        Benchmark result;

        Engine dummy;
        dummy.addOutputVariable(new OutputVariable); //canCompute() == true
        result.setEngine(&dummy);

        result.setTimes(std::vector<scalar>(runs, fl::nan));

        if (includeErrors) {
            std::vector<std::vector<scalar> > dummyVector(1,
                    std::vector<scalar>(1, fl::nan));
            result.setExpected(dummyVector);
            result.setObtained(dummyVector);
        }
        std::vector<Benchmark::Result> dummyResults = result.results();

        std::vector<std::string> names;
        for (std::size_t i = 0; i < dummyResults.size(); ++i) {
            names.push_back(dummyResults.at(i).first);
        }
        return names;
    }

    std::vector<Benchmark::Result> Benchmark::results(TimeUnit timeUnit, bool includeTimes) const {
        return results(fl::null, timeUnit, includeTimes);
    }

    std::vector<Benchmark::Result> Benchmark::results(
            const OutputVariable* outputVariable, TimeUnit unit, bool includeTimes) const {
        if (not _engine) {
            throw Exception("[benchmark error] engine not set for benchmark", FL_AT);
        }

        std::vector<scalar> time = _times;

        std::vector<Result> result;
        result.push_back(Result("library", fuzzylite::library()));
        result.push_back(Result("name", _name));
        result.push_back(Result("inputs", Op::str(_engine->numberOfInputVariables())));
        result.push_back(Result("outputs", Op::str(_engine->numberOfOutputVariables())));
        result.push_back(Result("ruleBlocks", Op::str(_engine->numberOfRuleBlocks())));
        std::size_t rules = 0;
        for (std::size_t i = 0; i < _engine->ruleBlocks().size(); ++i) {
            rules += _engine->ruleBlocks().at(i)->rules().size();
        }
        result.push_back(Result("rules", Op::str(rules)));
        result.push_back(Result("runs", Op::str(_times.size())));
        result.push_back(Result("evaluations", Op::str(_expected.size())));
        if (canComputeErrors()) {
            std::vector<std::string> names;
            scalar meanRange = 0.0;
            scalar rmse = std::sqrt(meanSquaredError(outputVariable));
            scalar nrmse = 0.0;
            scalar weights = 0.0;
            for (std::size_t i = 0; i < _engine->outputVariables().size(); ++i) {
                const OutputVariable* y = _engine->outputVariables().at(i);
                if (outputVariable == fl::null or outputVariable == y) {
                    names.push_back(y->getName());
                    meanRange += y->range();
                    nrmse += std::sqrt(meanSquaredError(y)) * 1.0 / y->range();
                    weights += 1.0 / y->range();
                }
            }
            meanRange /= names.size();
            nrmse /= weights;

            result.push_back(Result("outputVariable", Op::join(names, ",")));
            result.push_back(Result("range", Op::str(meanRange)));

            result.push_back(Result("tolerance", Op::str(getTolerance(), -1, std::ios_base::fmtflags(0x0))));
            result.push_back(Result("errors", Op::str(allErrors(outputVariable))));

            result.push_back(Result("nfErrors", Op::str(nonFiniteErrors(outputVariable))));
            result.push_back(Result("accErrors", Op::str(accuracyErrors(outputVariable))));

            result.push_back(Result("rmse", Op::str(rmse, 6, std::ios_base::scientific)));
            result.push_back(Result("nrmse", Op::str(nrmse, 6, std::ios_base::scientific)));
        }
        result.push_back(Result("units", stringOf(unit)));
        result.push_back(Result("sum(t)", Op::str(convert(Op::sum(time), NanoSeconds, unit),
                unit == NanoSeconds ? 0 : fuzzylite::decimals())));
        result.push_back(Result("mean(t)", Op::str(convert(Op::mean(time), NanoSeconds, unit))));
        result.push_back(Result("sd(t)", Op::str(convert(Op::standardDeviation(time), NanoSeconds, unit))));

        if (includeTimes) {
            for (std::size_t i = 0; i < time.size(); ++i) {
                result.push_back(Result("t" + Op::str(i + 1),
                        Op::str(time.at(i), unit == NanoSeconds ? 0 : fuzzylite::decimals())));
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
