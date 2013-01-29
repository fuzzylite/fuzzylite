cmake_minimum_required(VERSION 2.8)

project(fuzzylite CXX)

option(FL_USE_FLOAT "Use fl::scalar as float" OFF)
option(FL_USE_LOG "Enable the macro FL_LOG to print information" ON)
option(FL_DEBUG "Enable additional debugging information" OFF)

option(FL_PRECISION "Precision to differentiate numbers" 1e-5)
option(FL_DECIMALS "Number of decimals to print information" 3)

if(FL_DEBUG)
#    add_definitions(-DFL_DEBUG=true)
    set(CMAKE_BUILD_TYPE Debug)
endif(FL_DEBUG)

set(CMAKE_VERBOSE_MAKEFILE true)

set(FL_LIBS)

if(UNIX)
    add_definitions(-DFL_UNIX)
    set(CMAKE_CXX_FLAGS "-Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DFL_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
if(NOT APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl --no-undefined") #apple does not recognise these
endif(NOT APPLE)

elseif(WIN32)
    add_definitions(-DFL_WINDOWS)
    set(FL_LIBS dbghelp)
endif(UNIX)


if(FL_USE_FLOAT)
    add_definitions(-DFL_USE_FLOAT)
endif(FL_USE_FLOAT)

if(NOT FL_USE_LOG)
    add_definitions(-DFL_NO_LOG)
endif(NOT FL_USE_LOG)



if(FL_PRECISION)
    add_definitions(-DFL_PRECISION=${FL_PRECISION})
endif(FL_PRECISION)

if(FL_DECIMALS)
    add_definitions(-DFL_DECIMALS=${FL_DECIMALS})
endif(FL_DECIMALS)


include_directories(.)

file(STRINGS FL_HEADERS fl-headers)
file(STRINGS FL_SOURCES fl-sources)

string(REGEX REPLACE "\n" " " ${fl-headers} ${fl-headers})
string(REGEX REPLACE "\n" " " ${fl-sources} ${fl-sources})

# Always full RPATH from http://www.cmake.org/Wiki/CMake_RPATH_handling
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

set(CMAKE_DEBUG_POSTFIX -dbg)

add_library(fl-shared SHARED ${fl-headers} ${fl-sources})
set_target_properties(fl-shared PROPERTIES OUTPUT_NAME fuzzylite)
target_link_libraries(fl-shared ${FL_LIBS})


add_library(fl-static STATIC ${fl-headers} ${fl-sources})
set_target_properties(fl-static PROPERTIES OUTPUT_NAME fuzzylite-static)
target_link_libraries(fl-static ${FL_LIBS})

add_executable(fl-bin src/main.cpp)
set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite-demo)
set_target_properties(fl-bin PROPERTIES DEBUG_POSTFIX -dbg) 
target_link_libraries(fl-bin fl-shared ${FL_LIBS})


install(TARGETS fl-bin fl-shared fl-static
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY fl/ DESTINATION include/fl)


