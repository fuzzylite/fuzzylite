/*
 * FclImporter.h
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#ifndef FL_FCLIMPORTER_H_
#define FL_FCLIMPORTER_H_

#include "../scalar.h"
#include <string>
#include <vector>

namespace fl {
    class Engine;
    class Operator;
    class Term;
    class Defuzzifier;

    class FclImporter {
    protected:
        Engine* _engine;

        void processBlock(const std::string& tag, const std::string& block);
        void processVar(const std::string& var, const std::string& block);
        void processFuzzify(const std::string& block);
        void processDefuzzify(const std::string& block);
        void processRuleBlock(const std::string& block);

        Operator* extractOperator(const std::string& line) const;
        Term* extractTerm(const std::string& line) const;
        Term* createInstance(const std::string& termClass, const std::string& name,
                const std::vector<scalar>& params) const;
        Defuzzifier* extractDefuzzifier(const std::string& line) const;
        scalar extractDefaultValue(const std::string& line, bool& lockDefuzzifiedValue) const;

    public:
        Engine* fromFcl(const std::string& fcl);

        static void main();
    };

} /* namespace fl */
#endif /* FL_FCLIMPORTER_H_ */
