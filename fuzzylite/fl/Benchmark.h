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

#ifndef FL_BENCHMARK_H
#define FL_BENCHMARK_H

#include "fl/fuzzylite.h"

#include "fl/Complexity.h"
#include "fl/imex/FldExporter.h"

#include <string>
#include <vector>

namespace fl {

    class Engine;

    /**
      The Benchmark class is designed to evaluate the performance of an Engine.

      @author Juan Rada-Vilela, Ph.D.
      @see Engine
      @since 6.0
     */
    class FL_API Benchmark {
    private:
        std::string _name;
        Engine* _engine;
        std::vector<std::vector<scalar> > _expected;
        std::vector<std::vector<scalar> > _obtained;
        std::vector<scalar> _times;
        scalar _tolerance;

    public:

        /**
         Unit of time to utilize in the results
         */
        enum TimeUnit {
            NanoSeconds, MicroSeconds, MilliSeconds, Seconds, Minutes, Hours
        };

        /**
         Shape of the table of results
         */
        enum TableShape {
            Horizontal, Vertical
        };

        /**
         Contents of the table of results
         */
        enum TableContents {
            Header = 1, Body = 2, HeaderAndBody = (Header | Body)
        };

        /**
         Type of error between expected and obtained values
         */
        enum ErrorType {
            NonFinite, Accuracy, All
        };

        explicit Benchmark(const std::string& name = "", Engine* engine = fl::null,
                scalar tolerance = fuzzylite::macheps());
        virtual ~Benchmark();
        FL_DEFAULT_COPY_AND_MOVE(Benchmark)

        /**
         Sets the name of the benchmark
         @param name is the name of the benchmark
         */
        void setName(const std::string& name);
        /**
         Gets the name of the benchmark
         @return name is the name of the benchmark
         */
        std::string getName() const;

        /**
         Sets the engine to benchmark
         @param engine is the engine to benchmark
         */
        void setEngine(Engine* engine);
        /**
         Gets the engine to benchmark
         @return the engine to benchmark
         */
        Engine* getEngine() const;

        /**
         Sets the set of expected values from the engine, where the inner vector
         contains the input values and output values
         @param expected is the set of expected values from the engine
         */
        void setExpected(const std::vector<std::vector<scalar> >& expected);
        /**
         Gets the set of expected values from the engine, where the inner vector
         contains the input values and output values
         @return the set of expected values from the engine
         */
        const std::vector<std::vector<scalar> >& getExpected() const;

        /**
         Sets the set of obtained values from the engine, where the inner vector
         contains the input values and output values
         @param obtained is the set of obtained values from the engine
         */
        void setObtained(const std::vector<std::vector<scalar> >& obtained);
        /**
         Gets the set of obtained values from the engine, where the inner vector
         contains the input values and output values
         @return the set of obtained values from the engine
         */
        const std::vector<std::vector<scalar> >& getObtained() const;

        /**
         Sets the vector of nanoseconds taken to produce the set of obtained values
         from the set of expected input values
         @param times is the vector of nanoseconds taken to produce the set of obtained values
         from the set of expected input values
         */
        void setTimes(const std::vector<scalar> times);
        /**
         Gets the vector of nanoseconds taken to produce the set of obtained values
         from the set of expected input values
         @return the vector of nanoseconds taken to produce the set of obtained values
         from the set of expected input values
         */
        const std::vector<scalar>& getTimes() const;

        /**
         Sets the tolerance above which the difference between an expected and
         obtained value from the engine is considered an error
         @param tolerance is the tolerance above which the difference between
         an expected and obtained value from the engine is considered an error
         */
        void setTolerance(scalar tolerance);
        /**
         Gets the tolerance above which the difference between an expected and
         obtained value from the engine is considered an error
         @return the tolerance above which the difference between an expected
         and obtained value from the engine is considered an error
         */
        scalar getTolerance() const;

        /**
         Produces and loads into memory the set of expected values from the
         engine
         @param values is the number of values to evaluate the engine upon
         @param scope is the scope of the values to generate
         @throws Exception if the engine is not set
         */
        virtual void prepare(int values, FldExporter::ScopeOfValues scope);
        /**
         Reads and loads into memory the set of expected values from the
         engine
         @param reader is the reader of a set of lines containing
         space-separated values
         @param numberOfLines is the maximum number of lines to read from the
         reader, and a value $f@n=(\infty, -1]$f@ reads the entire file.
         */
        virtual void prepare(std::istream& reader, long numberOfLines = -1);

        /**
         Runs the benchmark on the engine only once
         @return the time in nanoseconds required by the run, which is
         also appended to the times stored in Benchmark::getTimes()
         */
        virtual scalar runOnce();
        /**
         Runs the benchmark on the engine multiple times
         @param times is the number of times to run the benchmark on the engine
         @return vector of the time in nanoseconds required by each run, which is
         also appended to the times stored in Benchmark::getTimes()
         */
        virtual std::vector<scalar> run(int times);

        /**
         Resets the benchmark to be ready to run again
         */
        virtual void reset();

        /**
         Indicates whether errors can be computed based on the expected and
         obtained values from the benchmark. If the benchmark was prepared from
         a file reader and the file included columns of expected output values
         and the benchmark has been run at least once, then the benchmark can
         automatically compute the errors and will automatically include them in
         the results.
         @return whether errors can be computed based on the expected and
         obtained values from the benchmark
         */
        virtual bool canComputeErrors() const;

        /**
         Computes the mean squared error over all output variables considering
         only those cases where there is an accuracy error as defined in
         Benchmark::accuracyErrors().
         @return  the mean squared error over all the output variables.
         */
        virtual scalar meanSquaredError() const;

        /**
         Computes the mean squared error of the given output variable
         considering only those cases where there is an accuracy error
         as defined in Benchmark::accuracyErrors().
         @param outputVariable is the output variable to compute the errors for
         @return the mean squared error over the given output variable.
         */
        virtual scalar meanSquaredError(const OutputVariable* outputVariable) const;

        /**
         Computes the number of errors over all the output variables caused by
         non-finite differences or accuracy differences. An error is counted when
         the difference between the expected and obtained values is not finite,
         or the absolute difference between the expected and obtained values
         is not smaller than the tolerance.
         @return the number of errors over all the output variables caused by
         non-finite differences or accuracy differences
         */
        virtual int allErrors() const;

        /**
         Computes the number of errors of the given output variable caused by
         non-finite differences or accuracy differences. An error is counted when
         the difference between the expected and obtained values is not finite,
         or the absolute difference between the expected and obtained values
         is not smaller than the tolerance.
         @param outputVariable is the output variable to account the errors for
         @return the number of errors of the given output variable caused by
         non-finite differences or accuracy differences
         */
        virtual int allErrors(const OutputVariable* outputVariable) const;

        /**
         Computes the number of errors over all the output variables caused by
         non-finite differences (ie, infinity and NaN). An error is counted when
         the difference between the expected and obtained values is not finite.
         @return the number of errors over all the output variables caused by
         non-finite differences
         */
        virtual int nonFiniteErrors() const;

        /**
         Computes the number of errors of the given output variable caused by
         non-finite differences (ie, infinity and NaN). An error is counted when
         the difference between the expected and obtained values is not finite.
         @param outputVariable is the output variable to account the errors for
         @return the number of errors of the given output variable caused by
         non-finite differences
         */
        virtual int nonFiniteErrors(const OutputVariable* outputVariable) const;

        /**
         Computes the number of errors over all the output variables caused by
         a significant difference in accuracy. An error is counted when the
         absolute difference between the expected and  obtained values
         is not smaller than the tolerance.

         @f$\text{E} = \sum_y \sum_i \epsilon_i^y, \text{where } \epsilon_i^y =
         \begin{cases}
         0 & \text{if} |e_i^y - o^y_i| < \theta\\
         1 & \text{otherwise}
         \end{cases}
         @f$,
         @f$y@f$ is the set of output variables, @f$e@f$ is the set of
         expected output values, @f$o@f$ is the set of obtained output values,
         and @f$\theta@f$ is the tolerance

         @return the number of errors over all the output variables caused by
         a significant difference in accuracy
         */
        virtual int accuracyErrors() const;


        /**
         Computes the number of errors over the given output variable caused by
         a significant difference in accuracy. An error is counted when the
         absolute difference between the expected and  obtained values
         is not smaller than the tolerance.

         @f$\text{E} = \sum_i \epsilon_i, \text{where } \epsilon_i =
         \begin{cases}
         0 & \text{if} |e_i - o_i| < \theta\\
         1 & \text{otherwise}
         \end{cases}
         @f$,
         @f$e@f$ is the set of expected output values,
         @f$o@f$ is the set of obtained output values,
         and @f$\theta@f$ is the tolerance

         @param outputVariable is the output variable to account the errors for
         @return the number of errors of the given output variable caused by
         a significant difference in accuracy
         */
        virtual int accuracyErrors(const OutputVariable* outputVariable) const;

        /**
         Computes the number of errors of the given type over all the output
         variables.
         @param errorType is the type of error to account for
         @return the number of errors over all the output variables
         */
        virtual int numberOfErrors(ErrorType errorType) const;

        /**
         Computes the number of errors of the given type over the given output
         variable.
         @param errorType is the type of error to account for
         @param outputVariable is the output variable to account the errors for
         @return the number of errors over the given output variable
         */
        virtual int numberOfErrors(ErrorType errorType,
                const OutputVariable* outputVariable) const;

        /**
         Returns the name of the time unit
         @param unit is the time unit
         @return the name of the time unit
         */
        static std::string stringOf(TimeUnit unit);

        /**
         Returns the factor of the given unit from NanoSeconds
         @param unit is the time unit
         @return the factor of the given unit from NanoSeconds
         */
        static scalar factorOf(TimeUnit unit);
        /**
         Converts the time to different scales
         @param time is the time to convert
         @param from is the units of the time to convert from
         @param to is the units of the time to convert to
         @return the time in the units specified
         */
        static scalar convert(scalar time, TimeUnit from, TimeUnit to);

        /**
         Returns the header of a horizontal table of results
         @param runs is the number of times the benchmark will be run, hence
         producing the relevant number of columns for each run
         @param includeErrors indicates whether to include columns for computing
         the errors
         @return the header of a horizontal table of results
         */
        virtual std::vector<std::string> header(int runs, bool includeErrors = true);

        /**Result is a type definition for a pair of strings*/
        typedef std::pair<std::string, std::string> Result;
        /**
         Computes and returns the results from the benchmark aggregating the
         statistics of all the output variables
         @param timeUnit is the unit of time of the results
         @param includeTimes indicates whether to include the times of each run
         @return the results from the benchmark
         */
        virtual std::vector<Result> results(TimeUnit timeUnit = NanoSeconds, bool includeTimes = true) const;

        /**
         Computes and returns the results from the benchmark for the given output
         variable
         @param outputVariable is the output variable to compute the statistics for
         @param timeUnit is the unit of time of the results
         @param includeTimes indicates whether to include the times of each run
         @return the results from the benchmark
         */
        virtual std::vector<Result> results(const OutputVariable* outputVariable,
                TimeUnit timeUnit = NanoSeconds, bool includeTimes = true) const;

        /**
         Formats the results
         @param results is the vector of results
         @param shape is the shape to present the table of results
         @param contents indicates the information to include in the table of results
         @param delimiter is the delimiter of the table of results
         @return the formatted results from the benchmark
         */
        virtual std::string format(std::vector<Result> results, TableShape shape,
                TableContents contents, const std::string& delimiter = "\t") const;
    };

}

#endif /* FL_BENCHMARK_H */

