/*
 * Discrete.h
 *
 *  Created on: 5/12/2012
 *      Author: jcrada
 */

#ifndef FL_DISCRETE_H_
#define FL_DISCRETE_H_

#include "fl/term/Term.h"

#include <vector>
#include <utility>
namespace fl {

    class Discrete : public Term {
    public:
        std::vector<scalar> x;
        std::vector<scalar> y;

        Discrete(const std::string& name = "");

        Discrete(const std::string& name,
                const std::vector<scalar>& x,
                const std::vector<scalar>& y);

        Discrete(const std::string& name,
                const std::vector<std::pair<scalar, scalar> >& xy);
        
        
        
        ~Discrete();

        std::string className() const;
        Discrete* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;



    };

} 
#endif /* FL_DISCRETE_H_ */
