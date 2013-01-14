cmake_minimum_required(VERSION 2.8)

project(fuzzylite)

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS "-g -Wall -DFL_NO_LOG")
option(USE_SINGLE_PRECISION "Use single precision"	OFF)

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

#add_definitions(-DFL_USE_SINGLE_PRECISION)

add_library(fl-shared SHARED ${fl-headers} ${fl-sources})
set_target_properties(fl-shared PROPERTIES OUTPUT_NAME fuzzylite)

add_library(fl-static STATIC ${fl-headers} ${fl-sources})
set_target_properties(fl-static PROPERTIES OUTPUT_NAME fuzzylite)


add_executable(fl-bin src/main.cpp)
set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite)
set_target_properties(fl-bin PROPERTIES COMPILE_FLAGS -DFL_DEBUG=true)
target_link_libraries(fl-bin fl-shared)


install(TARGETS fl-bin fl-shared fl-static
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION Lib)

install(DIRECTORY fl/ DESTINATION include/fl)


