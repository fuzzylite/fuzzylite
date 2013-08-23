cmake_minimum_required(VERSION 2.8)

#in windows, cmake . -G "NMake Makefiles"

project(fuzzylite CXX)

set(CMAKE_VERBOSE_MAKEFILE true)


if(WIN32)
    message("Windows")
    add_definitions(-DFL_WINDOWS)
endif()

if (UNIX)
    message("Unix")
    add_definitions(-DFL_UNIX)
endif()

if (APPLE)
    message("Apple")
    add_definitions(-DFL_APPLE)
endif()


set(FL_VERSION 3.1)
add_definitions(-DFL_VERSION="${FL_VERSION}")

set(FL_DATE "1307")
add_definitions(-DFL_DATE="1308")

option(FL_USE_FLOAT "Use fl::scalar as float" OFF)
option(FL_USE_LOG "Enable the macro FL_LOG to print information" ON)
option(FL_DEBUG "Enable additional debugging information" OFF)

option(FL_PRECISION "Precision to differentiate numbers" 1e-5)
option(FL_DECIMALS "Number of decimals to print information" 3)
option(FL_BACKTRACE "Provide backtrace information in case of errors" ON)


if(FL_DEBUG)
    add_definitions(-DFL_DEBUG=true)
    set(CMAKE_BUILD_TYPE Debug)
    set(FL_CONF -dbg)
else()
    set(CMAKE_BUILD_TYPE Release)
    set(FL_CONF)
endif(FL_DEBUG)



add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine FL__FILE__

set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

set(FL_LIBS)

if(UNIX)
    set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
if(NOT APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined") #apple does not recognise these
endif()

elseif(WIN32)
    #http://msdn.microsoft.com/en-us/library/thxezb7y.aspx
    set(CMAKE_CXX_FLAGS " /WX /W4 /EHsc")
    set(FL_LIBS dbghelp)
endif()


if(FL_USE_FLOAT)
    add_definitions(-DFL_USE_FLOAT)
endif(FL_USE_FLOAT)

if(NOT FL_USE_LOG)
    add_definitions(-DFL_NO_LOG)
endif(NOT FL_USE_LOG)


if(FL_PRECISION)
    add_definitions(-DFL_PRECISION=${FL_PRECISION})
else()
    set(FL_PRECISION 1e-5)
endif(FL_PRECISION)

if(FL_DECIMALS)
    add_definitions(-DFL_DECIMALS=${FL_DECIMALS})
else()
    set(FL_DECIMALS 3)
endif(FL_DECIMALS)

if(NOT FL_BACKTRACE)
    add_definitions(-DFL_BACKTRACE_OFF)
endif()

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
set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite-bin)
set_target_properties(fl-bin PROPERTIES DEBUG_POSTFIX -dbg)
target_link_libraries(fl-bin fl-shared ${FL_LIBS})


install(TARGETS fl-bin fl-shared fl-static
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		)

install(DIRECTORY fl/ DESTINATION include/fl)

if(WIN32)
	set(fl-suffix)
	
	add_custom_command(TARGET fl-bin POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_SOURCE_DIR}/lib/fuzzylite${FL_CONF}.dll"
        ${CMAKE_SOURCE_DIR}/bin)                 # <--this is out-file path
endif()

message("=====================================")
message("fuzzylite v.${FL_VERSION}b${FL_DATE}\n")
message("FL_DEBUG=${FL_DEBUG}")
message("FL_USE_LOG=${FL_USE_LOG}")
message("FL_USE_FLOAT=${FL_USE_FLOAT}")
message("FL_PRECISION=${FL_PRECISION}")
message("FL_DECIMALS=${FL_DECIMALS}")
message("FL_BACKTRACE=${FL_BACKTRACE}")
message("=====================================\n")

# uninstall target
#configure_file(
    #"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    #"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    #IMMEDIATE @ONLY)

#add_custom_target(uninstall
    #COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)

#unix uninstall
#xargs rm < install_manifest.txt
