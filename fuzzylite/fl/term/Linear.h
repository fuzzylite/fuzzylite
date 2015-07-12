/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_LINEAR_H
#define FL_LINEAR_H

#include "fl/term/Term.h"

namespace fl {
    class Engine;

    class FL_API Linear : public Term {
    private:
        std::vector<scalar> _coefficients;
        const Engine* _engine;
    public:
        explicit Linear(const std::string& name = "",
                const std::vector<scalar>& coefficients = std::vector<scalar>(),
                const Engine* engine = fl::null);
        virtual ~Linear() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Linear)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;

        virtual void set(const std::vector<scalar>& coeffs, const Engine* engine);

        virtual void setCoefficients(const std::vector<scalar>& coeffs);
        virtual const std::vector<scalar>& coefficients() const;
        virtual std::vector<scalar>& coefficients();

        virtual void setEngine(const Engine* engine);
        virtual const Engine* getEngine() const;

        virtual Linear* clone() const FL_IOVERRIDE;

        static Term* constructor();

        //Warning: this method is unsafe, make sure you use it correctly.
        template <typename T>
        static Linear* create(const std::string& name, const Engine* engine,
                T firstCoefficient, ...);
    };
}

/**
 * Template implementation
 */

#include "fl/Engine.h"

namespace fl {

    template <typename T>
    inline Linear* Linear::create(const std::string& name,
            const Engine* engine, T firstCoefficient, ...) {
        if (not engine) throw fl::Exception("[linear error] cannot create term <" + name + "> "
                "without a reference to the engine", FL_AT);
        std::vector<scalar> coefficients;
        coefficients.push_back((scalar) firstCoefficient);

        va_list args;
        va_start(args, firstCoefficient);
        for (std::size_t i = 0; i < engine->inputVariables().size(); ++i) {
            coefficients.push_back((scalar) va_arg(args, T));
        }
        va_end(args);

        return new Linear(name, coefficients, engine);
    }
}
#endif  /* FL_LINEAR_H */

