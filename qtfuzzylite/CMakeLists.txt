cmake_minimum_required(VERSION 2.8)

project(qtfuzzylite CXX)

set(CMAKE_VERBOSE_MAKEFILE true)

MACRO (TODAY RESULT)
    IF (WIN32)
        EXECUTE_PROCESS(COMMAND "cmd" " /C date /T" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE ".*../(..)/..(..).*" "\\2\\1" ${RESULT} ${${RESULT}})
    ELSEIF(UNIX)
        EXECUTE_PROCESS(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
        string(REGEX REPLACE ".*../(..)/..(..).*" "\\2\\1" ${RESULT} ${${RESULT}})
    ELSE ()
        MESSAGE(SEND_ERROR "date not implemented")
        SET(${RESULT} 000000)
    ENDIF (WIN32)
ENDMACRO (TODAY)

TODAY(FLQT_DATE)
add_definitions(-DFLQT_DATE="${FLQT_DATE}")

set(FLQT_VERSION 3.0)
add_definitions(-DFLQT_VERSION="${FLQT_VERSION}")

set(FLQT_LONG_VERSION "${FLQT_VERSION}b${FLQT_DATE}")
set(FLQT_FULLNAME "qtfuzzylite-${FLQT_LONG_VERSION}")

add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine FL__FILE__

option(FL_USE_FLOAT "Use fl::scalar as float" OFF)
option(FL_USE_LOG "Enable the macro FL_LOG to print information" ON)
option(FL_DEBUG "Enable additional debugging information" OFF)
option(FL_USE_QT5 "Utilize Qt5 instead of Qt4" OFF)

message("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
if(FL_DEBUG)
   set(CMAKE_BUILD_TYPE Debug)
   set(FL_CONF -dbg)
   find_library(fl-shared-dbg fuzzylite-dbg 
        HINTS "${CMAKE_SOURCE_DIR}/../fuzzylite/lib")
else(FL_DEBUG)
   set(CMAKE_BUILD_TYPE Release)
   set(FL_CONF)
   find_library(fl-shared NAMES fuzzylite 
        HINTS "${CMAKE_SOURCE_DIR}/../fuzzylite/lib")
endif(FL_DEBUG)


if(FL_USE_FLOAT)
    add_definitions(-DFL_USE_FLOAT)
endif(FL_USE_FLOAT)

if(NOT FL_USE_LOG)
    add_definitions(-DFL_NO_LOG)
endif(NOT FL_USE_LOG)


if(WIN32)
    message("Windows")
	add_definitions(-DFL_WINDOWS)
endif()

include_directories(. ${CMAKE_SOURCE_DIR}/../fuzzylite)


set(qtfl-headers
    fl/qt/About.h
    fl/qt/Control.h
    fl/qt/Term.h
    fl/qt/TermListWidget.h
    fl/qt/Variable.h
    fl/qt/Viewer.h
    fl/qt/Window.h
    fl/qt/Wizard.h)

set(qtfl-sources
    src/qtfuzzylite.cpp
    src/About.cpp
    src/Control.cpp
    src/main.cpp
    src/Model.cpp
    src/Term.cpp
    src/TermListWidget.cpp
    src/Variable.cpp
    src/Viewer.cpp
    src/Window.cpp
    src/Wizard.cpp)

set(qtfl-uis
    ui/About.ui
    ui/ImEx.ui
    ui/Term.ui
    ui/Variable.ui
    ui/Viewer.ui
    ui/Window.ui
    ui/Wizard.ui)
   


if(APPLE)
    set(qtfl-icon ${CMAKE_CURRENT_SOURCE_DIR}/ui/icons/qtfuzzylite.icns)
    set_source_files_properties( ${qtfl-icon} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(qtfl-output ${qtfl-output} ${qtfl-icon})
endif(APPLE)

if(WIN32)
    set(qtfl-sources ${qtfl-sources} ui/qtfuzzylite.rc)
endif()


if(FL_USE_QT5)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Core REQUIRED)
#find_package(Qt5WebKit REQUIRED)

link_libraries( ${QT_LIBRARIES} )
if(WIN32)
        link_libraries( ${QT_QTMAIN_LIBRARY} )
endif(WIN32)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
qt5_wrap_ui(qtfl-output  ${qtfl-uis})
qt5_add_resources(qtfl-output ui/qtfuzzylite.qrc)

add_executable(qtfuzzylite
                MACOSX_BUNDLE
                WIN32
                ${qtfl-sources} ${qtfl-output}
                #for the ide
                ${qtfl-headers} ${qtfl-uis})
set_target_properties(qtfuzzylite PROPERTIES DEBUG_POSTFIX -dbg)

qt5_use_modules(qtfuzzylite Widgets)
target_link_libraries(qtfuzzylite ${fl-shared${FL_CONF}})

#find_package(Qt5Core REQUIRED)
#find_package(Qt5Widgets REQUIRED)
#set(CMAKE_AUTOMOC ON)

#add_definitions(${Qt5Widgets_DEFINITIONS})
#include_directories(${Qt5Widgets_INCLUDE_DIRS})
#target_link_libraries(qtfuzzylite ${fl-shared${FL_CONF}} ${Qt5Widgets_LIBRARIES})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#qt5_wrap_cpp(qtfl-output ${qtfl-headers})
#qt5_wrap_ui(qtfl-output  ${qtfl-uis})
#qt5_add_resources(qtfl-output ui/qtfuzzylite.qrc)




else()

find_package(Qt4 REQUIRED)

add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

qt4_wrap_cpp(qtfl-output ${qtfl-headers})
qt4_wrap_ui(qtfl-output  ${qtfl-uis})
qt4_add_resources(qtfl-output ui/qtfuzzylite.qrc)

add_executable(qtfuzzylite
                MACOSX_BUNDLE
                WIN32
                ${qtfl-sources} ${qtfl-output}
                #for the ide
                ${qtfl-headers} ${qtfl-uis})
set_target_properties(qtfuzzylite PROPERTIES DEBUG_POSTFIX -dbg)
target_link_libraries(qtfuzzylite ${QT_LIBRARIES} ${fl-shared${FL_CONF}})

endif()



if(APPLE)
    set_target_properties(qtfuzzylite PROPERTIES
    MACOSX_BUNDLE_INFO_STRING "${FLQT_FULLNAME}, Licensed under GNU GPL. Copyright © Juan Rada-Vilela, All Rights Reserved."
    MACOSX_BUNDLE_ICON_FILE qtfuzzylite.icns
    MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
    MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
    MACOSX_BUNDLE_BUNDLE_VERSION "${FLQT_DATE}"
    MACOSX_BUNDLE_LONG_VERSION_STRING "${FLQT_LONG_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${FLQT_VERSION}"
    MACOSX_BUNDLE_COPYRIGHT "Copyright © 2013 Juan Rada-Vilela, Licensed under the GNU General Public License (GPL-2.0)"
)
    add_custom_target(  mac-deploy ALL
                        COMMAND ${QT_BINARY_DIR}/macdeployqt qtfuzzylite.app
                        WORKING_DIRECTORY ${WORK_DIR}
                        DEPENDS ${PROJECT_NAME})
    add_custom_target(  mac-dmg
                        COMMAND ${QT_BINARY_DIR}/macdeployqt qtfuzzylite.app -dmg -no-plugins
                        WORKING_DIRECTORY ${WORK_DIR}
                        DEPENDS mac-deploy )


#macdeployqt qtfuzzylite.app
#http://dragly.org/2012/01/13/deploy-qt-applications-for-mac-os-x/
endif(APPLE)


if(NOT APPLE)
    install(TARGETS qtfuzzylite RUNTIME DESTINATION bin)
endif()


message("=====================================")
message("${FLQT_FULLNAME}\n")
message("FL_DEBUG=${FL_DEBUG}")
message("FL_USE_LOG=${FL_USE_LOG}")
message("FL_USE_FLOAT=${FL_USE_FLOAT}")
message("FL_USE_QT5=${FL_USE_QT5}")
message("fuzzylite=${fl-shared}${FL_CONF}")
message("=====================================\n")

##unix uninstall
##xargs rm < install_manifest.txt
