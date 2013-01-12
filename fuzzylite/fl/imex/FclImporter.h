/*
 * FclImporter.h
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#ifndef FL_FCLIMPORTER_H_
#define FL_FCLIMPORTER_H_

#include "fl/imex/Importer.h"

#include "fl/scalar.h"

#include <string>
#include <vector>

namespace fl {
    class Operator;
    class TNorm;
    class SNorm;
    class Term;
    class Defuzzifier;

    class FclImporter : public Importer {
    protected:
        virtual void processBlock(const std::string& tag, const std::string& block, Engine* engine) const;
        virtual void processVar(const std::string& var, const std::string& block, Engine* engine)const;
        virtual void processFuzzify(const std::string& block, Engine* engine)const;
        virtual void processDefuzzify(const std::string& block, Engine* engine)const;
        virtual void processRuleBlock(const std::string& block, Engine* engine)const;

        virtual TNorm* extractTNorm(const std::string& line) const;
        virtual SNorm* extractSNorm(const std::string& line) const;

        virtual Term* extractTerm(const std::string& line) const;
        //        virtual Term* createInstance(const std::string& termClass, const std::string& name,
        //                const std::vector<scalar>& params) const;
        virtual Defuzzifier* extractDefuzzifier(const std::string& line) const;
        virtual scalar extractDefaultValue(const std::string& line, bool& lockDefuzzifiedValue) const;
        virtual void extractRange(const std::string& line, scalar& minimum, scalar& maximum) const;

    public:
        FclImporter();
        virtual ~FclImporter();

        virtual std::string name() const;

        virtual Engine* fromString(const std::string& fcl) const;

    };

}
#endif /* FL_FCLIMPORTER_H_ */
