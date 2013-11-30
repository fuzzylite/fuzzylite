/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * Exception.h
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXCEPTION_H
#define FL_EXCEPTION_H

#include "fl/fuzzylite.h"

#include <exception>
#include <string>
#include <vector>

namespace fl {

    class FL_EXPORT Exception : public std::exception {
    protected:
        std::string _what;
    public:
        Exception(const std::string& what, const std::string& file, int line,
                const std::string& function);
        virtual ~Exception() throw ();

        virtual void setWhat(const std::string& what);
        virtual std::string getWhat() const;
        virtual const char* what() const throw ();
        
        virtual void append(const std::string& whatElse);
        virtual void append(const std::string& file, int line, const std::string& function);
        virtual void append(const std::string& whatElse,
            const std::string& file, int line, const std::string& function);

        static std::string btCallStack(const int maxCalls = 30);

        static void signalHandler(int signal);
        
        static void terminate();
    };

}
#endif /* FL_EXCEPTION_H */
