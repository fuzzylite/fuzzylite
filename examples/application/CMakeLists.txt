cmake_minimum_required(VERSION 2.8.8)

project(FuzzyLiteDemo CXX)

#Some default policies and variables
if (APPLE)
	cmake_policy(SET CMP0042 NEW)
endif()
if (MSVC)
	cmake_policy(SET CMP0054 NEW)
endif()

if(NOT CMAKE_VERBOSE_MAKEFILE)
    set(CMAKE_VERBOSE_MAKEFILE false)
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel." FORCE )
endif()

option(FL_BACKTRACE "Provide backtrace information in case of errors" ON)
option(FL_STATIC "Statically link to fuzzylite libraries" ON)

set(BacktraceLibrary)

if (MSVC AND FL_BACKTRACE)
    set(BacktraceLibrary dbghelp)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(FL_DEBUG ON)
else()
    set(FL_DEBUG OFF)
endif()


set(FL_SYSTEM_LIBRARY FALSE) #Finds the library installed in the system
set(FL_LIBRARY_DIR)

if (NOT FL_SYSTEM_LIBRARY)
    #it is possible to find the FuzzyLiteLibrary locally as follows,
    #assuming the submodule of fuzzylite is added to the ${PROJECT_SOURCE_DIR}:
    set(FL_HOME ${PROJECT_SOURCE_DIR}/../../fuzzylite/)
    set(FL_INCLUDE_DIR ${FL_HOME})
    if (FL_DEBUG)
        set(FL_LIBRARY_DIR ${FL_HOME}/debug/bin)
    else()
        set(FL_LIBRARY_DIR ${FL_HOME}/release/bin)
    endif()
    message("Finding FuzzyLiteLibrary locally at ${FL_LIBRARY_DIR}")

    include_directories(${FL_INCLUDE_DIR})
endif()

set(FL_POSTFIX)
if (FL_STATIC)
    set(FL_POSTFIX "-static")
endif()
if (FL_DEBUG)
    set(FL_POSTFIX "${FL_POSTFIX}-debug")
endif()

set(FL_LIBRARY_NAME  fuzzylite${FL_POSTFIX})
find_library (FuzzyLiteLibrary ${FL_LIBRARY_NAME} HINTS ${FL_LIBRARY_DIR})

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19)
#C++11 not available before Visual Studio 2015
    if (NOT FL_CPP98)
        set(FL_CPP98 ON)
    endif()
endif()


#if building using C++98
if(FL_CPP98)
    add_definitions(-DFL_CPP98)
    if(NOT MSVC)
        #Set C++98 by default in Clang and others
        add_definitions(-std=c++98)
    endif()
else()
    if(NOT MSVC)
        #Set C++11 by default in Clang and others
        add_definitions(-std=c++11)
    endif()
endif(FL_CPP98)

#we add the definition of the building path to remove it when using macro FL_LOG
add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine
#we add the sources
set(sources src/main.cpp)



if(MSVC)
#Set compilation flags in Windows
    set(CMAKE_CXX_FLAGS "/W4 /EHsc")
    #Wx: Treat warnings as errors. W4: All warnings
    #http://msdn.microsoft.com/en-us/library/thxezb7y.aspx
    #EHsc: call destructors on __try __catch, and to ignore C4530: C++ exception handler used. Note, unwind semantics are not enabled
endif()
 
	
#we create the binary
add_executable(binary ${sources})
if (NOT FL_STATIC)
    target_compile_definitions(binary PRIVATE FL_IMPORT_LIBRARY)
endif()
#linking the fuzzylite library
target_link_libraries (binary ${FuzzyLiteLibrary} ${BacktraceLibrary})
#setting the name of the product
set_target_properties(binary PROPERTIES OUTPUT_NAME FuzzyLiteDemo)
#specially for windows
set_target_properties(binary PROPERTIES OUTPUT_NAME FuzzyLiteDemo IMPORT_PREFIX tmp-) #To prevent LNK1149 in Windows
#in case of building on debug mode
set_target_properties(binary PROPERTIES DEBUG_POSTFIX d)



message("=====================================")
message("FuzzyLite Demo v6.0\n")
message("FL_HOME=${FL_HOME}")
message("FL_LIBRARY_NAME=${FL_LIBRARY_NAME}")
message("FuzzyLiteLibrary=${FuzzyLiteLibrary}")
message("")
message("FL_BACKTRACE=${FL_BACKTRACE}")
message("FL_STATIC=${FL_STATIC}")
message("FL_DEBUG=${FL_DEBUG}")
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message("COMPILE_DEFINITIONS:")
get_directory_property(fl-definitions DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
foreach(d ${fl-definitions})
    message( STATUS "Defined: " ${d} )
endforeach()
message("=====================================\n")

