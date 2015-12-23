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

#include "fl/term/Aggregated.h"

#include "fl/imex/FllExporter.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/s/Maximum.h"
#include "fl/term/Activated.h"


namespace fl {

    Aggregated::Aggregated(const std::string& name, scalar minimum, scalar maximum,
            SNorm* aggregation)
    : Term(name), _minimum(minimum), _maximum(maximum), _aggregation(aggregation) {
    }

    Aggregated::Aggregated(const Aggregated& other) : Term(other) {
        copyFrom(other);
    }

    Aggregated& Aggregated::operator=(const Aggregated& other) {
        if (this != &other) {
            clear();
            _aggregation.reset(fl::null);

            Term::operator=(other);
            copyFrom(other);
        }
        return *this;
    }

    Aggregated::~Aggregated() {
        clear();
    }

    void Aggregated::copyFrom(const Aggregated& source) {
        _minimum = source._minimum;
        _maximum = source._maximum;

        if (source._aggregation.get())
            _aggregation.reset(source._aggregation->clone());

        for (std::size_t i = 0; i < source._terms.size(); ++i) {
            _terms.push_back(source._terms.at(i)->clone());
        }
    }

    std::string Aggregated::className() const {
        return "Aggregated";
    }

    scalar Aggregated::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        if (not (terms().empty() or getAggregation())) { //Exception for IntegralDefuzzifiers
            throw fl::Exception("[aggregation error] "
                    "aggregation operator needed to aggregate " + toString(), FL_AT);
        }
        scalar mu = 0.0;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            mu = getAggregation()->compute(mu, terms().at(i)->membership(x));
        }
        return mu;
    }

    scalar Aggregated::activationDegree(const Term* forTerm) const {
        scalar result = 0.0;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            Activated* activatedTerm = terms().at(i);
            if (activatedTerm->getTerm() == forTerm) {
                if (getAggregation())
                    result = getAggregation()->compute(result, activatedTerm->getDegree());
                else
                    result += activatedTerm->getDegree(); //Default for WeightDefuzzifier
            }
        }
        return result;
    }

    std::string Aggregated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << exporter.toString(getAggregation());
        ss << " " << Op::str(getMinimum()) << " " << Op::str(getMaximum()) << " ";
        for (std::size_t i = 0; i < terms().size(); ++i) {
            ss << " " << exporter.toString(terms().at(i));
        }
        return ss.str();
    }

    void Aggregated::configure(const std::string& parameters) {
        FL_IUNUSED(parameters);
    }

    Aggregated* Aggregated::clone() const {
        return new Aggregated(*this);
    }

    std::string Aggregated::toString() const {
        std::vector<std::string> aggregate;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            aggregate.push_back(terms().at(i)->toString());
        }
        FllExporter exporter;
        std::ostringstream ss;
        ss << getName() << ": " << className() << " "
                << exporter.toString(getAggregation()) << "["
                << fl::Op::join(aggregate, ",") << "]";
        return ss.str();
    }

    void Aggregated::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }

    scalar Aggregated::getMinimum() const {
        return this->_minimum;
    }

    void Aggregated::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }

    scalar Aggregated::getMaximum() const {
        return this->_maximum;
    }

    void Aggregated::setRange(scalar minimum, scalar maximum) {
        setMinimum(minimum);
        setMaximum(maximum);
    }

    scalar Aggregated::range() const {
        return getMaximum() - getMinimum();
    }

    void Aggregated::setAggregation(SNorm* aggregation) {
        this->_aggregation.reset(aggregation);
    }

    SNorm* Aggregated::getAggregation() const {
        return this->_aggregation.get();
    }

    /**
     * Operations for std::vector _terms
     */


    void Aggregated::addTerm(const Term* term, scalar degree, const TNorm* implication) {
        terms().push_back(new Activated(term, degree, implication));
    }

    void Aggregated::addTerm(Activated* term) {
        terms().push_back(term);
    }

    Activated* Aggregated::removeTerm(std::size_t index) {
        Activated* term = terms().at(index);
        terms().erase(terms().begin() + index);
        return term;
    }

    void Aggregated::clear() {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms.at(i);
        }
        _terms.clear();
    }

    Activated* Aggregated::getTerm(std::size_t index) const {
        return terms().at(index);
    }

    const std::vector<Activated*>& Aggregated::terms() const {
        return this->_terms;
    }

    std::vector<Activated*>& Aggregated::terms() {
        return this->_terms;
    }

    std::size_t Aggregated::numberOfTerms() const {
        return terms().size();
    }

    bool Aggregated::isEmpty() const {
        return terms().empty();
    }

}
