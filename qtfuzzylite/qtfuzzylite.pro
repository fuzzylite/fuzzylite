#http://doc.qt.digia.com/qt/qmake-platform-notes.html


win32{
    message("Windows")
    DEFINES += FL_WINDOWS
    LIBS += -ldbghelp
    RC_FILE = ui/fl_windows.rc
}

unix{
    message("Unix")
    DEFINES += FL_UNIX
    ICON = ui/icons/qtfuzzylite.icns
}

macx{
    message("MacOS")
#macdeployqt app-bundle
}


message($$CONFIG)

MAKEFILE = Makefile
TEMPLATE = app

LIBS += -L../fuzzylite/lib


CONFIG(debug, debug|release){
    message("debug")
    DEFINES += FL_DEBUG
    TARGET = qtfuzzylite-dbg
    LIBS += -lfuzzylite-dbg # -lQtSvg
}
CONFIG(release, debug|release){
    message("release")
    TARGET = qtfuzzylite
    LIBS += -lfuzzylite
}


target.path += /usr/local/bin
INSTALLS += target

VERSION = 2.0
DEPENDPATH += . ui
INCLUDEPATH += . ../fuzzylite #/usr/include/qt4 /Qt/4.8.4/include
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp
UI_HEADERS_DIR = ui
UI_SOURCES_DIR = ui
RESOURCES += ui/qtfuzzylite.qrc
RCC_DIR = ui




FORMS += Window.ui Preferences.ui About.ui
FORMS += Variable.ui Wizard.ui Term.ui ImEx.ui Viewer.ui

HEADERS += fl/qt/Window.h fl/qt/Preferences.h fl/qt/About.h
SOURCES += src/Window.cpp src/Preferences.cpp src/About.cpp

HEADERS += fl/qt/Term.h fl/qt/Variable.h fl/qt/Wizard.h
SOURCES += src/Term.cpp src/Variable.cpp src/Wizard.cpp

HEADERS += fl/qt/Model.h fl/qt/Control.h fl/qt/Viewer.h
SOURCES += src/Model.cpp src/Control.cpp src/Viewer.cpp


SOURCES += src/main.cpp
