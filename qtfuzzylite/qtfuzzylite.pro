#CONFIG+= qt app release warn_on static
#http://doc.qt.digia.com/qt/qmake-platform-notes.html


win32{
    DEFINES += FL_WINDOWS
    CONFIG -= debug_and_release release 
    LIBS += -ldbghelp
	RC_FILE = ui/fl_windows.rc
}

unix{
    DEFINES += FL_UNIX
}

macx{
	ICON = ui/icons/qtfuzzylite.icns
}

message($$CONFIG)

MAKEFILE = Makefile
TEMPLATE = app

TARGET = qtfuzzylite

VERSION = 2.0
DEPENDPATH += . src ui
INCLUDEPATH += . ../fuzzylite #/usr/include/qt4 /Qt/4.8.4/include
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp
UI_HEADERS_DIR = ui
UI_SOURCES_DIR = ui
RESOURCES += ui/resources.qrc
RCC_DIR = ui


LIBS += -L../fuzzylite/build -lfuzzylite# -lQtSvg

target.path += /usr/local/bin
INSTALLS += target

FORMS += Window.ui Preferences.ui About.ui
FORMS += Variable.ui Wizard.ui Term.ui ImEx.ui Viewer.ui

HEADERS += fl/qt/Window.h fl/qt/Preferences.h fl/qt/About.h
SOURCES += src/Window.cpp src/Preferences.cpp src/About.cpp

HEADERS += fl/qt/Term.h fl/qt/Variable.h fl/qt/Wizard.h
SOURCES += src/Term.cpp src/Variable.cpp src/Wizard.cpp

HEADERS += fl/qt/Model.h fl/qt/Control.h fl/qt/Viewer.h
SOURCES += src/Model.cpp src/Control.cpp src/Viewer.cpp


SOURCES += src/main.cpp
