#include "OutputLVar.h"

namespace fuzzy_lite {

    OutputLVar::OutputLVar() : LinguisticVariable(),
    _output(new DiscreteTerm("output")){

    }
    OutputLVar::OutputLVar(const std::string& name) : LinguisticVariable(name),
    _output(new DiscreteTerm("output")) {

    }

    OutputLVar::~OutputLVar() {
        delete _output;
    }

    const DiscreteTerm& OutputLVar::getOutput() const {
        return *this->_output;
    }

    flScalar OutputLVar::getDefuzzifiedOutput(const FuzzyOperator& op) const{
        return op.defuzzify(getOutput());
    }

    void OutputLVar::aggregateToOutput(const DiscreteTerm& term){
        _output->aggregate(term);
    }

    void OutputLVar::clearOutput(){
        _output->clear();
    }

}
