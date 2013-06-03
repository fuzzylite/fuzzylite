/* 
 * File:   Linear.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 3:42 PM
 */

#ifndef FL_LINEAR_H
#define	FL_LINEAR_H

#include "fl/term/Term.h"
#include "fl/variable/InputVariable.h"

namespace fl {

    class FL_EXPORT Linear : public Term {
    public:
        std::vector<scalar> coefficients;
        std::vector<const InputVariable*> inputVariables;
        Linear(const std::string& name = "",
                const std::vector<scalar>& coefficients = std::vector<scalar>(),
                const std::vector<InputVariable*>& inputVariables = std::vector<InputVariable*>());
        virtual ~Linear();

        //Warning: this method is unsafe, make sure you use it correctly.
        template <typename T>
        static Linear* create(const std::string& name, const std::vector<InputVariable*>& inputVariables,
                T firstCoefficient, ...) throw (fl::Exception);

        virtual void set(const std::vector<scalar>& coefficients,
                const std::vector<InputVariable*>& inputVariables) throw (fl::Exception);
        virtual void set(const std::vector<scalar>& coefficients,
                const std::vector<const InputVariable*>& inputVariables) throw (fl::Exception);

        virtual scalar membership(scalar x) const;

        virtual std::string className() const;
        virtual std::string toString() const;

        virtual Linear* copy() const;
    };

}

#endif	/* FL_LINEAR_H */

