/*
 * config.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_CONFIG_H
#define FL_CONFIG_H

#include <iostream>
#include <sstream>


#define FL_VERSION "2.0"
#define FL_DATE "01/2013"

#define FL_AT __FILE__, __LINE__, __FUNCTION__

#ifndef FL_DECIMALS
#define FL_DECIMALS 3 //for formatting strings
#endif

#ifndef FL_DIVISIONS
#define FL_DIVISIONS 500 //for defuzzifiers
#endif

#ifndef FL_PRECISION
#define FL_PRECISION 1e-5
#endif

#define FL_LOG_PREFIX __FILE__ << " [" << __LINE__ << "]:"

#ifdef FL_NO_LOG
#define FL_LOG(message)
#define FL_LOGP(message)
#else
#define FL_LOG(message) std::cout << FL_LOG_PREFIX << message << std::endl
#define FL_LOGP(message) std::cout << message << std::endl
#endif

#ifndef FL_DEBUG
#define FL_DEBUG false
#endif

#define FL_BEGIN_DEBUG_BLOCK if (FL_DEBUG){
#define FL_END_DEBUG_BLOCK }


#define FL_DBG(message) FL_BEGIN_DEBUG_BLOCK \
        std::cout << FL_LOG_PREFIX << message << std::endl;\
        FL_END_DEBUG_BLOCK


#endif /* FL_DEFINITIONS_H */
