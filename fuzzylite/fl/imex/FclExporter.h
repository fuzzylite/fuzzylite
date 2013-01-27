/*
 * FclExporter.h
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#ifndef FL_FCLEXPORTER_H
#define FL_FCLEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {
    class TNorm;
    class SNorm;
    class Defuzzifier;
    class Term;

    class FL_EXPORT FclExporter : public Exporter {
    protected:
        virtual std::string toFcl(const TNorm* tnorm) const;
        virtual std::string toFcl(const SNorm* snorm) const;
        virtual std::string toFcl(const Defuzzifier* defuzzifier) const;
        virtual std::string toFcl(const Term* term) const;

    public:
        FclExporter();
        virtual ~FclExporter();

        virtual std::string name() const;
        virtual std::string toString(const Engine* engine) const;
    };

}
#endif /* FL_FCLEXPORTER_H */
