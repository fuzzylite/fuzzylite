cmake_minimum_required(VERSION 3.13)

cmake_policy(SET CMP0048 NEW)
set(FL_VERSION 7.0.0)
project(fuzzylite VERSION ${FL_VERSION} LANGUAGES CXX)


if (APPLE)
    #    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

    # MACOSX_RPATH is enabled by default.
    cmake_policy(SET CMP0042 NEW)
endif ()

if (MSVC)
    # Only interpret if() arguments as variables or keywords when unquoted.
    cmake_policy(SET CMP0054 NEW)
endif ()


###DEFINES SECTION
if (NOT CMAKE_VERBOSE_MAKEFILE)
    set(CMAKE_VERBOSE_MAKEFILE false)
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

add_definitions(-DFL_BUILD_PATH="${PROJECT_SOURCE_DIR}") #used to determine FL__FILE__

option(FL_BUILD_SHARED "Build shared library" ON)
option(FL_BUILD_STATIC "Build static library" ON)
if (FL_BUILD_SHARED)
    option(FL_BUILD_BINARY "Build fuzzylite binary" ON)
endif ()

option(FL_CPP98 "Builds utilizing C++98, i.e., passing -std=c++98" OFF)
option(FL_USE_FLOAT "Use fl::scalar as float" OFF)
option(FL_BACKTRACE "Provide backtrace information in case of errors" ON)

option(FL_BUILD_TESTS "Builds the unit tests" ON)
option(FL_BUILD_COVERAGE "Build the unit tests to compute coverage" OFF)
option(FL_WARNINGS_AS_ERRORS "Configure warnings as errors" OFF)

if (FL_USE_FLOAT)
    add_definitions(-DFL_USE_FLOAT)
endif (FL_USE_FLOAT)

if (FL_BACKTRACE)
    add_definitions(-DFL_BACKTRACE)
endif ()

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19)
    #C++11 not available before Visual Studio 2015
    if (NOT FL_CPP98)
        set(FL_CPP98 ON)
    endif ()
endif ()

if (FL_CPP98)
    add_definitions(-DFL_CPP98)
    if (NOT MSVC)
        #Set C++98 by default in Clang and others
        add_definitions(-std=c++98)
    endif ()
else ()
    if (NOT MSVC)
        #Set C++11 by default in Clang and others
        add_definitions(-std=c++11)
    endif ()
endif (FL_CPP98)

#Put all binaries in same location
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

if (NOT MSVC)

    #Add Unix compilation flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra") # -Wconversion -Wsign-conversion
    if (FL_WARNINGS_AS_ERRORS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    endif ()

    #    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    #    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

    if (NOT APPLE)
        #To avoid undefined methods in library
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    endif ()
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
    #Address fl::null errors of literal null conversion
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-non-literal-null-conversion")
    #TEMPORAL: flag removal to avoid warnings of `float` variadic in Operation::join
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-varargs")
endif ()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 6)
        #In GNU gcc v6, the default is C++11
        if (FL_CPP98)
            #set the default to C++98
            #Fix error: 'template<class> class std::auto_ptr' is deprecated with gcc-6
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++98")
        endif ()
    endif ()
    #In GNU gcc 4.7, Op::str(T, std::size_t(0)) raises a warning of type-limits
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits")
    endif ()
endif ()


set(FL_LIBS)

if (MSVC)
    #Set compilation flags in Windows
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
    if (FL_WARNINGS_AS_ERRORS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Werror")
    endif ()
    #Wx: Treat warnings as errors. W4: All warnings
    #http://msdn.microsoft.com/en-us/library/thxezb7y.aspx
    #EHsc: call destructors on __try __catch, and to ignore C4530: C++ exception handler used. Note, unwind semantics are not enabled
    #Add Backtrace library
    if (FL_BACKTRACE)
        set(FL_LIBS dbghelp)
    endif ()
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD AND FL_BACKTRACE)
    set(FL_LIBS execinfo)
endif ()

#if(APPLE)
#    # This produces a warning: ld: symbol(s) not found for architecture x86_64 on mac
#    set(FL_LIBS stdc++)
#endif()


###BUILD SECTION

file(STRINGS FL_HEADERS fl-headers)
file(STRINGS FL_SOURCES fl-sources)
file(STRINGS FL_TESTS fl-tests)

string(REGEX REPLACE "\n" " " ${fl-headers} ${fl-headers})
string(REGEX REPLACE "\n" " " ${fl-sources} ${fl-sources})
string(REGEX REPLACE "\n" " " ${fl-tests} ${fl-tests})

list(APPEND fl-targets "")

message("${exepath}")


set(CMAKE_DEBUG_POSTFIX debug)

if (MSVC OR CMAKE_GENERATOR STREQUAL Xcode)
    if (FL_BUILD_SHARED)
        add_library(fl-shared SHARED ${fl-headers} ${fl-sources})
    endif ()

    if (FL_BUILD_STATIC)
        add_library(fl-static STATIC ${fl-headers} ${fl-sources})
    endif ()
else ()
    if (FL_BUILD_SHARED OR FL_BUILD_STATIC)
        add_library(fl-obj OBJECT ${fl-headers} ${fl-sources})
        # The objects need the fuzzylite headers to compile, but
        # these should only be visile to fl-obj at this time, so
        # they are marked as PRIVATE.
        target_include_directories(fl-obj PRIVATE ${PROJECT_SOURCE_DIR})

        if (NOT MINGW)
            set_target_properties(fl-obj PROPERTIES COMPILE_FLAGS "-fPIC")
        endif ()
    endif ()

    if (FL_BUILD_SHARED)
        add_library(fl-shared SHARED $<TARGET_OBJECTS:fl-obj>)
    endif (FL_BUILD_SHARED)

    if (FL_BUILD_STATIC)
        add_library(fl-static STATIC $<TARGET_OBJECTS:fl-obj>)
    endif (FL_BUILD_STATIC)
endif ()

if (FL_BUILD_SHARED)
    # The public include directories to be shared with projects that
    # link to fuzzylite depend on whether they're being included from
    # source (BUILD_INTERFACE), or from /usr/include (INSTALL_INTERFACE)
    target_include_directories(fl-shared
            PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>)
    set_target_properties(fl-shared PROPERTIES OUTPUT_NAME fuzzylite)
    set_target_properties(fl-shared PROPERTIES DEBUG_POSTFIX -debug)
    target_compile_definitions(fl-shared PRIVATE FL_EXPORT_LIBRARY)
    set_target_properties(fl-shared PROPERTIES VERSION ${FL_VERSION})
    target_link_libraries(fl-shared ${FL_LIBS})
    list(APPEND fl-targets fl-shared)
endif ()

if (FL_BUILD_STATIC)
    target_include_directories(fl-static
            PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>)
    set_target_properties(fl-static PROPERTIES OUTPUT_NAME fuzzylite-static)
    set_target_properties(fl-static PROPERTIES DEBUG_POSTFIX -debug)
    set_target_properties(fl-static PROPERTIES VERSION ${FL_VERSION})
    target_link_libraries(fl-static ${FL_LIBS})
    list(APPEND fl-targets fl-static)
endif ()

if (FL_BUILD_BINARY)
    add_executable(fl-bin src/main.cpp)
    # Likewise to fl-obj, fl-bin should not be linked into other projects.
    # The includes are marked private.
    target_include_directories(fl-bin PRIVATE ${PROJECT_SOURCE_DIR})
    set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite)
    set_target_properties(fl-bin PROPERTIES OUTPUT_NAME fuzzylite IMPORT_PREFIX tmp-) #To prevent LNK1149 in Windows
    set_target_properties(fl-bin PROPERTIES DEBUG_POSTFIX -debug)
    target_compile_definitions(fl-bin PRIVATE FL_IMPORT_LIBRARY) #if building with fl-shared
    target_link_libraries(fl-bin fl-shared ${FL_LIBS})
    list(APPEND fl-targets fl-bin)
endif (FL_BUILD_BINARY)

if (FL_BUILD_TESTS AND FL_CPP98)
    message(WARNING "Skipping tests: tests require C++11 or later, but using FL_CPP98=${FL_CPP98}.")

elseif (FL_BUILD_TESTS)
    message(CHECK_START "Detecting Catch2 library in system")
    if (CMAKE_VERSION VERSION_LESS "3.14")
        find_package(Catch2 2 REQUIRED)
    else ()
        find_package(Catch2 2 QUIET)
    endif ()


    if (Catch2_FOUND)
        message(CHECK_PASS "found at ${Catch2_DIR}")
    else ()
        message(CHECK_FAIL "not found")
        # You can install it manually: |
        # git clone -b v2.x https://github.com/catchorg/Catch2.git lib/Catch2
        # cd lib/Catch2
        # cmake -B build -DCATCH_ENABLE_WERROR=OFF .
        # cmake --build build --parallel --target install

        message(CHECK_START "Fetching Catch2 v2.13.10 library from https://github.com/catchorg/Catch2.git")
        set(FETCHCONTENT_QUIET FALSE)
        # But we are going to try to fetch it:
        # see https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md
        Include(FetchContent)
        FetchContent_Declare(
                Catch2
                GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                GIT_TAG v2.13.10
        )
        FetchContent_MakeAvailable(Catch2)
        list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/contrib)
        message(CHECK_PASS "fetched")
    endif ()

    if (FL_BUILD_COVERAGE)
        add_executable(fl-test ${fl-headers} ${fl-sources} ${fl-tests})
        target_include_directories(fl-test
                PRIVATE
                ${PROJECT_SOURCE_DIR})
        target_compile_options(fl-test PRIVATE --coverage -O0)
        target_link_libraries(fl-test PRIVATE ${FL_LIBS} Catch2::Catch2 --coverage)
    else ()
        add_executable(fl-test ${fl-headers} ${fl-tests})
        target_compile_definitions(fl-test PRIVATE FL_IMPORT_LIBRARY)
        target_link_libraries(fl-test PRIVATE fl-shared ${FL_LIBS} Catch2::Catch2)
    endif ()

    set_target_properties(fl-test PROPERTIES OUTPUT_NAME fuzzylite-tests)
    set_target_properties(fl-test PROPERTIES OUTPUT_NAME fuzzylite-tests IMPORT_PREFIX tmp-) #To prevent LNK1149 in Windows
    set_target_properties(fl-test PROPERTIES DEBUG_POSTFIX -debug)

    include(CTest)
    include(Catch)

    enable_testing()

    catch_discover_tests(fl-test)

    list(APPEND fl-targets fl-test)

endif ()

###INSTALL SECTION

if (NOT FL_INSTALL_BINDIR)
    set(FL_INSTALL_BINDIR bin)
endif ()

if (NOT FL_INSTALL_INCLUDEDIR)
    if (CMAKE_INSTALL_INCLUDEDIR)
        set(FL_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
    else ()
        set(FL_INSTALL_INCLUDEDIR include)
    endif ()
endif ()

if (NOT FL_INSTALL_LIBDIR)
    if (CMAKE_INSTALL_LIBDIR)
        set(FL_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
    else ()
        set(FL_INSTALL_LIBDIR lib)
    endif ()
endif ()

install(TARGETS ${fl-targets} EXPORT fuzzylite-config
        RUNTIME DESTINATION ${FL_INSTALL_BINDIR}
        LIBRARY DESTINATION ${FL_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${FL_INSTALL_LIBDIR}
)
# Store the fuzzylite CMake config for other projects to use
install(EXPORT fuzzylite-config DESTINATION share/fuzzylite/cmake)
export(TARGETS ${fl-targets} FILE fuzzylite.cmake)

install(DIRECTORY fuzzylite/ DESTINATION ${FL_INSTALL_INCLUDEDIR}/fuzzylite)
install(DIRECTORY fl/ DESTINATION ${FL_INSTALL_INCLUDEDIR}/fl)

#pkg-config
configure_file(${PROJECT_SOURCE_DIR}/fuzzylite.pc.in ${PROJECT_BINARY_DIR}/fuzzylite.pc @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/fuzzylite.pc DESTINATION ${FL_INSTALL_LIBDIR}/pkgconfig)

message("=====================================")
message("fuzzylite v${FL_VERSION}\n")
message("FL_CPP98=${FL_CPP98}")
message("FL_USE_FLOAT=${FL_USE_FLOAT}")
message("FL_BACKTRACE=${FL_BACKTRACE}")
message("FL_LIBS=${FL_LIBS}")
message("FL_INSTALL_BINDIR=${FL_INSTALL_BINDIR}")
message("FL_INSTALL_LIBDIR=${FL_INSTALL_LIBDIR}")
message("FL_BUILD_TESTS=${FL_BUILD_TESTS}")
message("FL_BUILD_COVERAGE=${FL_BUILD_COVERAGE}")
message("FL_WARNINGS_AS_ERRORS=${FL_WARNINGS_AS_ERRORS}")
message("")
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message("CMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}")
message("CMAKE_EXPORT_COMPILE_COMMANDS=${CMAKE_EXPORT_COMPILE_COMMANDS}")
message("COMPILE_DEFINITIONS:")
get_directory_property(fl-definitions DIRECTORY ${PROJECT_SOURCE_DIR} COMPILE_DEFINITIONS)
foreach (d ${fl-definitions})
    message(STATUS "Defined: " ${d})
endforeach ()

message("=====================================\n")

###UNINSTALL SECTION
#configure_file(
#"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#IMMEDIATE @ONLY)

#add_custom_target(uninstall
#COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)

#unix uninstall
#xargs rm < install_manifest.txt
