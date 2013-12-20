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
 * Variable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/variable/Variable.h"

#include "fl/term/Term.h"

#include "fl/norm/Norm.h"

#include "fl/imex/FllExporter.h"

#include <algorithm>
#include <sstream>

namespace fl {

    Variable::Variable(const std::string& name, scalar minimum, scalar maximum)
    : _name(name), _minimum(minimum), _maximum(maximum), _enabled(true) {
    }

    Variable::Variable(const Variable& copy) {
        this->_name = copy._name;
        this->_minimum = copy._minimum;
        this->_maximum = copy._maximum;
        this->_enabled = copy._enabled;
        for (int i = 0; i < copy.numberOfTerms(); ++i) {
            addTerm(copy.getTerm(i)->copy());
        }
    }

    Variable::~Variable() {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms.at(i);
        }
    }

    void Variable::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Variable::getName() const {
        return this->_name;
    }

    void Variable::setRange(scalar minimum, scalar maximum) {
        setMinimum(minimum);
        setMaximum(maximum);
    }

    scalar Variable::range() const {
        return this->_maximum - this->_minimum;
    }

    void Variable::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Variable::getMinimum() const {
        return this->_minimum;
    }

    void Variable::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Variable::getMaximum() const {
        return this->_maximum;
    }

    void Variable::setEnabled(bool enabled) {
        this->_enabled = enabled;
    }

    bool Variable::isEnabled() const {
        return this->_enabled;
    }

    std::string Variable::fuzzify(scalar x) const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            scalar fx = _terms.at(i)->membership(x);
            if (i == 0) {
                ss << fl::Op::str(fx);
            } else {
                if (fl::Op::isNan(fx) or fl::Op::isGE(fx, 0.0))
                    ss << " + " << fl::Op::str(fx);
                else
                    ss << " - " << fl::Op::str(std::fabs(fx));
            }
            ss << "/" << _terms.at(i)->getName();
        }
        return ss.str();
    }

    Term* Variable::highestMembership(scalar x, scalar* yhighest) const {
        Term* result = NULL;
        scalar ymax = -fl::inf;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            scalar y = _terms.at(i)->membership(x);
            if (fl::Op::isGt(y, ymax)) {
                ymax = y;
                result = _terms.at(i);
            }
        }
        if (yhighest) *yhighest = ymax;
        return result;
    }

    std::string Variable::toString() const {
        return FllExporter("", "; ").toString(this);
    }

    /**
     * Operations for datatype _terms
     */

    void Variable::sort() {
        std::map<const Term*, scalar> centroids;
        Centroid defuzzifier;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            Term* term = _terms.at(i);
            centroids[term] = defuzzifier.defuzzify(term, _minimum, _maximum);
        }
        SortByCoG criterion;
        criterion.centroids = centroids;
        std::sort(_terms.begin(), _terms.end(), criterion);
    }

    void Variable::addTerm(Term* term) {
        this->_terms.push_back(term);
    }

    void Variable::insertTerm(Term* term, int index) {
        this->_terms.insert(this->_terms.begin() + index, term);
    }

    Term* Variable::getTerm(int index) const {
        return this->_terms.at(index);
    }

    Term* Variable::getTerm(const std::string& name) const {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            if (_terms.at(i)->getName() == name) {
                return _terms.at(i);
            }
        }
        throw fl::Exception("[variable error] term <" + name + "> "
                "not found in variable <" + this->_name + ">", FL_AT);
    }

    bool Variable::hasTerm(const std::string& name) const {
        return getTerm(name) != NULL;
    }

    Term* Variable::removeTerm(int index) {
        Term* result = this->_terms.at(index);
        this->_terms.erase(this->_terms.begin() + index);
        return result;
    }

    int Variable::numberOfTerms() const {
        return this->_terms.size();
    }

    const std::vector<Term*>& Variable::terms() const {
        return this->_terms;
    }

}

