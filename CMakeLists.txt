cmake_minimum_required(VERSION 3.15)

set(FL_VERSION 7.0.0)
project(fuzzylite VERSION ${FL_VERSION} LANGUAGES CXX)

if ("${CMAKE_CXX_STANDARD}" STREQUAL "")
    set(CMAKE_CXX_STANDARD 11)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
# export commands for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GNUInstallDirs: resolves common paths (eg, CMAKE_INSTALL_[LIB|INCLUDE|DATAROOT]DIR)
include(GNUInstallDirs)

###DEFINES SECTION
option(FL_BUILD_SHARED "Build shared library" ON)
option(FL_BUILD_STATIC "Build static library" ON)
if (FL_BUILD_SHARED)
    option(FL_BUILD_BINARY "Build fuzzylite binary" ON)
endif ()

option(FL_USE_FLOAT "Use fl::scalar as float" OFF)
option(FL_BACKTRACE "Provide backtrace information in case of errors" ON)

option(FL_BUILD_TESTS "Builds the unit tests" ON)
option(FL_BUILD_COVERAGE "Build the unit tests to compute coverage" OFF)

if (CMAKE_CXX_STANDARD STREQUAL 98)
    add_definitions(-DFL_CPP98)
    message(DEPRECATION "fuzzylite 8 will drop support for standard C++98")
endif ()

if (FL_USE_FLOAT)
    add_definitions(-DFL_USE_FLOAT)
endif ()

if (FL_BACKTRACE)
    add_definitions(-DFL_BACKTRACE)
endif ()

# Put all binaries in same location
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

if (UNIX)
    #Add Unix compilation flags
    add_compile_options(-pedantic -Wall -Wextra) # -Wconversion -Wsign-conversion
    if (NOT APPLE)
        add_link_options(-Wl,--no-undefined)
    endif ()

    # strip path from __FILE__
    # add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.)
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
    #Address fl::null errors of literal null conversion
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-non-literal-null-conversion")
    #TEMPORAL: flag removal to avoid warnings of `float` variadic in Operation::join
    add_compile_options(-Wno-varargs)
endif ()

set(FL_LIBS)

if (MSVC)
    #Set compilation flags in Windows
    add_compile_options(/W4 /EHsc)
    if (FL_USE_FLOAT)
        # warning C4244: 'argument': conversion from 'double' to '_Ty', possible loss of data
        # warning C4305: 'return': truncation from 'double' to 'fuzzylite::scalar'
        add_compile_options(/wd4244 /wd4305)
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

# BUILD SECTION

file(STRINGS FL_HEADERS fl-headers)
file(STRINGS FL_SOURCES fl-sources)
file(STRINGS FL_TESTS fl-tests)

string(REGEX REPLACE "\n" " " ${fl-headers} ${fl-headers})
string(REGEX REPLACE "\n" " " ${fl-sources} ${fl-sources})
string(REGEX REPLACE "\n" " " ${fl-tests} ${fl-tests})

set(fl-targets)

if (MSVC OR CMAKE_GENERATOR STREQUAL Xcode)
    if (FL_BUILD_SHARED)
        add_library(dynamicTarget SHARED ${fl-headers} ${fl-sources})
    endif ()

    if (FL_BUILD_STATIC)
        add_library(staticTarget STATIC ${fl-headers} ${fl-sources})
    endif ()
else ()
    if (FL_BUILD_SHARED OR FL_BUILD_STATIC)
        add_library(objectTarget OBJECT ${fl-headers} ${fl-sources})
        # The objects need the fuzzylite headers to compile, but
        # these should only be visile to objectTarget at this time, so
        # they are marked as PRIVATE.
        target_include_directories(objectTarget PRIVATE ${PROJECT_SOURCE_DIR})
        set_target_properties(objectTarget PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
        if (NOT MINGW)
            set_target_properties(objectTarget PROPERTIES POSITION_INDEPENDENT_CODE ON)
        endif ()
    endif ()

    if (FL_BUILD_SHARED)
        add_library(dynamicTarget SHARED $<TARGET_OBJECTS:objectTarget>)
    endif ()

    if (FL_BUILD_STATIC)
        add_library(staticTarget STATIC $<TARGET_OBJECTS:objectTarget>)
    endif ()
endif ()

if (FL_BUILD_SHARED)
    # The public include directories to be shared with projects that
    # link to fuzzylite depend on whether they're being included from
    # source (BUILD_INTERFACE), or from /usr/include (INSTALL_INTERFACE)
    target_include_directories(dynamicTarget
            PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>)

    set_target_properties(dynamicTarget PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_target_properties(dynamicTarget PROPERTIES OUTPUT_NAME fuzzylite)
    set_target_properties(dynamicTarget PROPERTIES VERSION ${FL_VERSION})
    target_compile_definitions(dynamicTarget PRIVATE FL_EXPORT_LIBRARY)
    target_link_libraries(dynamicTarget ${FL_LIBS})
    list(APPEND fl-targets dynamicTarget)
endif ()

if (FL_BUILD_STATIC)
    target_include_directories(staticTarget PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:include>)

    set_target_properties(staticTarget PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_target_properties(staticTarget PROPERTIES OUTPUT_NAME fuzzylite-static)
    set_target_properties(staticTarget PROPERTIES VERSION ${FL_VERSION})
    target_link_libraries(staticTarget ${FL_LIBS})

    list(APPEND fl-targets staticTarget)
endif ()

if (FL_BUILD_BINARY)
    add_executable(binaryTarget src/main.cpp)
    # Likewise to objectTarget, binaryTarget should not be linked into other projects.
    # The includes are marked private.
    target_include_directories(binaryTarget PRIVATE ${PROJECT_SOURCE_DIR})
    set_target_properties(binaryTarget PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_target_properties(binaryTarget PROPERTIES OUTPUT_NAME fuzzylite)
    set_target_properties(binaryTarget PROPERTIES OUTPUT_NAME fuzzylite IMPORT_PREFIX tmp-) #To prevent LNK1149 in Windows
    target_compile_definitions(binaryTarget PRIVATE FL_IMPORT_LIBRARY) #if building with dynamicTarget
    target_link_libraries(binaryTarget dynamicTarget ${FL_LIBS})

    list(APPEND fl-targets binaryTarget)
endif ()

if (FL_BUILD_TESTS AND NOT CMAKE_CXX_STANDARD EQUAL 98)
    message(CHECK_START "Detecting Catch2 library in system")
    # `make install-catch2` will install Catch2 under `.local`
    find_package(Catch2 3 QUIET HINTS .local/)

    if (Catch2_FOUND)
        message(CHECK_PASS "found at ${Catch2_DIR}")
    else ()
        message(CHECK_FAIL "not found")

        set(FL_CATCH_TAG v3.7.1)
        message(CHECK_START "Fetching Catch2 ${FL_CATCH_TAG} library from https://github.com/catchorg/Catch2.git")
        set(FETCHCONTENT_QUIET FALSE)
        # But we are going to try to fetch it:
        # see https://github.com/catchorg/Catch2/blob/v2.x/docs/cmake-integration.md
        include(FetchContent)
        fetchcontent_declare(
                Catch2
                GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                GIT_TAG ${FL_CATCH_TAG}
        )
        fetchcontent_makeavailable(Catch2)
        message(CHECK_PASS "fetched")
    endif ()

    include(Catch)

    if (FL_BUILD_COVERAGE)
        add_executable(testTarget ${fl-headers} ${fl-sources} ${fl-tests})
        target_include_directories(testTarget PRIVATE ${PROJECT_SOURCE_DIR})
        target_compile_options(testTarget PRIVATE --coverage -O0)
        target_link_libraries(testTarget PRIVATE ${FL_LIBS} Catch2::Catch2WithMain --coverage)
    else ()
        add_executable(testTarget ${fl-headers} ${fl-tests})
        target_compile_definitions(testTarget PRIVATE FL_IMPORT_LIBRARY)
        target_link_libraries(testTarget PRIVATE dynamicTarget ${FL_LIBS} Catch2::Catch2WithMain)
    endif ()

    set_target_properties(testTarget PROPERTIES OUTPUT_NAME fuzzylite-tests)
    set_target_properties(testTarget PROPERTIES OUTPUT_NAME fuzzylite-tests IMPORT_PREFIX tmp-) #To prevent LNK1149 in Windows

    # Catch2 v3 minimum CXX_STANDARD is 14, but seems to build fine with C++11
    # We use the CXX_STANDARD used to build fuzzylite.
    set_target_properties(testTarget PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})

    # disable creation of test targets for nightly, continuous and experimental builds
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED ON)

    include(CTest)

    enable_testing()

    catch_discover_tests(testTarget)

    list(APPEND fl-targets testTarget)

endif ()

# INSTALL SECTION

install(TARGETS ${fl-targets} EXPORT fuzzylite
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
## Store the fuzzylite CMake config for other projects to use
install(EXPORT fuzzylite
        NAMESPACE fuzzylite::
        DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/fuzzylite"
)
export(TARGETS ${fl-targets} FILE fuzzylite.cmake)

install(DIRECTORY fuzzylite DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY fl DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

## Provide pkg-config integration
set(PKGCONFIG_INSTALL_DIR
        "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig"
        CACHE PATH "Path where fuzzylite.pc is installed"
)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/fuzzylite.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/fuzzylite.pc
        @ONLY
)
install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/fuzzylite.pc"
        DESTINATION
        ${PKGCONFIG_INSTALL_DIR}
)

# VARIABLES
message("=====================================")
message("fuzzylite v${FL_VERSION}")
message("")
message("FL_USE_FLOAT=${FL_USE_FLOAT}")
message("FL_BACKTRACE=${FL_BACKTRACE}")
message("FL_LIBS=${FL_LIBS}")
message("FL_BUILD_TESTS=${FL_BUILD_TESTS}")
message("FL_BUILD_COVERAGE=${FL_BUILD_COVERAGE}")
message("")
message("CMAKE_GENERATOR=${CMAKE_GENERATOR}")
message("CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
message("CMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}")
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
message("CMAKE_COMPILE_WARNING_AS_ERROR=${CMAKE_COMPILE_WARNING_AS_ERROR}")
message("CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message("CMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}")
message("CMAKE_EXPORT_COMPILE_COMMANDS=${CMAKE_EXPORT_COMPILE_COMMANDS}")
message("CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
message("CMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}")
message("CMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}")
message("CMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}")
message("CMAKE_INSTALL_DATAROOTDIR=${CMAKE_INSTALL_DATAROOTDIR}")
message("COMPILE_DEFINITIONS:")
get_directory_property(fl-definitions DIRECTORY ${PROJECT_SOURCE_DIR} COMPILE_DEFINITIONS)
foreach (d ${fl-definitions})
    message(STATUS "Defined: " ${d})
endforeach ()

message("=====================================\n")

####UNINSTALL SECTION
#configure_file(
#"${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#IMMEDIATE @ONLY)

#add_custom_target(uninstall
#        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)

#unix uninstall
#xargs rm < install_manifest.txt
