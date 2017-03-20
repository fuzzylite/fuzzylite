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

#include "fl/variable/Variable.h"

#include "fl/imex/FllExporter.h"
#include "fl/norm/Norm.h"
#include "fl/term/Constant.h"
#include "fl/term/Linear.h"

#include <queue>

namespace fl {

    Variable::Variable(const std::string& name, scalar minimum, scalar maximum)
    : _name(name), _description(""),
    _value(fl::nan), _minimum(minimum), _maximum(maximum),
    _enabled(true), _lockValueInRange(false) { }

    Variable::Variable(const Variable& other) {
        copyFrom(other);
    }

    Variable& Variable::operator=(const Variable& other) {
        if (this != &other) {
            for (std::size_t i = 0; i < _terms.size(); ++i) {
                delete _terms.at(i);
            }
            _terms.clear();
            copyFrom(other);
        }
        return *this;
    }

    void Variable::copyFrom(const Variable& other) {
        _name = other._name;
        _description = other._description;
        _value = other._value;
        _minimum = other._minimum;
        _maximum = other._maximum;
        _enabled = other._enabled;
        _lockValueInRange = other._lockValueInRange;
        for (std::size_t i = 0; i < other._terms.size(); ++i) {
            _terms.push_back(other._terms.at(i)->clone());
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

    void Variable::setDescription(const std::string& description) {
        this->_description = description;
    }

    std::string Variable::getDescription() const {
        return this->_description;
    }

    void Variable::setValue(scalar value) {
        this->_value = _lockValueInRange
                ? Op::bound(value, _minimum, _maximum)
                : value;
    }

    scalar Variable::getValue() const {
        return this->_value;
    }

    void Variable::setRange(scalar minimum, scalar maximum) {
        setMinimum(minimum);
        setMaximum(maximum);
    }

    scalar Variable::range() const {
        return getMaximum() - getMinimum();
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

    void Variable::setLockValueInRange(bool lockValueInRange) {
        this->_lockValueInRange = lockValueInRange;
    }

    bool Variable::isLockValueInRange() const {
        return this->_lockValueInRange;
    }

    Variable::Type Variable::type() const {
        return None;
    }

    Complexity Variable::complexity() const {
        Complexity result;
        if (isEnabled()) {
            for (std::size_t i = 0; i < _terms.size(); ++i) {
                result += _terms.at(i)->complexity();
            }
        }
        return result;
    }

    std::string Variable::fuzzify(scalar x) const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            Term* term = _terms.at(i);
            scalar fx = fl::nan;
            try {
                fx = term->membership(x);
            } catch (...) {
                //ignore
            }
            if (i == 0) {
                ss << Op::str(fx);
            } else {
                if (Op::isNaN(fx) or Op::isGE(fx, 0.0))
                    ss << " + " << Op::str(fx);
                else
                    ss << " - " << Op::str(std::abs(fx));
            }
            ss << "/" << term->getName();
        }
        return ss.str();
    }

    Term* Variable::highestMembership(scalar x, scalar* yhighest) const {
        Term* result = fl::null;
        scalar ymax = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            scalar y = fl::nan;
            Term* term = _terms.at(i);
            try {
                y = term->membership(x);
            } catch (...) {
                //ignore
            }
            if (Op::isGt(y, ymax)) {
                ymax = y;
                result = term;
            }
        }
        if (yhighest) *yhighest = ymax;
        return result;
    }

    std::string Variable::toString() const {
        return FllExporter().toString(this);
    }

    /**
     * Operations for datatype _terms
     */

    typedef std::pair<Term*, scalar> TermCentroid;

    struct Ascending {

        bool operator()(const TermCentroid& a, const TermCentroid& b) const {
            return a.second > b.second;
        }
    };

    void Variable::sort() {
        std::priority_queue <TermCentroid, std::vector<TermCentroid>, Ascending> termCentroids;
        Centroid defuzzifier;
        FL_DBG("Sorting...");
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            Term* term = _terms.at(i);
            scalar centroid = fl::inf;
            try {
                if (dynamic_cast<const Constant*> (term) or dynamic_cast<const Linear*> (term)) {
                    centroid = term->membership(0);
                } else {
                    centroid = defuzzifier.defuzzify(term, getMinimum(), getMaximum());
                }
            } catch (...) { //ignore error possibly due to Function not loaded
                centroid = fl::inf;
            }
            termCentroids.push(TermCentroid(term, centroid));
            FL_DBG(term->toString() << " -> " << centroid)
        }

        std::vector<Term*> sortedTerms;
        while (termCentroids.size() > 0) {
            sortedTerms.push_back(termCentroids.top().first);
            FL_DBG(termCentroids.top().first->toString() << " -> " << termCentroids.top().second);
            termCentroids.pop();
        }
        setTerms(sortedTerms);
    }

    void Variable::addTerm(Term* term) {
        _terms.push_back(term);
    }

    void Variable::insertTerm(Term* term, std::size_t index) {
        _terms.insert(_terms.begin() + index, term);
    }

    Term* Variable::getTerm(std::size_t index) const {
        return _terms.at(index);
    }

    Term* Variable::getTerm(const std::string& name) const {
        for (std::size_t i = 0; i < terms().size(); ++i) {
            if (_terms.at(i)->getName() == name) {
                return terms().at(i);
            }
        }
        throw Exception("[variable error] term <" + name + "> "
                "not found in variable <" + getName() + ">", FL_AT);
    }

    bool Variable::hasTerm(const std::string& name) const {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            if (_terms.at(i)->getName() == name) {
                return true;
            }
        }
        return false;
    }

    Term* Variable::removeTerm(std::size_t index) {
        Term* result = _terms.at(index);
        _terms.erase(_terms.begin() + index);
        return result;
    }

    std::size_t Variable::numberOfTerms() const {
        return _terms.size();
    }

    const std::vector<Term*>& Variable::terms() const {
        return this->_terms;
    }

    void Variable::setTerms(const std::vector<Term*>& terms) {
        this->_terms = terms;
    }

    std::vector<Term*>& Variable::terms() {
        return this->_terms;
    }

    Variable* Variable::clone() const {
        return new Variable(*this);
    }

}

