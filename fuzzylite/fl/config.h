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

//class FL_EXPORT is require to build DLLs in Windows.
#ifdef FL_WINDOWS
#define FL_EXPORT __declspec(dllexport)
#else
#define FL_EXPORT 
#endif

#ifdef FL_WINDOWS
#include <ciso646> //alternative operator spellings:
//#define and &&
//#define or ||
//#define not !
//#define bitand &
//#define bitor |

//TODO: add these functions in Infix.cpp
//#define acosh(x)
//#define asinh(x)
//#define atanh(x)
//#define log1p

//C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
//http://msdn.microsoft.com/en-us/library/sa28fef8%28v=vs.80%29.aspx
//to ignore warnings dealing with exceptions in Windows
#pragma warning(disable:4290) 

//Windows NMake complains I should have pointers in all headers instead of 
//stack allocated objects. For example, std::string* instead of std::string.
#pragma warning(disable:4251)
#else 

#endif

#define FL_VERSION "2.0"
#define FL_DATE "02/2013"

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
