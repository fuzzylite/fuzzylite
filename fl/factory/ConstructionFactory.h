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

#ifndef FL_CONSTRUCTIONFACTORY_H
#define FL_CONSTRUCTIONFACTORY_H

#include "fl/fuzzylite.h"

#include <map>
#include <string>
#include <vector>

namespace fl {

    /**
      The ConstructionFactory class is the base class for a factory whose
      objects are created from a registered ConstructionFactory::Constructor.

      @author Juan Rada-Vilela, Ph.D.
      @see FactoryManager
      @since 5.0
     */

    template <typename T>
    class ConstructionFactory {
    public:
        /**
          The Constructor type definition refers to a zero-parameter method
          which returns an instance of T
         */
        typedef T(*Constructor)();

    private:
        std::string _name;
        std::map<std::string, Constructor> _constructors;

    public:
        explicit ConstructionFactory(const std::string& name);
        virtual ~ConstructionFactory();
        FL_DEFAULT_COPY_AND_MOVE(ConstructionFactory)

        /**
          Returns the name of the factory
          @return the name of the factory
         */
        virtual std::string name() const;

        /**
          Registers the constructor in the factory
          @param key is the unique name by which constructors are registered
          @param constructor is the pointer to the constructor of the object
         */
        virtual void registerConstructor(const std::string& key, Constructor constructor);
        /**
          Deregisters from the factory the constructor associated to the given key
          @param key is the unique name by which constructors are registered
         */
        virtual void deregisterConstructor(const std::string& key);
        /**
          Checks whether the factory has a constructor registered by the given key
          @param key is the unique name by which constructors are registered
          @return whether the factory has the given constructor registered
         */
        virtual bool hasConstructor(const std::string& key) const;
        /**
          Gets the constructor registered by the given key
          @param key is the unique name by which constructors are registered
          @return the pointer to the given constructor
         */
        virtual Constructor getConstructor(const std::string& key) const;
        /**
          Creates an object by executing the constructor associated to the given key
          @param key is the unique name by which constructors are registered
          @return an object by executing the constructor associated to the given key
         */
        virtual T constructObject(const std::string& key) const;
        /**
          Returns a vector of keys for the constructors available
          @return a vector of keys for the constructors available
         */
        virtual std::vector<std::string> available() const;
        /**
          Gets the map of registered keys and constructors
          @return the map of registered keys and constructors
         */
        virtual std::map<std::string, Constructor>& constructors();
        /**
          Gets an immutable map of registered keys and constructors
          @return an immutable map of registered keys and constructors
         */
        virtual const std::map<std::string, Constructor>& constructors() const;
    };

}

/**
 * Template implementation
 */


#include "fl/Exception.h"
#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/hedge/Hedge.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"
#include "fl/term/Term.h"

namespace fl {

    template <typename T>
    inline ConstructionFactory<T>::ConstructionFactory(const std::string& name) : _name(name) {

    }

    template <typename T>
    inline ConstructionFactory<T>::~ConstructionFactory() {
    }

    template<typename T>
    inline std::string ConstructionFactory<T>::name() const {
        return this->_name;
    }

    template <typename T>
    inline void ConstructionFactory<T>::registerConstructor(const std::string& key, Constructor constructor) {
        this->_constructors[key] = constructor;
    }

    template <typename T>
    inline void ConstructionFactory<T>::deregisterConstructor(const std::string& key) {
        typename std::map<std::string, Constructor>::iterator it = this->_constructors.find(key);
        if (it != this->_constructors.end()) {
            this->_constructors.erase(it);
        }
    }

    template <typename T>
    inline bool ConstructionFactory<T>::hasConstructor(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
        return (it != this->_constructors.end());
    }

    template <typename T>
    inline typename ConstructionFactory<T>::Constructor ConstructionFactory<T>::getConstructor(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
        if (it != this->_constructors.end()) {
            return it->second;
        }
        return fl::null;
    }

    template <typename T>
    inline T ConstructionFactory<T>::constructObject(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
        if (it != this->_constructors.end()) {
            if (it->second) {
                return it->second();
            }
            return fl::null;
        }
        std::ostringstream ss;
        ss << "[factory error] constructor of " + _name + " <" << key << "> not registered";
        throw Exception(ss.str(), FL_AT);
    }

    template <typename T>
    inline std::vector<std::string> ConstructionFactory<T>::available() const {
        std::vector<std::string> result;
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.begin();
        while (it != this->_constructors.end()) {
            result.push_back(it->first);
            ++it;
        }
        return result;
    }

    template<typename T>
    inline std::map<std::string, typename ConstructionFactory<T>::Constructor>& ConstructionFactory<T>::constructors() {
        return this->_constructors;
    }

    template<typename T>
    inline const std::map<std::string, typename ConstructionFactory<T>::Constructor>& ConstructionFactory<T>::constructors() const {
        return this->_constructors;
    }
}

#endif  /* FL_CONSTRUCTIONFACTORY_H */

