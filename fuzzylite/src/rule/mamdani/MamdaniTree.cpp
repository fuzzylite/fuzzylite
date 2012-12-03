/*
 * MamdaniTree.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "MamdaniTree.h"

#include "../../variable/Variable.h"
#include "../../hedge/Hedge.h"
#include "../../term/Term.h"
#include "../../rule/Rule.h"
#include "../../engine/Operator.h"

#include <sstream>

namespace fl {

    MamdaniProposition::MamdaniProposition() {
    }

    MamdaniProposition::~MamdaniProposition() {
    }

    void MamdaniProposition::setVariable(Variable* variable) {
        this->_variable = variable;
    }
    Variable* MamdaniProposition::getVariable() const {
        return this->_variable;
    }

    void MamdaniProposition::setTerm(Term* term) {
        this->_term = term;
    }
    Term* MamdaniProposition::getTerm() const {
        return this->_term;
    }
    void MamdaniProposition::setWeight(scalar weight) {
        this->_weight = weight;
    }
    scalar MamdaniProposition::getWeight() const {
        return this->_weight;
    }

    std::string MamdaniProposition::toString() const {
        std::stringstream ss;
        ss << this->_variable->getName() << " " <<
                Rule::FL_IS << " ";
        for (std::size_t i = 0; i < this->_hedges.size(); ++i) {
            ss << this->_hedges[i]->name() << " ";
        }
        ss << this->_term->getName();
        if (!Op::IsNan(this->_weight)) {
            ss << " " << Rule::FL_WITH << " " << this->_weight;
        }
        return ss.str();
    }

    /**
     * Operations for datatype _hedges
     */
    void MamdaniProposition::addHedge(Hedge* hedge) {
        this->_hedges.push_back(hedge);
    }
    void MamdaniProposition::insertHedge(Hedge* hedge, int index) {
        this->_hedges.insert(this->_hedges.begin() + index, hedge);
    }
    Hedge* MamdaniProposition::getHedge(int index) const {
        return this->_hedges[index];
    }
    Hedge* MamdaniProposition::removeHedge(int index) {
        Hedge* result = this->_hedges[index];
        this->_hedges.erase(this->_hedges.begin() + index);
        return result;
    }
    int MamdaniProposition::numberOfHedges() const {
        return this->_hedges.size();
    }
    const std::vector<Hedge*>& MamdaniProposition::hedges() const {
        return this->_hedges;
    }

} /* namespace fl */
