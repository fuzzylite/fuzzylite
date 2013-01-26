/* 
 * File:   FisExporter.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:52 PM
 */

#ifndef FL_FISEXPORTER_H
#define	FL_FISEXPORTER_H

#include "fl/imex/Exporter.h"

#include <vector>

namespace fl {
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;
    class MamdaniRule;
    class MamdaniProposition;
    class Variable;

    flclass FisExporter : public Exporter {
    protected:
        std::string exportSystem(const Engine* engine) const;
        std::string exportInputs(const Engine* engine) const;
        std::string exportOutputs(const Engine* engine) const;
        std::string exportRules(const Engine* engine) const;
        std::string exportRule(const MamdaniRule* rule, const Engine* engine) const;
        std::string translate(const std::vector<MamdaniProposition*>& propositions,
            const std::vector<Variable*> variables) const;
        
        
        std::string toFis(const TNorm* tnorm) const;
        std::string toFis(const SNorm* snorm) const;
        std::string toFis(const Defuzzifier* defuzzifier) const;
        std::string toFis(const Term* term) const;
    public:
        FisExporter();
        virtual ~FisExporter();

        virtual std::string name() const;
        virtual std::string toString(const Engine* engine) const;
    };

}

#endif	/* FL_FISEXPORTER_H */

