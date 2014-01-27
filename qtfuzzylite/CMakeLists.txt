cmake_minimum_required(VERSION 2.8)

#After building:
# In Linux:
# In Mac:
# otool -L qtfuzzylite #gives the dependencies of qtfuzzylite
# install_name_tool -change /Users/jcrada/Development/fl/fuzzylite/fuzzylite/bin/libfuzzylite.dylib @executable_path/../Frameworks/libfuzzylite.dylib qtfuzzylite.app/Contents/MacOS/qtfuzzylite

project(qtfuzzylite CXX)

set(CMAKE_VERBOSE_MAKEFILE true)


if(WIN32)
    message("Windows")
    add_definitions(-DFL_WINDOWS)
	set(CMAKE_CXX_FLAGS " /WX /EHsc")
	#set(CMAKE_CXX_FLAGS " /EHsc")
endif()

if (UNIX)
    message("Unix")
    add_definitions(-DFL_UNIX)
endif()

if (APPLE)
    message("Apple")
    add_definitions(-DFL_APPLE)
endif()


set(FLQT_DATE "1401")
add_definitions(-DFLQT_DATE="${FLQT_DATE}")

set(FLQT_VERSION 4.0)
add_definitions(-DFLQT_VERSION="${FLQT_VERSION}")

set(FLQT_LONG_VERSION "${FLQT_VERSION}b${FLQT_DATE}")
set(FLQT_FULLNAME "qtfuzzylite-${FLQT_LONG_VERSION}")

add_definitions(-DFL_BUILD_PATH="${CMAKE_SOURCE_DIR}") #used to determine FL__FILE__

option(FL_USE_FLOAT "Use fl::scalar as float" OFF)
option(FL_DEBUG "Enable additional debugging information" OFF)
option(FL_USE_QT5 "Utilize Qt4 instead of Qt5" OFF)

message("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")


if(FL_DEBUG)
   set(CMAKE_BUILD_TYPE Debug)
   set(FL_CONF -dbg)
   find_library(fl-shared-dbg fuzzylite-dbg 
        HINTS "${CMAKE_SOURCE_DIR}/../fuzzylite/bin")
else(FL_DEBUG)
   set(CMAKE_BUILD_TYPE Release)
   set(FL_CONF)
   find_library(fl-shared NAMES fuzzylite 
        HINTS "${CMAKE_SOURCE_DIR}/../fuzzylite/bin")
endif(FL_DEBUG)


if(FL_USE_FLOAT)
    add_definitions(-DFL_USE_FLOAT)
endif(FL_USE_FLOAT)

include_directories(. ${CMAKE_SOURCE_DIR}/../fuzzylite)


set(qtfl-headers
    fl/qt/About.h
    fl/qt/Control.h
    fl/qt/ImEx.h
    fl/qt/Model.h
    fl/qt/Surface2D.h
    fl/qt/Term.h
    fl/qt/Variable.h
    fl/qt/Viewer.h
    fl/qt/Window.h
    fl/qt/Wizard.h
)

set(qtfl-sources
    src/qtfuzzylite.cpp
    src/About.cpp
    src/Control.cpp
    src/ImEx.cpp
    src/main.cpp
    src/Model.cpp
    src/Surface2D.cpp
    src/Term.cpp
    src/Variable.cpp
    src/Viewer.cpp
    src/Window.cpp
    src/Wizard.cpp
    qwt/qwt_interval.cpp
    qwt/qwt_raster_data.cpp)

set(qtfl-uis
    ui/About.ui
    ui/ImEx.ui
    ui/Surface2D.ui
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
find_package(Qt5Network REQUIRED)

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

qt5_use_modules(qtfuzzylite Widgets Network)
target_link_libraries(qtfuzzylite ${fl-shared${FL_CONF}})

else()
#Use Qt4
find_package(Qt4 REQUIRED QtGui QtCore QtNetwork)
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
    MACOSX_BUNDLE_INFO_STRING "${FLQT_FULLNAME} by Juan Rada-Vilela, licensed under the GNU GPL 3.0"
    MACOSX_BUNDLE_ICON_FILE qtfuzzylite.icns
    MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
    MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
    MACOSX_BUNDLE_BUNDLE_VERSION "${FLQT_DATE}"
    MACOSX_BUNDLE_LONG_VERSION_STRING "${FLQT_LONG_VERSION}"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${FLQT_VERSION}"
    MACOSX_BUNDLE_COPYRIGHT "Juan Rada-Vilela, Licensed under the GNU General Public License 3.0"
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

if(WIN32)
	message("copying ${CMAKE_SOURCE_DIR}/../fuzzylite/bin/fuzzylite${FL_CONF}.dll")
	add_custom_command(TARGET qtfuzzylite POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_SOURCE_DIR}/../fuzzylite/bin/fuzzylite${FL_CONF}.dll"
        ${CMAKE_SOURCE_DIR})                 # <--this is out-file path
endif()


if(NOT APPLE)
    install(TARGETS qtfuzzylite RUNTIME DESTINATION bin)
endif()


message("=====================================")
message("${FLQT_FULLNAME}\n")
message("FL_DEBUG=${FL_DEBUG}")
message("FL_USE_FLOAT=${FL_USE_FLOAT}")
message("FL_USE_QT5=${FL_USE_QT5}")
message("fuzzylite=${fl-shared}${FL_CONF}")
message("=====================================\n")

##unix uninstall
##xargs rm < install_manifest.txt
