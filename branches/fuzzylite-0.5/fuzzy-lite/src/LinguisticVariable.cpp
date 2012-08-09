#include "LinguisticVariable.h"
#include "FuzzyExceptions.h"
#include "StringOperator.h"
#include "TriangularDTerm.h"
#include "TrapezoidDTerm.h"
#include <algorithm>
#include <math.h>

namespace fuzzy_lite {

    LinguisticVariable::LinguisticVariable() {

    }

    LinguisticVariable::LinguisticVariable(const std::string& name) : _name(name) {

    }

    LinguisticVariable::~LinguisticVariable() {

    }

    int LinguisticVariable::positionFor(flScalar value) {
        for (size_t i = 0; i < _terms.size(); ++i) {
            if (_terms[i]->numberOfCoords() <= 0) {
                continue;
            }
            if (value < _terms[i]->getCoordX(0)) {
                return i;
            }
        }
        return _terms.size();
    }

    std::string LinguisticVariable::getName() const {
        return this->_name;
    }

    void LinguisticVariable::setName(const std::string& name) {
        this->_name = name;
    }

    void LinguisticVariable::addTerm(DiscreteTerm& term) {
        _terms.insert(_terms.begin() + positionFor(term.getCoordX(0)), &term);
    }

    DiscreteTerm* LinguisticVariable::removeTerm(int index) throw (OutOfRangeException) {
        OutOfRangeException::CheckArray(FL_AT,index,numberOfTerms());
        DiscreteTerm* result = _terms[index];
        _terms.erase(_terms.begin() + index);
        return result;
    }

    DiscreteTerm* LinguisticVariable::removeTerm(const std::string& name) {
        for (int i = 0; i < numberOfTerms(); ++i) {
            if (getTerm(i).getName() == name) {
                DiscreteTerm* result = &getTerm(i);
                removeTerm(i);
                return result;
            }
        }
        return NULL;
    }

    DiscreteTerm& LinguisticVariable::getTerm(int index) const throw (OutOfRangeException) {
        OutOfRangeException::CheckArray(FL_AT,index,numberOfTerms());
        return *_terms[index];
    }

    DiscreteTerm* LinguisticVariable::firstTerm() const {
        return _terms.size() > 0 ? _terms[0] : NULL;
    }

    DiscreteTerm* LinguisticVariable::lastTerm() const {
        return _terms.size() > 0 ? _terms[_terms.size() - 1] : NULL;
    }

    DiscreteTerm* LinguisticVariable::getTerm(const std::string& name) const {
        for (size_t i = 0; i < this->_terms.size(); ++i) {
            if (_terms[i]->getName() == name) {
                return _terms[i];
            }
        }
        return NULL;
    }

    bool LinguisticVariable::isEmpty() const {
        return _terms.size() == 0;
    }

    int LinguisticVariable::numberOfTerms() const {
        return _terms.size();
    }

    flScalar LinguisticVariable::minimum() const {
        return _terms.size() == 0 ? NAN : firstTerm()->minimum();
    }

    flScalar LinguisticVariable::maximum() const {
        return _terms.size() == 0 ? NAN : lastTerm()->maximum();
    }

    DiscreteTerm LinguisticVariable::compound() const {
        DiscreteTerm result("Aggregated " + getName());
        for (int i = 0; i < numberOfTerms(); ++i) {
            result.aggregate(getTerm(i));
        }
        return result;
    }

    std::string LinguisticVariable::fuzzify(flScalar crisp) const {
        std::vector<std::string> fuzzyness;
        for (int i = 0; i < numberOfTerms(); ++i) {
            flScalar degree = fabs(getTerm(i).membership(crisp)); //sometimes it gets -0.00000000, that is why the abs
            fuzzyness.push_back(StringOperator::ScalarToString(degree) + "/" + getTerm(i).getName());
        }
        //        StringOperator::sort(fuzzyness, false);
        std::string result;
        for (size_t i = 0; i < fuzzyness.size(); ++i) {
            result += fuzzyness[i] + (i < fuzzyness.size() - 1 ? " + " : "");
        }
        return result;
    }

    DiscreteTerm* LinguisticVariable::bestFuzzyApproximation(flScalar crisp) {
        DiscreteTerm* result = NULL;
        flScalar highest_degree = -1.0;
        for (int i = 0; i < numberOfTerms(); ++i) {
            flScalar degree = getTerm(i).membership(crisp);
            if (degree > highest_degree) {
                result = &getTerm(i);
                highest_degree = degree;
            }
        }
        return result;
    }

    std::string LinguisticVariable::toString() const {
        std::string result(getName());
        result += "={ ";
        for (int i = 0; i < numberOfTerms(); ++i) {
            result += getTerm(i).getName() + " ";
        }
        result += "}";
        return result;
    }

    void LinguisticVariable::autoCreateTerms(flScalar min, flScalar max, int terms,
            std::vector<std::string>& names, MEMBERSHIP_FUNCTION mf,
            bool from_infinity, bool to_infinity) throw (InvalidArgumentException) {

        if ((size_t)terms != names.size()) {
            throw InvalidArgumentException(FL_AT, "The number of labels does not match the number of terms");
        }
        if (min > max) {
            throw InvalidArgumentException(FL_AT, "The minimum value is greater than the maximum value");
        }
        if (numberOfTerms() != 0) {
            FL_LOGW("Linguistic Variable <" << getName() << "> removed some terms before the autocreation of new ones. This may lead to memory leaks");
            _terms.clear();
        }

        flScalar step = (FuzzyOperator::Absolute(min) + FuzzyOperator::Absolute(max)) / (terms + 1);
        DiscreteTerm* x = NULL;
        int l = 0;
        for (flScalar i = min; l < terms; i += step) {
            switch (mf) {
                case MF_TRIANGULAR:
                    x = new TriangularDTerm(names[l++], i, i + 2 * step);
                    break;
                case MF_TRAPEZOIDAL:
                    x = new TrapezoidDTerm(names[l++], i, i + 2 * step);
                    break;
                default:
                    throw InvalidArgumentException(FL_AT, StringOperator::IntToString(mf) +
                            " is not an option for membership function");
            }
            addTerm(*x);
        }
        getTerm(0).setFromInfinity(from_infinity);
        getTerm(numberOfTerms() - 1).setToInfinity(to_infinity);
    }

}
