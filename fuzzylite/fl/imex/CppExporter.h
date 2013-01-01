/* 
 * File:   CppExporter.h
 * Author: jcrada
 *
 * Created on 1 January 2013, 2:41 PM
 */

#ifndef FL_CPPEXPORTER_H
#define	FL_CPPEXPORTER_H

#include "fl/imex/Exporter.h"

namespace fl {

    class CppExporter : public Exporter{
    protected:
        std::string toString(const Hedge* hedge) const;
    public:
        std::string name() const;
        std::string toString(const Engine* engine) const;
        
        std::string toString(const InputVariable* input) const;
        std::string toString(const OutputVariable* output) const;
        std::string toString(const RuleBlock* ruleblock) const;
    };
}
#endif	/* FL_CPPEXPORTER_H */

