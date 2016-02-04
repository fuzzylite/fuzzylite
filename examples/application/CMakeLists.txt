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



set(FL_SYSTEM_LIBRARY FALSE) #Finds the library installed in the system
set(FL_LIBRARY_DIR)

if (NOT FL_SYSTEM_LIBRARY)
    #it is possible to find the FuzzyLiteLibrary locally as follows,
    #assuming the submodule of fuzzylite is added to the ${PROJECT_SOURCE_DIR}:
    set(FL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/fuzzylite/fuzzylite)
    set(FL_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/fuzzylite/fuzzylite/release/bin)
    message("Finding FuzzyLiteLibrary locally at ${FL_LIBRARY_DIR}")

    include_directories(${FL_INCLUDE_DIR})
endif()

find_library (FuzzyLiteLibrary fuzzylite HINTS ${FL_LIBRARY_DIR}) # we use the shared library or
#find_library (FuzzyLiteLibrary fuzzylite-static) # we use the static library

#we add the definitions for C++11 on this application, 
add_definitions(-std=c++11)
#and for the headers of fuzzylite
add_definitions(-DFL_CPP11=ON)
#we add the definition of the building path to remove it when using macro FL_LOG
add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine

#we add the sources
set(sources src/main.cpp)

#we create the binary
add_executable(binary ${sources})
#linking the fuzzylite library
target_link_libraries (binary ${FuzzyLiteLibrary})
#setting the name of the product
set_target_properties(binary PROPERTIES OUTPUT_NAME FuzzyLiteDemo)
#specially for windows
set_target_properties(binary PROPERTIES OUTPUT_NAME FuzzyLiteDemo IMPORT_PREFIX tmp-) #To prevent LNK1149 in Windows
#in case of building on debug mode
set_target_properties(binary PROPERTIES DEBUG_POSTFIX d)


