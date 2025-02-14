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

#include "fuzzylite/term/Aggregated.h"

#include "fuzzylite/imex/FllExporter.h"
#include "fuzzylite/norm/s/Maximum.h"
#include "fuzzylite/norm/s/UnboundedSum.h"

namespace fuzzylite {

    Aggregated::Aggregated(
        const std::string& name, scalar minimum, scalar maximum, SNorm* aggregation, const std::vector<Activated>& terms
    ) :
        Term(name),
        _minimum(minimum),
        _maximum(maximum),
        _aggregation(aggregation),
        _terms(terms) {}

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

    Aggregated::~Aggregated() {}

    void Aggregated::copyFrom(const Aggregated& source) {
        _minimum = source._minimum;
        _maximum = source._maximum;

        if (source._aggregation.get())
            _aggregation.reset(source._aggregation->clone());

        for (std::size_t i = 0; i < source._terms.size(); ++i)
            _terms.push_back(source._terms.at(i));
    }

    std::string Aggregated::className() const {
        return "Aggregated";
    }

    scalar Aggregated::membership(scalar x) const {
        if (Op::isNaN(x))
            return fl::nan;
        if (not _terms.empty() and not _aggregation.get()) {  // Exception for IntegralDefuzzifiers
            throw Exception(
                "[aggregation error] "
                "aggregation operator needed to aggregate variable "
                "<" + getName()
                    + ">",
                FL_AT
            );
        }
        scalar mu = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i)
            mu = _aggregation->compute(mu, _terms.at(i).membership(x));
        return mu;
    }

    scalar Aggregated::activationDegree(const Term* forTerm) const {
        scalar result = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            const Activated& activatedTerm = _terms.at(i);
            if (activatedTerm.getTerm() == forTerm) {
                if (_aggregation.get())
                    result = _aggregation->compute(result, activatedTerm.getDegree());
                else
                    result += activatedTerm.getDegree();  // Default for WeightDefuzzifier
            }
        }
        return result;
    }

    Activated Aggregated::highestActivatedTerm() const {
        std::vector<Activated> maxTerms = maximallyActivatedTerms();
        if (maxTerms.empty())
            return Activated(fl::null, 0.0);
        return maxTerms.front();
    }

    std::vector<Activated> Aggregated::maximallyActivatedTerms() const {
        std::vector<Activated> maxActivatedTerms;
        scalar maxDegree = 0.0;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            const Activated& term = terms().at(i);
            if (term.getDegree() > maxDegree) {
                maxActivatedTerms.clear();
                maxActivatedTerms.push_back(term);
                maxDegree = term.getDegree();
            } else if (maxDegree > 0.0 and maxDegree == term.getDegree()) {
                maxActivatedTerms.push_back(term);
            }
        }
        return maxActivatedTerms;
    }

    std::vector<Activated> Aggregated::groupedTerms() const {
        return grouped().terms();
    }

    Aggregated Aggregated::grouped() const {
        const UnboundedSum sum;
        const SNorm* aggregation = getAggregation() ? getAggregation() : &sum;
        std::map<std::string, Activated> groups;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            const Activated& activated = getTerm(i);
            std::map<std::string, Activated>::iterator it = groups.find(activated.getTerm()->getName());
            if (it == groups.end())
                groups[activated.getTerm()->getName()] = activated;
            else {
                Activated& groupedTerm = it->second;
                const scalar groupedDegree = aggregation->compute(groupedTerm.getDegree(), activated.getDegree());
                groupedTerm.setDegree(groupedDegree);
            }
        }

        std::vector<Activated> activations;
        activations.reserve(groups.size());
        // Preserve order of terms when grouping
        for (std::size_t i = 0; i < terms().size() and not groups.empty(); ++i) {
            const Activated& activated = terms().at(i);
            std::map<std::string, Activated>::iterator it = groups.find(activated.getTerm()->getName());
            if (it != groups.end()) {
                activations.push_back(it->second);
                groups.erase(it);
            }
        }

        Aggregated result(getName(), getMinimum(), getMaximum(), null, activations);
        if (getAggregation())
            result.setAggregation(getAggregation()->clone());
        return result;
    }

    std::string Aggregated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << exporter.toString(getAggregation());
        ss << " " << Op::str(getMinimum()) << " " << Op::str(getMaximum());
        for (std::size_t i = 0; i < terms().size(); ++i)
            ss << " " << exporter.toString(&terms().at(i));
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
        for (std::size_t i = 0; i < terms().size(); ++i)
            aggregate.push_back(terms().at(i).toString());
        FllExporter exporter;
        std::ostringstream ss;
        if (getAggregation()) {
            ss << getName() << ": " << className() << " " << exporter.toString(getAggregation()) << "["
               << Op::join(aggregate, ",") << "]";
        } else {
            ss << getName() << ": " << className() << " " << "[" << Op::join(aggregate, "+") << "]";  //\u2295: (+)
        }
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
        _terms.push_back(Activated(term, degree, implication));
        FL_DBG("Aggregating " << _terms.back().toString());
    }

    void Aggregated::addTerm(const Activated& term) {
        _terms.push_back(term);
        FL_DBG("Aggregating " << _terms.back().toString());
    }

    const Activated& Aggregated::removeTerm(std::size_t index) {
        const Activated& term = _terms.at(index);
        _terms.erase(_terms.begin() + index);
        return term;
    }

    void Aggregated::clear() {
        _terms.clear();
    }

    const Activated& Aggregated::getTerm(std::size_t index) const {
        return _terms.at(index);
    }

    void Aggregated::setTerms(const std::vector<Activated>& terms) {
        this->_terms = terms;
    }

    const std::vector<Activated>& Aggregated::terms() const {
        return this->_terms;
    }

    std::vector<Activated>& Aggregated::terms() {
        return this->_terms;
    }

    std::size_t Aggregated::numberOfTerms() const {
        return _terms.size();
    }

    bool Aggregated::isEmpty() const {
        return _terms.empty();
    }

    std::string Aggregated::fuzzyValue() const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            const std::string fuzzyValue = terms().at(i).fuzzyValue();
            const char sign = fuzzyValue.at(0);
            const std::string value = fuzzyValue.substr(1);
            if (i == 0 and sign == '-')
                ss << sign;
            if (i > 0)
                ss << ' ' << sign << ' ';
            ss << value;
        }
        return ss.str();
    }

    Aggregated& Aggregated::terms(const std::vector<Activated>& terms) {
        _terms.insert(_terms.end(), terms.begin(), terms.end());
        return *this;
    }

    Aggregated& Aggregated::term(const Term* term, scalar degree, const TNorm* implication) {
        terms().push_back(Activated(term, degree, implication));
        return *this;
    }

    Aggregated& Aggregated::range(scalar minimum, scalar maximum) {
        setMinimum(minimum);
        setMaximum(maximum);
        return *this;
    }

    Aggregated& Aggregated::aggregation(SNorm* aggregation) {
        setAggregation(aggregation);
        return *this;
    }
}
