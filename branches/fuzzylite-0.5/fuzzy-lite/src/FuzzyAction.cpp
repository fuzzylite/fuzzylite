#include "FuzzyAction.h"

#include "MamdaniRule.h"

#include <sstream>
namespace fuzzy_lite {

    FuzzyAction::FuzzyAction() : _output_lvar(NULL), _term(NULL),
    _probability(1.0) {

    }

    FuzzyAction::~FuzzyAction() {

    }

    void FuzzyAction::setOutputLVar(OutputLVar* output_lvar) {
        this->_output_lvar = output_lvar;
    }

    void FuzzyAction::setTerm(const DiscreteTerm& term) {
        this->_term = &term;
    }

    void FuzzyAction::setProbability(flScalar prob) {
        this->_probability = prob;
    }

    void FuzzyAction::execute(flScalar degree, const FuzzyOperator& fuzzy_op){
        DiscreteTerm output(*_term);
        output.modulate(degree,fuzzy_op,_probability);
        FL_LOG(output.toString());
        _output_lvar->aggregateToOutput(output);
    }

    std::string FuzzyAction::toString() const{
        std::stringstream ss;
        ss << _output_lvar->getName() << " " << MamdaniRule::FR_IS << " " << _term->getName();
        return ss.str();
    }
}
