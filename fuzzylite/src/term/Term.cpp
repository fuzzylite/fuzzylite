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
 * File:   Term.cpp
 * Author: jcrada
 *
 * Created on 18 December 2013, 9:55 AM
 */

#include "fl/term/Term.h"
#include "fl/imex/FllExporter.h"

namespace fl {

    Term::Term(const std::string& name) : _name(name) {

    }

    Term::~Term() {

    }

    void Term::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Term::getName() const {
        return this->_name;
    }

    std::string Term::toString() const {
        return FllExporter("", "; ").toString(this);
    }
}