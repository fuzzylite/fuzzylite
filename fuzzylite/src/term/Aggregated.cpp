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
            _terms.push_back(source._terms.at(i));
        }
    }

    std::string Aggregated::className() const {
        return "Aggregated";
    }

    scalar Aggregated::membership(scalar x) const {
        if (FL_IS_NAN(x)) return fl::nan;
        if (not (_terms.empty() or _aggregation.get())) { //Exception for IntegralDefuzzifiers
            throw fl::Exception("[aggregation error] "
                    "aggregation operator needed to aggregate variable "
                    "<" + getName() + ">", FL_AT);
        }
        scalar mu = 0.0;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            mu = _aggregation->compute(mu, _terms.at(i).membership(x));
        }
        return mu;
    }

    scalar Aggregated::activationDegree(const Term* forTerm) const {
        scalar result = 0.0;
        for (std::size_t i = 0; i < terms().size(); ++i) {
            const Activated& activatedTerm = terms().at(i);
            if (activatedTerm.getTerm() == forTerm) {
                if (getAggregation())
                    result = getAggregation()->compute(result, activatedTerm.getDegree());
                else
                    result += activatedTerm.getDegree(); //Default for WeightDefuzzifier
            }
        }
        return result;
    }

    const Activated* Aggregated::highestActivatedTerm() const {
        const Activated* maximumTerm = fl::null;
        fl::scalar maximumActivation = -fl::inf;
        for (std::size_t i = 0  ; i < _terms.size(); ++i){
            const Activated& activated = _terms.at(i);
            if (fl::Op::isGt(activated.getDegree(), maximumActivation)){
                maximumActivation = activated.getDegree();
                maximumTerm = &activated;
            }
        }
        return maximumTerm;
    }

    std::string Aggregated::parameters() const {
        FllExporter exporter;
        std::ostringstream ss;
        ss << exporter.toString(getAggregation());
        ss << " " << Op::str(getMinimum()) << " " << Op::str(getMaximum()) << " ";
        for (std::size_t i = 0; i < terms().size(); ++i) {
            ss << " " << exporter.toString(&terms().at(i));
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
            aggregate.push_back(terms().at(i).toString());
        }
        FllExporter exporter;
        std::ostringstream ss;
        if (getAggregation()) {
            ss << getName() << ": " << className() << " "
                    << exporter.toString(getAggregation()) << "["
                    << fl::Op::join(aggregate, ",") << "]";
        } else {
            ss << getName() << ": " << className() << " " << "["
                    << fl::Op::join(aggregate, "+") << "]"; //\u2295: (+)
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
    }

    void Aggregated::addTerm(const Activated& term) {
        _terms.push_back(term);
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

}
