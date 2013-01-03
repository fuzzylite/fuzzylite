/* 
 * File:   FisImporter.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:53 PM
 */

#ifndef FL_FISIMPORTER_H
#define	FL_FISIMPORTER_H

#include "fl/imex/Importer.h"

#include "fl/scalar.h"

#include <vector>

namespace fl {
    class Operator;
    class TNorm;
    class SNorm;
    class Term;
    class Defuzzifier;
    class Variable;

    class FisImporter : public Importer {
    protected:

        virtual void importSystem(const std::string& section, Engine* engine,
                std::string& andMethod, std::string& orMethod,
                std::string& impMethod, std::string& aggMethod,
                std::string& defuzzMethod) const;
        virtual void importInput(const std::string& section, Engine* engine) const;
        virtual void importOutput(const std::string& section, Engine* engine) const;
        virtual void importRules(const std::string& section, Engine* engine) const;
        virtual std::string translateProposition(scalar code, Variable* variable) const;

        virtual TNorm* extractTNorm(const std::string& name) const;
        virtual SNorm* extractSNorm(const std::string& name) const;
        virtual Term* extractTerm(const std::string& line) const;
        virtual Term* createInstance(const std::string& termClass, const std::string& name,
                const std::vector<scalar>& params) const;
        virtual Defuzzifier* extractDefuzzifier(const std::string& name) const;
        virtual void extractRange(const std::string& range, scalar& minimum, scalar& maximum) const;

    public:
        FisImporter();
        virtual ~FisImporter();

        virtual std::string name() const;

        virtual Engine* fromString(const std::string& fcl) const;
    };

}
#endif	/* FL_FISIMPORTER_H */

