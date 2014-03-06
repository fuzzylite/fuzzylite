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
 * File:   Factory.h
 * Author: jcrada
 *
 * Created on 29 November 2013, 7:15 AM
 */

#ifndef FL_FACTORY_H
#define	FL_FACTORY_H

#include "fl/fuzzylite.h"

#include <map>
#include <string>
#include <vector>

namespace fl {

    template <typename T>
    class FL_EXPORT Factory {
    public:
        typedef T(*Constructor)();

    protected:
        std::map<std::string, Constructor> map;

    public:
        Factory();
        virtual ~Factory();

        virtual void registerClass(const std::string& key, Constructor creator);
        virtual void deregisterClass(const std::string& key);
        virtual bool hasRegisteredClass(const std::string& key) const;
        virtual std::vector<std::string> available() const;
        virtual T createInstance(const std::string& key) const;
    };

}

#endif	/* FL_FACTORY_H */

