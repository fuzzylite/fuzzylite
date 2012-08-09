#include "MamdaniRule.h"

#include "defs.h"
#include "StringOperator.h"
#include "FuzzyOperator.h"
#include "TriangularDTerm.h"

#include <sstream>
#include <stack>
namespace fuzzy_lite {

    const std::string MamdaniRule::FR_IF = "if";
    const std::string MamdaniRule::FR_IS = "is";
    const std::string MamdaniRule::FR_THEN = "then";
    const std::string MamdaniRule::FR_AND = "and";
    const std::string MamdaniRule::FR_OR = "or";

    MamdaniRule::MamdaniRule() : FuzzyRule(),
    _antecedent(NULL) {

    }

    MamdaniRule::~MamdaniRule() {
        if (_antecedent) {
            delete _antecedent;
        }
        FuzzyAction* action = NULL;
        for (int i = _consequent.size() - 1; i >= 0; --i) {
            action = _consequent[i];
            _consequent.pop_back();
            delete action;
        }
    }

    std::string MamdaniRule::getUnparsedRule() const {
        return this->_unparsed_rule;
    }

    flScalar MamdaniRule::evaluate(const FuzzyOperator& fuzzy_op) const {
        if (!_antecedent) {
            throw NullPointerException(FL_AT, "Antedecent expected in rule [" + getUnparsedRule() + "]");
        }
        return _antecedent->degreeOfTruth(fuzzy_op);
    }

    bool MamdaniRule::isValid() const {
        return _antecedent && _consequent.size() > 0;
    }

    void MamdaniRule::fire(flScalar degree, const FuzzyOperator& fuzzy_op) {
        if (!_antecedent) {
            throw NullPointerException(FL_AT, "Antedecent expected in rule [" + getUnparsedRule() + "]");
        }
        if (_consequent.size() <= 0) {
            throw NullPointerException(FL_AT, "At least one consequent expected in rule [" + getUnparsedRule() + "]");
        }
        if (degree > flScalar(0.0)) {
            FL_LOG("Degree: " << degree << " - Fired: " << toString());
            for (size_t i = 0; i < _consequent.size(); ++i) {
                _consequent[i]->execute(degree, fuzzy_op);
            }
        }
    }

    void MamdaniRule::parse(const std::string& rule, const FuzzyEngine& engine) throw (RuleParsingException) {
        this->_unparsed_rule = rule;
        std::string antecedent, consequent;
        Extract(rule, antecedent, consequent);
        std::string postfix = InfixToPostfix(antecedent);
        //        try {
        parseAntecedent(postfix, engine);
        //        } catch (RuleParsingException& e) {
        //            throw RuleParsingException(FL_AT, "Error parsing antecedent of rule: " + getUnparsedRule(), e);
        //        }
        //        try {
        parseConsequent(consequent, engine);
        //        } catch (RuleParsingException& e) {
        //            throw RuleParsingException(FL_AT, "Error parsing consequent of rule: " + getUnparsedRule(), e);
        //        }
    }

    void MamdaniRule::parseAntecedent(const std::string& postfix_antecedent, const FuzzyEngine& engine) throw (RuleParsingException) {

        enum e_state {
            S_LVAR = 1, S_IS, S_HEDGE, S_TERM, S_OPERATOR
        };
        e_state current_state = S_LVAR;
        std::stringstream ss(postfix_antecedent);
        std::string token;
        InputLVar* input = NULL;
        std::vector<Hedge*> hedges;
        Hedge* hedge = NULL;
        DiscreteTerm* term = NULL;
        std::stack<FuzzyProposition*> propositions;
        FuzzyProposition* proposition;
        while (ss >> token) {
            switch (current_state) {
                case S_LVAR:
                case S_OPERATOR:
                    if (token == FR_AND || token == FR_OR) {
                        if (!_antecedent) {
                            _antecedent = propositions.top();
                            propositions.pop();
                        }
                        proposition = _antecedent;
                        _antecedent = new FuzzyProposition();
                        _antecedent->setOperation(token == FR_AND ? FuzzyProposition::O_AND :
                                FuzzyProposition::O_OR);
                        _antecedent->setLeft(*proposition);
                        proposition = propositions.top();
                        propositions.pop();
                        _antecedent->setRight(*proposition);
                        current_state = S_OPERATOR;
                        break;
                    }
                    if (current_state == S_OPERATOR) {
                        throw RuleParsingException(FL_AT,
                                "Operator expected but found <" + token +
                                ">. Rule: " + getUnparsedRule());
                    }
                    //If it is not an operator
                    input = engine.getInputLVar(token);
                    if (!input) {
                        throw RuleParsingException(FL_AT, "Input variable <" +
                                token + "> not registered in fuzzy engine");
                    }
                    current_state = S_IS;
                    break;
                case S_IS:
                    if (token == FR_IS) {
                        current_state = S_HEDGE;
                    } else {
                        throw RuleParsingException(FL_AT, "<is> expected but found <" +
                                token + ">");
                    }
                    break;
                case S_HEDGE:
                    hedge = engine.getHedge(token);
                    if (hedge) {
                        hedges.push_back(hedge);
                        break;
                    }
                    //intentional fall-through
                case S_TERM:
                    term = input->getTerm(token);
                    if (term) {
                        current_state = S_LVAR;
                        proposition = new FuzzyProposition();
                        proposition->setInputLVar(*input);
                        proposition->setTerm(*term);
                        for (size_t i = 0; i < hedges.size(); ++i) {
                            proposition->addHedge(*hedges[i]);
                        }
                        propositions.push(proposition);
                        proposition = NULL;
                        hedges.clear();
                    } else {
                        throw RuleParsingException(FL_AT, "Term <" + token +
                                "> not found in input variable <" + input->getName() +
                                ">");
                    }
                    break;
            }
        }
        if (!propositions.empty()) { //Type of Rule (one proposition in antecedent: if A is x then C is y
            if (_antecedent) {
                throw RuleParsingException(FL_AT, "Unexpected error");
            }
            _antecedent = propositions.top();
            propositions.pop();
        }

    }

    void MamdaniRule::parseConsequent(const std::string& consequent, const FuzzyEngine& engine) throw (RuleParsingException) {
        std::stringstream ss(consequent);
        std::string token;

        enum e_state {
            S_LVAR = 1, S_IS, S_TERM, S_AND
        };
        e_state state = S_LVAR;

        OutputLVar* output = NULL;
        DiscreteTerm* term = NULL;
        FuzzyAction* action = NULL;
        while (ss >> token) {
            switch (state) {
                case S_LVAR:
                    output = engine.getOutputLVar(token);
                    if (!output) {
                        throw RuleParsingException(FL_AT, "Output variable <" +
                                token + "> not registered in fuzzy engine");
                    }
                    state = S_IS;
                    break;
                case S_IS:
                    if (token == FR_IS) {
                        state = S_TERM;
                    } else {
                        throw RuleParsingException(FL_AT, "<is> expected but found <" +
                                token + ">");
                    }
                    break;
                case S_TERM:
                    term = output->getTerm(token);
                    if (!term) {
                        throw RuleParsingException(FL_AT, "Term <" + token +
                                "> not found in input variable <" + output->getName() + ">");
                    }
                    action = new FuzzyAction();
                    action->setOutputLVar(output);
                    action->setTerm(*term);
                    action->setProbability(1.0);
                    _consequent.push_back(action);
                    state = S_AND;
                    break;
                case S_AND:
                    if (token != FR_AND) {
                        throw RuleParsingException(FL_AT, "<and> expected but found <" +
                                token + ">");
                    }
                    state = S_LVAR;
                    break;
            }
        }
    }

    std::string MamdaniRule::toString() const {
        std::stringstream ss;
        ss << FR_IF << " " << _antecedent->toString() << " " << FR_THEN << " ";
        for (size_t i = 0; i < _consequent.size(); ++i) {
            ss << _consequent[i]->toString() + (i < _consequent.size() - 1 ? FR_AND : "") + " ";
        }
        return ss.str();
    }

    bool MamdaniRule::Validate(const std::string& rule, const FuzzyEngine& engine) {
        std::stringstream ss(rule);
        std::string token;
        int parenthesis = 0;
        // LinguisticVariable is <Hedge> Term operator

        enum e_state {
            S_IF = 1, S_LVAR, S_IS, S_HEDGE, S_TERM, S_OPERATOR
        };
        e_state current_state = S_LVAR;
        e_state previous_state;
        const LinguisticVariable* lvar = NULL;
        while (ss >> token) {
            if (token == "(") {
                ++parenthesis;
                continue;
            } else if (token == ")") {
                --parenthesis;
                continue;
            }
            previous_state = current_state;
            switch (current_state) {
                case S_IF:
                    if (token == FR_IF) {
                        current_state = S_LVAR;
                    } else {
                        FL_LOG("Error: <if> expected but found " + token);
                    }
                case S_LVAR:
                    if ((lvar = engine.getInputLVar(token)) || (lvar = engine.getOutputLVar(token))) {
                        current_state = S_IS;
                    } else {
                        FL_LOG("Error: <Linguistic Variable> expected but found " + token);
                    }
                    break;
                case S_IS:
                    if (token == FR_IS) {
                        current_state = S_HEDGE;
                    } else {
                        FL_LOG("Error: <is> expected but found " + token);
                    }
                    break;
                case S_HEDGE:
                    current_state = S_TERM;
                    break;
                case S_TERM:
                    if (lvar != NULL && lvar->getTerm(token)) {
                        current_state = S_OPERATOR;
                    } else {
                        FL_LOG("Error: <Term> expected but found " + token);
                    }
                    break;
                case S_OPERATOR:
                    if (token == FR_AND || token == FR_OR || token == FR_THEN) {
                        current_state = S_LVAR;
                    } else {
                        FL_LOG("Error: <Operator> expected but found " + token);
                    }
            }
            if (previous_state == current_state) {
                return false;
            }
            if (parenthesis != 0) {
                FL_LOG("Error: Parenthesis do not match ");
                return false;
            }
        }
        return true;
    }

    void MamdaniRule::Extract(const std::string& rule, std::string& antecedent, std::string& consequent) {
        //Version 1
        //        int then_index = rule.find(FR_THEN) + FR_THEN.size();
        //        FL_ASSERT(then_index > 0 && then_index < rule.size());
        //        consequent = rule.substr(then_index);
        //        int if_index = rule.find(FR_IF) + FR_IF.size();
        //        FL_ASSERT(if_index >= 0 && if_index < rule.size());
        //        antecedent = rule.substr(if_index, then_index - FR_THEN.size() - FR_IF.size() - 1);
        //Version 2
        std::string token;
        std::stringstream ss(rule);
        bool _if_clause = true;
        while (ss >> token) {
            if (token == FR_THEN) {
                _if_clause = false;
            }
            if (_if_clause) {
                if (token != FR_IF) {
                    antecedent.append(token + " ");
                }
            } else {
                if (token != FR_THEN) {
                    consequent.append(token + " ");
                }
            }
        }

    }

    std::string MamdaniRule::InfixToPostfix(const std::string&infix) {
        std::stringstream ss(infix);
        std::string token;
        std::string tmp;
        std::string result;
        std::stack<std::string> stack;

        while (ss >> token) {
            if (token == "(") {
                stack.push(token);
            } else if (token == ")") {
                FL_ASSERT(!stack.empty());
                tmp = stack.top();
                stack.pop();
                while (tmp != "(") {
                    result += tmp + " ";
                    FL_ASSERT(!stack.empty());
                    tmp = stack.top();
                    stack.pop();
                }
            } else if (token == FR_AND || token == FR_OR) {
                if (stack.empty()) {
                    stack.push(token);
                } else {
                    FL_ASSERT(!stack.empty())
                    tmp = stack.top();
                    stack.pop();
                    while (tmp != FR_AND && tmp != FR_OR && tmp != "(") {
                        result += tmp + " ";
                        FL_ASSERT(!stack.empty())
                        tmp = stack.top();
                        stack.pop();
                    }
                    stack.push(tmp);
                    stack.push(token);
                }
            } else {
                result += token + " ";
            }
        }

        while (!stack.empty()) {
            FL_ASSERT(!stack.empty())
            token = stack.top();
            stack.pop();
            result += token + " ";
        }
        return result;
    }

    void MamdaniRule::main(int argc, char** argv) {


    }
}
