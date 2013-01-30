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

add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine FL__FILE__

set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

set(FL_LIBS)

if(UNIX)
    add_definitions(-DFL_UNIX)
    set(CMAKE_CXX_FLAGS "-Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DFL_DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
#if(NOT APPLE) #TODO: check again in apple...
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined") #apple does not recognise these
#endif(NOT APPLE)

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

message("${exepath}")


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


# uninstall target
#configure_file(
    #"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    #"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    #IMMEDIATE @ONLY)

#add_custom_target(uninstall
    #COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)
 
#unix uninstall
#xargs rm < install_manifest.txt
