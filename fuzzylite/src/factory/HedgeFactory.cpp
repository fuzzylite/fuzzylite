/* 
 * File:   HedgeFactory.cpp
 * Author: jcrada
 * 
 * Created on 11 January 2013, 8:51 PM
 */

#include "fl/factory/HedgeFactory.h"

#include "fl/hedge/Any.h"
#include "fl/hedge/Extremely.h"
#include "fl/hedge/Not.h"
#include "fl/hedge/Seldom.h"
#include "fl/hedge/Somewhat.h"
#include "fl/hedge/Very.h"

#include "fl/Exception.h"

namespace fl {

    HedgeFactory::HedgeFactory() { }

    HedgeFactory::~HedgeFactory() { }

    Hedge* HedgeFactory::create(const std::string& name) const {
        if (name == Any().name()) return new Any;
        if (name == Extremely().name()) return new Extremely;
        if (name == Not().name()) return new Not;
        if (name == Seldom().name()) return new Seldom;
        if (name == Somewhat().name()) return new Somewhat;
        if (name == Very().name()) return new Very;
        throw fl::Exception("[factory error] hedge of name <" + name + " not recognized", FL_AT);
    }

    std::vector<std::string> HedgeFactory::available() const {
        std::vector<std::string> result;
        result.push_back(Any().name());
        result.push_back(Extremely().name());
        result.push_back(Not().name());
        result.push_back(Seldom().name());
        result.push_back(Somewhat().name());
        result.push_back(Very().name());
        return result;
    }


}
