/* 
 * File:   AbstractFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:51 PM
 */

#ifndef FL_ABSTRACTFACTORY_H
#define	FL_ABSTRACTFACTORY_H

#include <map>
#include <string>

namespace fl {

    template <class T>
    class AbstractFactory {
    protected:
        std::map<std::string, T> _factory;
    public:
        AbstractFactory();

        virtual ~AbstractFactory() {
            for (std::map<std::string, T>::iterator it = _factory.begin();
                    it != _factory.end(); ++it) {
                delete it->second;
            }
        }

        virtual void add(const std::string& key, T value) {
            std::map<std::string, T>::const_iterator it = _factory.find(key);
            if (it != _factory.end() and it->second) {
                delete it->second;
            }
            it[key] = value;
        }

        virtual void remove(const std::string& key) {
            std::map<std::string, T>::const_iterator it = _factory.find(key);
            if (it != _factory.end()) {
                if (it->second) delete it->second;
                _factory.erase(it);
            }
        }

        virtual T get(const std::string& key) const {
            std::map<std::string, T>::const_iterator it = _factory.find(key);
            if (it != _factory.end()) {
                return it->second;
            }
            return NULL;
        }

        virtual bool contains(const std::string& key) const {
            return get(key) != NULL;
        }

        virtual std::string toString() const {
            std::ostringstream ss;
            ss << "{ ";
            for (std::map<std::string, T>::const_iterator it = _factory.begin();
                    it != _factory.end(); ++it) {
                ss << it->first << ":" << it->second << " ";
            }
            ss << "}";
            return ss.str();
        }
    };
}
#endif	/* FL_ABSTRACTFACTORY_H */

