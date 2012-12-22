/*
 * FclExporter.h
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#ifndef FL_FCLEXPORTER_H_
#define FL_FCLEXPORTER_H_

#include <string>

namespace fl {
    class Engine;
    class TNorm;
    class SNorm;

    class FclExporter {
    protected:
        virtual std::string toFcl(const TNorm* tnorm) const;
        virtual std::string toFcl(const SNorm* snorm) const;

    public:
        FclExporter();
        virtual ~FclExporter();
        virtual std::string toFcl(const Engine* engine);
    };

}
#endif /* FL_FCLEXPORTER_H_ */
