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
    class FclExporter {
    public:
        static std::string toFcl(const Engine* engine);
    };

} /* namespace fl */
#endif /* FL_FCLEXPORTER_H_ */
