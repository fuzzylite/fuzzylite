/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/term/Accumulated.h"

#include "fl/imex/FllExporter.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/s/Maximum.h"
#include "fl/term/Activated.h"


namespace fl {

    Accumulated::Accumulated(const std::string& name, scalar minimum, scalar maximum,
            SNorm* accumulation)
    : Term(name), _minimum(minimum), _maximum(maximum), _accumulation(accumulation) {
    }

    Accumulated::Accumulated(const Accumulated& other) : Term(other) {
        copyFrom(other);
    }

    Accumulated& Accumulated::operator=(const Accumulated& other) {
        if (this != &other) {
            clear();
            _accumulation.reset(fl::null);

            Term::operator=(other);
            copyFrom(other);
        }
        return *this;
    }

    Accumulated::~Accumulated() {
        clear();
    }

    void Accumulated::copyFrom(const Accumulated& source) {
        _minimum = source._minimum;
        _maximum = source._maximum;

        if (source._accumulation.get())
            _accumulation.reset(source._accumulation->clone());

        for (std::size_t i = 0; i < source._terms.size(); ++i) {
            _terms.push_back(source._terms.at(i)->clone());
        }
    }

    std::string Accumulated::className() const {
        return "Accumulated";
    }

    scalar Accumulated::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        if (not (terms().empty() or getAccumulation())) { //Exception for IntegralDefuzzifiers
            throw fl::Exception("[accumulation error] "
                    "accumulation operator needed to accumulate " + toString(), FL_AT);
        }
        scalar mu = 0.0;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            mu = getAccumulation()->compute(mu, terms().at(i)->membership(x));
        }
        return mu;
    }

    scalar Accumulated::activationDegree(const Term* forTerm) const {
        scalar result = 0.0;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            Activated* activatedTerm = terms().at(i);
            if (activatedTerm->getTerm() == forTerm) {
                if (getAccumulation())
                    result = getAccumulation()->compute(result, activatedTerm->getDegree());
                else
                    result += activatedTerm->getDegree(); //Default for WeightDefuzzifier
            }
        }
        return result;
    }

    std::string Accumulated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << exporter.toString(getAccumulation());
        ss << " " << Op::str(getMinimum()) << " " << Op::str(getMaximum()) << " ";
        for (std::size_t i = 0; i < terms().size(); ++i) {
            ss << " " << exporter.toString(terms().at(i));
        }
        return ss.str();
    }

    void Accumulated::configure(const std::string& parameters) {
        (void) parameters;
    }

    Accumulated* Accumulated::clone() const {
        return new Accumulated(*this);
    }

    std::string Accumulated::toString() const {
        std::vector<std::string> accumulate;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            accumulate.push_back(terms().at(i)->toString());
        }
        FllExporter exporter;
        std::ostringstream ss;
        ss << getName() << ": " << className() << " "
                << exporter.toString(getAccumulation()) << "["
                << fl::Op::join(accumulate, ",") << "]";
        return ss.str();
    }

    void Accumulated::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Accumulated::getMinimum() const {
        return this->_minimum;
    }

    void Accumulated::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Accumulated::getMaximum() const {
        return this->_maximum;
    }

    void Accumulated::setRange(scalar minimum, scalar maximum) {
        setMinimum(minimum);
        setMaximum(maximum);
    }

    scalar Accumulated::range() const {
        return getMaximum() - getMinimum();
    }

    void Accumulated::setAccumulation(SNorm* accumulation) {
        this->_accumulation.reset(accumulation);
    }

    SNorm* Accumulated::getAccumulation() const {
        return this->_accumulation.get();
    }

    /**
     * Operations for std::vector _terms
     */


    void Accumulated::addTerm(const Term* term, scalar degree, const TNorm* implication) {
        terms().push_back(new Activated(term, degree, implication));
    }

    void Accumulated::addTerm(Activated* term) {
        terms().push_back(term);
    }

    Activated* Accumulated::removeTerm(std::size_t index) {
        Activated* term = terms().at(index);
        terms().erase(terms().begin() + index);
        return term;
    }

    void Accumulated::clear() {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms.at(i);
        }
        _terms.clear();
    }

    Activated* Accumulated::getTerm(std::size_t index) const {
        return terms().at(index);
    }

    const std::vector<Activated*>& Accumulated::terms() const {
        return this->_terms;
    }

    std::vector<Activated*>& Accumulated::terms() {
        return this->_terms;
    }

    std::size_t Accumulated::numberOfTerms() const {
        return terms().size();
    }

    bool Accumulated::isEmpty() const {
        return terms().empty();
    }

}
