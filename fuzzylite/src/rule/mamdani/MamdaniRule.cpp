/*
 * MamdaniRule.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "MamdaniRule.h"

#include "MamdaniAntecedent.h"
#include "MamdaniConsequent.h"

#include "../../definitions.h"

#include <regex.h>
#include <vector>

namespace fl {

    MamdaniRule::MamdaniRule()
            : Rule() {
    }

    MamdaniRule::~MamdaniRule() {
    }

    MamdaniRule* MamdaniRule::parse(const std::string& rule, const Engine* engine) {
        MamdaniRule* result = new MamdaniRule();

        std::vector<std::string> tokens;

        regex_t regexCompiled;
        regcomp(&regexCompiled, "(^\\s*if\\s+)(.*)(\\s+then\\s+)(.*)", REG_EXTENDED);

        int numberOfTokens = regexCompiled.re_nsub + 1;
        regmatch_t matches[numberOfTokens];

        if (regexec(&regexCompiled, rule.c_str(), numberOfTokens, matches, 0)
                == 0) {
            for (int i = 0; i < numberOfTokens; ++i) {
                if (matches[i].rm_so == -1) { //FL_LOG("No more tokens");
                    break;
                }
                std::string token = rule.substr(matches[i].rm_so,
                        matches[i].rm_eo - matches[i].rm_so);
                tokens.push_back(token);
            }
        }
        regfree(&regexCompiled);

        if (tokens.size() != 5) {
            FL_LOG("expected five (5) tokens, but found " << tokens.size() << " in rule: " << rule);
        }

        std::string antecedentText = tokens[2];
        std::string consequentText = tokens[4];

        FL_LOG("Antecedent: " << antecedentText);
        FL_LOG("Consequent: " << consequentText);

        MamdaniAntecedent* antecedent = new MamdaniAntecedent;
        antecedent->load(antecedentText, engine);
        result->setAntecedent(antecedent);

        MamdaniConsequent* consequent = new MamdaniConsequent;
        consequent->load(consequentText, engine);
        result->setConsequent(consequent);

        return result;
    }

    void MamdaniRule::main() {
//        std::string rule = "if Service is GOOD then Tip is HIGH";
//
//        regex_t regexCompiled;
//
//        regcomp(&regexCompiled, "(^\\s*if\\s+)(.*)(\\s+then\\s+)(.*)$", REG_EXTENDED);
//
//        int numberOfGroups = regexCompiled.re_nsub + 1;
//        regmatch_t matches[numberOfGroups];
//
//        std::vector<std::string> groups;
//
//        if (regexec(&regexCompiled, rule.c_str(), numberOfGroups, matches, 0) == 0) {
//            for (std::size_t i = 0; i < numberOfGroups; ++i) {
//                if (matches[i].rm_so == (std::size_t) -1) {
//                    FL_LOG("No more numberOfGroups");
//                    break;
//                }
//                std::string group = rule.substr(matches[i].rm_so,
//                        matches[i].rm_eo - matches[i].rm_so);
//                groups.push_back(group);
//                FL_LOG("group " << i << ":" << group);
//            }
//        }
//        FL_LOG(groups[2]);
//        FL_LOG(groups[4]);
//        regfree(&regexCompiled);

        MamdaniRule::parse("if Service is GOOD then Tip is HIGH", NULL);

    }

} /* namespace fl */
