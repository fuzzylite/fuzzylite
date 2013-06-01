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
    protected:
        std::vector<scalar> _coefficients;
        std::vector<const InputVariable*> _inputVariables;

    public:
        Linear(const std::string& name = "",
                const std::vector<scalar>& coefficients = std::vector<scalar>(),
                const std::vector<InputVariable*>& inputVariables = std::vector<InputVariable*>());
        virtual ~Linear();      
        
        //Warning: this method is unsafe, make sure you use it correctly.
        template <typename T>
        static Linear* create(const std::string& name, const std::vector<InputVariable*>& inputVariables,
                T firstCoefficient, ...) throw (fl::Exception);

        virtual void setInputVariables(const std::vector<InputVariable*>& inputVariables);
        virtual void setInputVariables(const std::vector<const InputVariable*>& inputVariables);
        virtual const std::vector<const InputVariable*>& getInputVariables() const;

        virtual void setCoefficients(const std::vector<scalar>& coefficients);
        virtual const std::vector<scalar>& getCoefficients() const;

        virtual void setNumberOfCoefficients(int coefficients);
        virtual int getNumberOfCoefficients() const;

        virtual void setCoefficient(int index, scalar coefficient);
        virtual scalar getCoefficient(int index) const;

        virtual scalar membership(scalar x) const;

        virtual std::string className() const;
        virtual std::string toString() const;

        virtual Linear* copy() const;
    };
    
}

#endif	/* FL_LINEAR_H */

