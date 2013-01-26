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
    class Engine;
    class InputVariable;
    class OutputVariable;
    class Term;
    class RuleBlock;
    class Operator;
    class Defuzzifier;

    class Hedge;

    flclass CppExporter : public Exporter {
    protected:
        virtual std::string toCpp(const Hedge* hedge) const;
        virtual std::string toCpp(const Term* term) const;
        virtual std::string toCpp(const Operator* op) const;
        virtual std::string toCpp(const Defuzzifier* defuzzifier) const;
    public:
        CppExporter();
        virtual ~CppExporter();
        virtual std::string name() const;
        virtual std::string toString(const Engine* engine) const;

    };
}
#endif	/* FL_CPPEXPORTER_H */

