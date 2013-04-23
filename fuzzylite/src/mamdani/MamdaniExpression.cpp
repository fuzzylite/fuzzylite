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

#include "fl/rule/mamdani/MamdaniExpression.h"

#include "fl/variable/Variable.h"
#include "fl/hedge/Hedge.h"
#include "fl/term/Term.h"
#include "fl/rule/Rule.h"

namespace fl {

    MamdaniExpression::MamdaniExpression(bool isOperator)
    : isOperator(isOperator) {
    }

    MamdaniExpression::~MamdaniExpression() {
    }

    MamdaniProposition::MamdaniProposition()
    : MamdaniExpression(false), variable(NULL), term(NULL) {
    }

    std::string MamdaniProposition::toString() const {
        std::stringstream ss;
        ss << variable->getName() << " " << Rule::FL_IS << " ";
        for (std::size_t i = 0; i < hedges.size(); ++i) {
            ss << hedges.at(i)->name() << " ";
        }
        ss << term->getName();
        return ss.str();
    }

    MamdaniOperator::MamdaniOperator()
    : MamdaniExpression(true), left(NULL), right(NULL) {
    }

    MamdaniOperator::~MamdaniOperator() {
        if (left)
            delete left;
        if (right)
            delete right;
    }

    std::string MamdaniOperator::toString() const {
        return name;
    }

}
