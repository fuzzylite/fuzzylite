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

    class FisImporter : public Importer {
    protected:

        virtual void loadSystem(const std::string& section, Engine* engine) const;
        virtual void loadInput(const std::string& section, Engine* engine) const;
        virtual void loadOutput(const std::string& section, Engine* engine) const;
        virtual void loadRules(const std::string& section, Engine* engine) const;
        
        
        virtual TNorm* extractTNorm(const std::string& name) const;
        virtual SNorm* extractSNorm(const std::string& name) const;
        virtual Term* extractTerm(const std::string& line) const;
        virtual Term* createInstance(const std::string& termClass, const std::string& name,
                const std::vector<scalar>& params) const;
        virtual Defuzzifier* extractDefuzzifier(const std::string& name) const;
        virtual scalar extractDefaultValue(const std::string& name, bool& lockDefuzzifiedValue) const;
        virtual void extractRange(const std::string& range, scalar& minimum, scalar& maximum) const;

    public:
        FisImporter();
        virtual ~FisImporter();

        virtual std::string name() const;

        virtual Engine* fromString(const std::string& fcl) const;
    };
    
}
#endif	/* FL_FISIMPORTER_H */

