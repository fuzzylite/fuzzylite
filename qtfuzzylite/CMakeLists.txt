cmake_minimum_required(VERSION 2.8)

project(qtfuzzylite CXX)

set(CMAKE_VERBOSE_MAKEFILE true)

if(FL_DEBUG)
   message("Configuration: Debug")
   set(CMAKE_BUILD_TYPE Debug)
   set(FL_CONF -dbg)
   find_library(fl-shared-dbg fuzzylite-dbg ${CMAKE_SOURCE_DIR}/../fuzzylite/lib)  
else(FL_DEBUG)
   message("Configuration: Release")
   set(CMAKE_BUILD_TYPE Release)
   set(FL_CONF )
   find_library(fl-shared fuzzylite ${CMAKE_SOURCE_DIR}/../fuzzylite/lib) 
endif(FL_DEBUG)

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

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
    if(CMAKE_GENERATOR STREQUAL Xcode)
        set( WORK_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release" )
    else()
        set( WORK_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
    endif()

    set(qtfl-icon ${CMAKE_CURRENT_SOURCE_DIR}/ui/icons/qtfuzzylite.icns) 
    set_source_files_properties( ${qtfl-icon} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(qtfl-output ${qtfl-output} ${OSX_ICON_FILES})
endif(APPLE)


add_executable(qtfuzzylite
                MACOSX_BUNDLE
                WIN32
                ${qtfl-sources} ${qtfl-output} 
                #for the ide
                ${qtfl-headers} ${qtfl-uis} ${qtfl-mocs} ${qtfl-res})
set_target_properties(qtfuzzylite PROPERTIES DEBUG_POSTFIX -dbg) 
target_link_libraries(qtfuzzylite ${QT_LIBRARIES} ${fl-shared${FL_CONF}})

if(APPLE) #TODO: check if can be merged with previous if(APPLE)
set_target_properties(qtfuzzylite PROPERTIES
	MACOSX_BUNDLE_INFO_STRING "qtfuzzylite Version ${VERSION_STRING}, Copyright © 2013, Juan Rada-Vilela, All Rights Reserved."
	MACOSX_BUNDLE_ICON_FILE qtfuzzylite.icns
	MACOSX_BUNDLE_GUI_IDENTIFIER "qtfuzzylite"
	MACOSX_BUNDLE_BUNDLE_NAME "qtfuzzylite"
	MACOSX_BUNDLE_BUNDLE_VERSION "${FL_COMMIT_ID}"
	MACOSX_BUNDLE_LONG_VERSION_STRING "qtfuzzylite Version 2.0, ${FL_COMMIT_ID}"
	MACOSX_BUNDLE_SHORT_VERSION_STRING "2.0"
	MACOSX_BUNDLE_COPYRIGHT "Copyright © 2013, Juan Rada-Vilela, All Rights Reserved."
)
    add_custom_target(  addframeworks ALL 
                        COMMAND ${QT_BINARY_DIR}/macdeployqt qtfuzzylite.app
                        WORKING_DIRECTORY ${WORK_DIR}
                        DEPENDS ${PROJECT_NAME})

#TODO:check if -no-plugins are needed
    add_custom_target(  makedmg 
                        COMMAND ${QT_BINARY_DIR}/macdeployqt FlightCrew-gui.app -dmg -no-plugins
                        WORKING_DIRECTORY ${WORK_DIR}
                        DEPENDS addframeworks )
 

#macdeployqt qtfuzzylite.app
#http://dragly.org/2012/01/13/deploy-qt-applications-for-mac-os-x/
endif(APPLE)


install(TARGETS qtfuzzylite RUNTIME DESTINATION bin)

##unix uninstall
##xargs rm < install_manifest.txt
