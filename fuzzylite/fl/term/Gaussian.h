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
 * Gaussian.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_GAUSSIAN_H
#define FL_GAUSSIAN_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Gaussian : public Term {
    protected:
        scalar _mean;
        scalar _standardDeviation;

    public:
        Gaussian(const std::string& name = "",
                scalar mean = fl::nan,
                scalar standardDeviation = fl::nan);
        virtual ~Gaussian();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);
        
        virtual scalar membership(scalar x) const;

        virtual void setMean(scalar c);
        virtual scalar getMean() const;

        virtual void setStandardDeviation(scalar sigma);
        virtual scalar getStandardDeviation() const;
        
        virtual Gaussian* clone() const;
        
        static Term* constructor();
    };

}
#endif /* FL_GAUSSIAN_H */
