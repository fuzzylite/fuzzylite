cmake_minimum_required(VERSION 2.8)

project(qtfuzzylite CXX)

MACRO (TODAY RESULT)
    IF (WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE ".*(..)/(..)/..(..).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE "(..)/(..)/..(..).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
    ELSE ()
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 000000)
    ENDIF (WIN32)
ENDMACRO (TODAY)

TODAY(FLQT_DATE)
add_definitions(-DFLQT_DATE="${FLQT_DATE}")

set(FLQT_VERSION 2.0)
add_definitions(-DFLQT_VERSION="${FLQT_VERSION}")

message("qtfuzzylite v.${FLQT_VERSION}-${FLQT_DATE}")

set(CMAKE_VERBOSE_MAKEFILE true)
message("Source path:" ${CMAKE_SOURCE_DIR})
if(FL_DEBUG)
   message("Configuration: Debug")
   set(CMAKE_BUILD_TYPE Debug)
   set(FL_CONF -dbg)
   find_library(fl-shared-dbg fuzzylite-dbg ${CMAKE_SOURCE_DIR}/../fuzzylite/lib)
else(FL_DEBUG)
   message("Configuration: Release")
   set(CMAKE_BUILD_TYPE Release)
   set(FL_CONF)
   find_library(fl-shared fuzzylite ${CMAKE_SOURCE_DIR}/../fuzzylite/lib)
endif(FL_DEBUG)

message("fuzzylite library: ${fl-shared}${FL_CONF}")


find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

if(WIN32)
    message("Windows building")
	add_definitions(-DFL_WINDOWS)
endif()

include_directories(. ${CMAKE_SOURCE_DIR}/../fuzzylite)



set(qtfl-headers
    fl/qt/About.h
    fl/qt/Control.h
    fl/qt/Preferences.h
    fl/qt/Term.h
    fl/qt/Variable.h
    fl/qt/Viewer.h
    fl/qt/Window.h
    fl/qt/Wizard.h)

set(qtfl-sources
    src/About.cpp
    src/Control.cpp
    src/main.cpp
    src/Model.cpp
    src/Preferences.cpp
    src/Term.cpp
    src/Variable.cpp
    src/Viewer.cpp
    src/Window.cpp
    src/Wizard.cpp)

set(qtfl-uis
    ui/About.ui
    ui/ImEx.ui
    ui/Preferences.ui
    ui/Term.ui
    ui/Variable.ui
    ui/Viewer.ui
    ui/Window.ui
    ui/Wizard.ui)


qt4_wrap_cpp(qtfl-output ${qtfl-headers})
qt4_wrap_ui(qtfl-output  ${qtfl-uis})
qt4_add_resources(qtfl-output ui/qtfuzzylite.qrc)

if(APPLE)
    set(qtfl-icon ${CMAKE_CURRENT_SOURCE_DIR}/ui/icons/qtfuzzylite.icns)
    set_source_files_properties( ${qtfl-icon} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(qtfl-output ${qtfl-output} ${qtfl-icon})
endif(APPLE)

if(WIN32)
    set(qtfl-sources ${qtfl-sources} ui/qtfuzzylite.rc)
endif()


add_executable(qtfuzzylite
                MACOSX_BUNDLE
                WIN32
                ${qtfl-sources} ${qtfl-output}
                #for the ide
                ${qtfl-headers} ${qtfl-uis} ${qtfl-mocs} ${qtfl-res})
set_target_properties(qtfuzzylite PROPERTIES DEBUG_POSTFIX -dbg)
target_link_libraries(qtfuzzylite ${QT_LIBRARIES} ${fl-shared${FL_CONF}})

if(APPLE)
    set_target_properties(qtfuzzylite PROPERTIES
    MACOSX_BUNDLE_INFO_STRING "qtfuzzylite Version ${FLQT_VERSION}-${FLQT_DATE}, Copyright © 2013, Juan Rada-Vilela, All Rights Reserved."
    MACOSX_BUNDLE_ICON_FILE qtfuzzylite.icns
    MACOSX_BUNDLE_GUI_IDENTIFIER "qtfuzzylite"
    MACOSX_BUNDLE_BUNDLE_NAME "qtfuzzylite"
    MACOSX_BUNDLE_BUNDLE_VERSION "${FLQT_VERSION}-${FLQT_DATE}"
    MACOSX_BUNDLE_LONG_VERSION_STRING "qtfuzzylite Version ${FLQT_VERSION}-${FLQT_DATE}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${FLQT_VERSION}"
    MACOSX_BUNDLE_COPYRIGHT "Copyright © 2013, Juan Rada-Vilela, All Rights Reserved."
)
    add_custom_target(  mac-deploy ALL
                        COMMAND ${QT_BINARY_DIR}/macdeployqt qtfuzzylite.app
                        WORKING_DIRECTORY ${WORK_DIR}
                        DEPENDS ${PROJECT_NAME})
    add_custom_target(  mac-dmg
                        COMMAND ${QT_BINARY_DIR}/macdeployqt qtfuzzylite.app -dmg -no-plugins
                        WORKING_DIRECTORY ${WORK_DIR}
                        DEPENDS addframeworks )


#macdeployqt qtfuzzylite.app
#http://dragly.org/2012/01/13/deploy-qt-applications-for-mac-os-x/
endif(APPLE)


install(TARGETS qtfuzzylite RUNTIME DESTINATION bin)

if(NOT APPLE)
    install(TARGETS qtfuzzylite RUNTIME DESTINATION bin)
endif()
##unix uninstall
##xargs rm < install_manifest.txt
