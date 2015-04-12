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

#ifndef FL_FACTORY_H
#define FL_FACTORY_H

#include "fl/fuzzylite.h"

#include <map>
#include <string>
#include <vector>

namespace fl {

    template <typename T>
    class FL_API ConstructionFactory {
    public:
        typedef T(*Constructor)();

    protected:
        std::string _name;
        std::map<std::string, Constructor> _constructors;

    public:
        explicit ConstructionFactory(const std::string& name);
        virtual ~ConstructionFactory();
        FL_DEFAULT_COPY_AND_MOVE(ConstructionFactory)

        virtual std::string name() const;

        virtual void registerConstructor(const std::string& key, Constructor constructor);
        virtual void deregisterConstructor(const std::string& key);
        virtual bool hasConstructor(const std::string& key) const;
        virtual Constructor getConstructor(const std::string& key) const;
        virtual T constructObject(const std::string& key) const;
        virtual std::vector<std::string> available() const;

    };

}

#endif  /* FL_FACTORY_H */

