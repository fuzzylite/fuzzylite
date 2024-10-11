/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#ifndef FL_CLONINGFACTORY_H
#define FL_CLONINGFACTORY_H

#include <map>
#include <string>
#include <vector>

#include "fuzzylite/fuzzylite.h"

namespace fuzzylite {

    /**
      The CloningFactory class is the base class for a factory whose objects
      are created from a registered object by calling the `clone()` method.

      @param <T> is the class of the object to be cloned

      @author Juan Rada-Vilela, Ph.D.
      @see FactoryManager
      @since 5.0
     */

    template <typename T>
    class CloningFactory {
      private:
        std::string _name;
        std::map<std::string, T> _objects;

      protected:
        /**
         * Copies the contents of the factory into this one
         * @param other a cloning factory
         */
        void copyFrom(const CloningFactory& other);

      public:
        explicit CloningFactory(const std::string& name = "");
        CloningFactory(const CloningFactory& other);
        CloningFactory& operator=(const CloningFactory& other);
        virtual ~CloningFactory();
        FL_DEFAULT_MOVE(CloningFactory)

        /**
          Returns the name of the factory
          @return the name of the factory
         */
        virtual std::string name() const;

        /**
          Registers the object in the factory and assumes its ownership
          @param key is the unique name by which objects are registered
          @param object is the object to be cloned via a `clone` method
         */
        virtual void registerObject(const std::string& key, T object);
        /**
          Deregisters the given object from the factory and deletes it
          @param key is the unique name by which objects are registered
         */
        virtual void deregisterObject(const std::string& key);
        /**
          Checks whether the factory has the given object registered
          @param key is the unique name by which objects are registered
          @return whether the factory has the given object registered
         */
        virtual bool hasObject(const std::string& key) const;
        /**
          Gets the object registered by the given key, not a clone of the object
          @param key is the unique name by which objects are registered
          @return the object registered by the given key
         */
        virtual T getObject(const std::string& key) const;
        /**
          Creates a cloned object by executing the clone method on the registered object
          @param key is the unique name by which objects are registered
          @return a cloned object by executing the clone method on the registered object
         */
        virtual T cloneObject(const std::string& key) const;
        /**
          Returns a vector of the available objects
          @return a vector of the available objects
         */
        virtual std::vector<std::string> available() const;
        /**
          Gets the map of registered keys and objects
          @return the map of registered keys and objects
         */
        virtual std::map<std::string, T>& objects();
        /**
          Gets an immutable map of registered keys and objects
          @return an immutable map of registered keys and objects
         */
        virtual const std::map<std::string, T>& objects() const;

        /**
         * Removes and deletes the contents of the factory
         */
        virtual void clear();
        /**
         * Clones the factory
         * @return a clone of the factory
         */
        virtual CloningFactory* clone() const;
    };
}

/**
  Template implementation
 */

#include "fuzzylite/Exception.h"

namespace fuzzylite {

    template <typename T>
    inline CloningFactory<T>::CloningFactory(const std::string& name) : _name(name) {}

    template <typename T>
    inline CloningFactory<T>::CloningFactory(const CloningFactory& other) {
        copyFrom(other);
    }

    template <typename T>
    inline CloningFactory<T>& CloningFactory<T>::operator=(const CloningFactory& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    template <typename T>
    inline CloningFactory<T>::~CloningFactory() {
        CloningFactory::clear();
    }

    template <typename T>
    inline std::string CloningFactory<T>::name() const {
        return this->_name;
    }

    template <typename T>
    inline void CloningFactory<T>::registerObject(const std::string& key, T object) {
        this->_objects[key] = object;
    }

    template <typename T>
    inline void CloningFactory<T>::deregisterObject(const std::string& key) {
        typename std::map<std::string, T>::iterator it = this->_objects.find(key);
        if (it != this->_objects.end()) {
            if (it->second)
                delete it->second;
            this->_objects.erase(it);
        }
    }

    template <typename T>
    inline bool CloningFactory<T>::hasObject(const std::string& key) const {
        typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
        return it != this->_objects.end();
    }

    template <typename T>
    inline T CloningFactory<T>::getObject(const std::string& key) const {
        typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
        if (it != this->_objects.end()) {
            if (it->second)
                return it->second;
        }
        return fl::null;
    }

    template <typename T>
    inline T CloningFactory<T>::cloneObject(const std::string& key) const {
        typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
        if (it != this->_objects.end()) {
            if (it->second)
                return it->second->clone();
            return fl::null;
        }
        throw Exception("[cloning error] " + _name + " object by name <" + key + "> not registered", FL_AT);
    }

    template <typename T>
    inline std::vector<std::string> CloningFactory<T>::available() const {
        std::vector<std::string> result;
        typename std::map<std::string, T>::const_iterator it = this->_objects.begin();
        while (it != this->_objects.end()) {
            result.push_back(it->first);
            ++it;
        }
        return result;
    }

    template <typename T>
    inline std::map<std::string, T>& CloningFactory<T>::objects() {
        return this->_objects;
    }

    template <typename T>
    inline const std::map<std::string, T>& CloningFactory<T>::objects() const {
        return this->_objects;
    }

    template <typename T>
    inline void CloningFactory<T>::clear() {
        typename std::map<std::string, T>::const_iterator it = this->_objects.begin();
        while (it != this->_objects.end()) {
            if (it->second)
                delete it->second;
            ++it;
        }
        this->_objects.clear();
    }

    template <typename T>
    void CloningFactory<T>::copyFrom(const CloningFactory& other) {
        this->_name = other._name;
        typename std::map<std::string, T>::const_iterator it = other._objects.begin();
        while (it != other._objects.end()) {
            T clone = fl::null;
            if (it->second)
                clone = it->second->clone();
            deregisterObject(it->first);
            registerObject(it->first, clone);

            ++it;
        }
    }

    template <typename T>
    CloningFactory<T>* CloningFactory<T>::clone() const {
        return new CloningFactory<T>(*this);
    }

}

#endif /* FL_CLONINGFACTORY_H */
