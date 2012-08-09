/* 
 * File:   defs.h
 * Author: jcrada
 *
 * Created on October 30, 2009, 7:09 PM
 */

#ifndef _DEFS_H
#define	_DEFS_H

#include <iostream>
#include <sstream>
#include <assert.h>

namespace fuzzy_lite {

#define FL_AT __FILE__, __LINE__, __FUNCTION__
 
#ifdef FL_USE_INLINE
#define FL_INLINE inline
#else
#define FL_INLINE
#endif

#ifdef FL_USE_ASSERT
#define FL_ASSERT(assertion) assert(assertion);
#define FL_ASSERTM(value,assertion) if (!(assertion)){FL_LOGM(value);} assert(assertion);
#else
#define FL_ASSERT(assertion)
#define FL_ASSERTM(value,assertion) 
#endif

#ifdef FL_USE_LOG
#define FL_LOG_PREFIX __FILE__ << " [" << __LINE__ << "]:"
#define FL_LOG(message) std::cout << FL_LOG_PREFIX << message << std::endl;
#define FL_LOGW(message) std::cout << FL_LOG_PREFIX << "WARNING: " << message << std::endl;
#else
#define FL_LOG(message) 
#define FL_LOGW(message) 
#endif


#ifdef FL_USE_DEBUG
#define FL_DEBUG_PREFIX __FILE__ << " [" << __LINE__ << "]:"
#define FL_DEBUG(message) std::cout << FL_DEBUG_PREFIX << message << std::endl;

#define FL_DEBUGI static int FL_DEBUG_COUNTER = 0; std::cout << FL_DEBUG_PREFIX << ++FL_DEBUG_COUNTER << std::endl;
#else
#define FL_DEBUG(message)
#define FL_DEBUGI 
#endif
}

#endif	/* _DEFS_H */

