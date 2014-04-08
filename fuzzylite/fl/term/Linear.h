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
 * File:   Linear.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 3:42 PM
 */

#ifndef FL_LINEAR_H
#define	FL_LINEAR_H

#include "fl/term/Term.h"
#include "fl/variable/InputVariable.h"

namespace fl {
    class Engine;

    class FL_EXPORT Linear : public Term {
    protected:
        const Engine* _engine;
    public:
        std::vector<scalar> coefficients;
        Linear(const std::string& name = "",
                const std::vector<scalar>& coefficients = std::vector<scalar>(),
                const Engine* engine = NULL);
        virtual ~Linear();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void set(const std::vector<scalar>& coeffs, const Engine* engine) throw (fl::Exception);

        virtual void setEngine(const Engine* engine);
        virtual const Engine* getEngine() const;

        virtual Linear* clone() const;

        static Term* constructor();

        //Warning: this method is unsafe, make sure you use it correctly.
        template <typename T>
        static Linear* create(const std::string& name, const Engine* engine,
                T firstCoefficient, ...) throw (fl::Exception);
    };

}

#endif	/* FL_LINEAR_H */

