CONFIG-= app release warn_on

MAKEFILE = Makefile
TEMPLATE = app
TARGET = qtfuzzylite
DEPENDPATH += . src ui
INCLUDEPATH += . /usr/include/qt4
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp
UI_HEADERS_DIR = ui
UI_SOURCES_DIR = ui
RESOURCES += ui/resources.qrc
RCC_DIR = ui/

LIBS += -lfuzzylite #-lQtSvg

target.path += bin
INSTALLS += target

FORMS += Window.ui Configuration.ui
FORMS += Variable.ui Wizard.ui Term.ui ImEx.ui Viewer.ui

HEADERS += fl/qt/Window.h fl/qt/Configuration.h
SOURCES += src/Window.cpp src/Configuration.cpp

HEADERS += fl/qt/Term.h fl/qt/Variable.h fl/qt/Wizard.h
SOURCES += src/Term.cpp src/Variable.cpp src/Wizard.cpp

HEADERS += fl/qt/Model.h fl/qt/Control.h fl/qt/Viewer.h
SOURCES += src/Model.cpp src/Control.cpp src/Viewer.cpp


SOURCES += src/main.cpp
