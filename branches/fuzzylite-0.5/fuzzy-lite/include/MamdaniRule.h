/* 
 * File:   MamdaniRule.h
 * Author: jcrada
 *
 * Created on November 5, 2009, 11:15 AM
 */

#ifndef _MAMDANIRULE_H
#define	_MAMDANIRULE_H

#include "FuzzyRule.h"
#include "FuzzyProposition.h"
#include "FuzzyAction.h"

namespace fuzzy_lite {

    class MamdaniRule : public FuzzyRule {
    private:
        FuzzyProposition* _antecedent;
        std::vector<FuzzyAction*> _consequent;
        std::string _unparsed_rule;
    protected:
        virtual void parseAntecedent(const std::string& postfix_antecedent,
                const FuzzyEngine& engine) throw (RuleParsingException);
        virtual void parseConsequent(const std::string& consequent,
                const FuzzyEngine& engine) throw (RuleParsingException);
    public:
        static const std::string FR_IF;
        static const std::string FR_IS;
        static const std::string FR_THEN;
        static const std::string FR_AND;
        static const std::string FR_OR;

        MamdaniRule();
        virtual ~MamdaniRule();

        virtual flScalar evaluate(const FuzzyOperator& fuzzy_op) const;
        virtual bool isValid() const;
        virtual void fire(flScalar degree, const FuzzyOperator& fuzzy_op);

        virtual void parse(const std::string& rule, const FuzzyEngine& engine) throw (RuleParsingException);

        virtual std::string getUnparsedRule() const;

        virtual std::string toString() const;

        static void Extract(const std::string& rule, std::string& antecedent, std::string& consequent);
        static std::string InfixToPostfix(const std::string& infix);
        static bool Validate(const std::string& rule, const FuzzyEngine& engine);

        static void testComplex();
        static void testSimple();

        static void main(int argc, char** argv);

    };
}

#endif	/* _MAMDANIRULE_H */

