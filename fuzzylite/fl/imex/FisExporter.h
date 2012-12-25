/* 
 * File:   FisExporter.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:52 PM
 */

#ifndef FL_FISEXPORTER_H
#define	FL_FISEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;

    class FisExporter : public Exporter {
    protected:
        std::string toFis(const TNorm* tnorm) const;
        std::string toFis(const SNorm* snorm) const;
        std::string toFis(const Defuzzifier* defuzzifier) const;
        std::string toFis(const Term* term) const;
    public:
        FisExporter();
        virtual ~FisExporter();

        virtual std::string name() const;
        virtual std::string toString(const Engine* engine);
    };

}

#endif	/* FL_FISEXPORTER_H */

