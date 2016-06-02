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

#ifndef FL_BENCHMARK_H
#define FL_BENCHMARK_H

#include "fl/fuzzylite.h"

#include "fl/imex/FldExporter.h"
#include <string>
#include <vector>

namespace fl {

    class Engine;

    class FL_API Benchmark {
    private:
        std::string _name;
        Engine* _engine;
        std::vector<std::vector<scalar> > _expected;
        std::vector<std::vector<scalar> > _obtained;
        std::vector<scalar> _nanoSeconds;
        scalar _errorThreshold;

    public:
        explicit Benchmark(const std::string& name = "", Engine* engine = fl::null);
        virtual ~Benchmark();

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
         Sets the number of nanoseconds taken to produce the set obtained values
         from the set of expected input values
         @param nanoSeconds is the number of nanoseconds taken to produce the set obtained values
         from the set of expected input values
         */
        void setNanoSeconds(const std::vector<scalar> nanoSeconds);
        /**
         Gets the number of nanoseconds taken to produce the set obtained values
         from the set of expected input values
         @return the number of nanoseconds taken to produce the set obtained values
         from the set of expected input values
         */
        const std::vector<scalar>& getNanoSeconds() const;

        /**
         Sets the threshold above which the difference between an expected and 
         obtained value from the engine is considered an error
         @param errorThreshold is the threshold above which the difference between 
         an expected and obtained value from the engine is considered an error
         */
        void setErrorThreshold(scalar errorThreshold);
        /**
         Gets the threshold above which the difference between an expected and 
         obtained value from the engine is considered an error
         @return the threshold above which the difference between an expected 
         and obtained value from the engine is considered an error
         */
        scalar getErrorThreshold() const;

        /**
         Produces and loads into memory the set of expected values from the 
         engine
         @param values is the number of values to evaluate the engine upon
         @param scope is the scope of the values to generate
         */
        virtual void prepare(int values, FldExporter::ScopeOfValues scope);
        /**
         Reads and loads into memory the set of expected values from the 
         engine
         @param reader is the reader of a set of lines containing 
         space-separated  values
         */
        virtual void prepare(std::istream& reader);
        
        /**
         Runs the benchmark on the engine
         @param times is the number of times the benchmark is run
         */
        virtual void run(int times = 1);

        /**
         Resets the benchmark to be ready to run again
         */
        virtual void reset();

        /**
         Computes the mean square error based on the expected and obtained values
         over all the output variables as
         
         @f$\text{MSE} = \frac{1}{|y|\times|e|} \sum_y \sum_i (e_i^y - o^y_i)^2@f$,
         where @f$y@f$ is the set of output variables, @f$e@f$ is the set of 
         expected output values, @f$o@f$ is the set of obtained output values

         @return  the mean square error based on the expected and obtained values
         over all the output variables.
         */
        virtual double meanSquareError() const;

        /**
         Computes the number of errors over all the output variables based on 
         whether the absolute difference between an expected and obtained value 
         is greater than the error threshold 
         
         @f$\text{E} = \sum_y \sum_i 
         \begin{cases}
         0 & \text{if} |e_i^y - o^y_i| < \theta\\
         1 & \text{otherwise}
         \end{cases}
         @f$,
         where @f$y@f$ is the set of output variables, @f$e@f$ is the set of 
         expected output values, @f$o@f$ is the set of obtained output values

         @return the number of errors over all the output variables based on the 
         difference between the expected and obtained values
         */
        virtual int numberOfErrors() const;

        /**
         Returns the header for the resulting dataset
         @param runs is the number of runs the benchmark is run
         @param delimiter is the delimiter of the dataset
         @return the header for the resulting dataset
         */
        virtual std::string header(int runs, const std::string& delimiter = "\t") const;
        /**
         Computes and returns the results from the benchmark
         @param delimiter is the delimiter of the dataset
         @return the results from the benchmark
         */
        virtual std::string results(const std::string& delimiter = "\t");
    };

}

#endif /* FL_BENCHMARK_H */

