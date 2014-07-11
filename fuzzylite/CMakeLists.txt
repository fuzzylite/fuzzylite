cmake_minimum_required(VERSION 2.8) 

#in windows, cmake . -G "NMake Makefiles"
#in unix, cmake . -G "Unix Makefiles"

#After building:
# In Linux:
# In Mac:
# otool -L fuzzylite #gives the dependencies of fuzzylite
# install_name_tool -change /Users/jcrada/Development/fuzzylite/bin/libfuzzylite.dylib @executable_path/libfuzzylite.dylib fuzzylite

project(fuzzylite CXX)

set(CMAKE_VERBOSE_MAKEFILE false)


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


set(FL_VERSION 5.0)
add_definitions(-DFL_VERSION="${FL_VERSION}")

set(FL_DATE "1407")
add_definitions(-DFL_DATE="${FL_DATE}")

option(FL_USE_FLOAT "Use fl::scalar as float" OFF)
option(FL_DEBUG "Enable additional debugging information" OFF)
option(FL_BACKTRACE "Provide backtrace information in case of errors" ON)


if(FL_DEBUG)
    add_definitions(-DFL_DEBUG=true)
    set(CMAKE_BUILD_TYPE Debug)
    set(FL_CONF d)
else()
    set(CMAKE_BUILD_TYPE Release)
    set(FL_CONF)
endif(FL_DEBUG)



add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine FL__FILE__

#set(EXECUTABLE_OUTPUT_PATH bin)
#set(LIBRARY_OUTPUT_PATH lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(FL_LIBS)


if(UNIX)
    set(CMAKE_CXX_FLAGS "-pedantic -Werror -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    #set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,bin/") #does apple recognise these?

if(NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined") #To avoid undefined methods in library
endif()

elseif(WIN32)
    set(CMAKE_CXX_FLAGS "/WX /W4")
	#Wx: Treat warnings as errors. W4: All warnings
	#http://msdn.microsoft.com/en-us/library/thxezb7y.aspx
	#EHsc: call destructors on __try __catch, and to ignore C4530: C++ exception handler used. Note, unwind semantics are not enabled
    set(FL_LIBS dbghelp)
endif()


if(FL_USE_FLOAT)
    add_definitions(-DFL_USE_FLOAT)
endif(FL_USE_FLOAT)

if(NOT FL_BACKTRACE)
    add_definitions(-DFL_BACKTRACE_OFF)
endif()

include_directories(.)
cmake_policy(SET CMP0015 OLD)
link_directories(.)

file(STRINGS FL_HEADERS fl-headers)
file(STRINGS FL_SOURCES fl-sources)

string(REGEX REPLACE "\n" " " ${fl-headers} ${fl-headers})
string(REGEX REPLACE "\n" " " ${fl-sources} ${fl-sources})

message("${exepath}")


set(CMAKE_DEBUG_POSTFIX d)

add_library(fl-shared SHARED ${fl-headers} ${fl-sources})
set_target_properties(fl-shared PROPERTIES OUTPUT_NAME fuzzylite)
target_link_libraries(fl-shared ${FL_LIBS})

add_library(fl-static STATIC ${fl-headers} ${fl-sources})
set_target_properties(fl-static PROPERTIES OUTPUT_NAME fuzzylite-static)
target_link_libraries(fl-static ${FL_LIBS})

add_executable(fl-bin fl/Console.h src/Console.cpp src/main.cpp)
set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite)
set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite IMPORT_PREFIX _) #To prevent LNK1149 in Windows
#set_target_properties(fl-bin PROPERTIES DEBUG_POSTFIX -dbg)
target_link_libraries(fl-bin fl-static ${FL_LIBS})


install(TARGETS fl-bin fl-shared fl-static
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY fl/ DESTINATION include/fl)

#if(WIN32)
#   set(fl-suffix)
#   add_custom_command(TARGET fl-bin POST_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_SOURCE_DIR}/lib/fuzzylite${FL_CONF}.dll"
#        ${CMAKE_SOURCE_DIR}/bin)                 # <--this is out-file path
#endif()

message("=====================================")
message("fuzzylite v.${FL_VERSION}b${FL_DATE}\n")
message("FL_DEBUG=${FL_DEBUG}")
message("FL_USE_FLOAT=${FL_USE_FLOAT}")
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
