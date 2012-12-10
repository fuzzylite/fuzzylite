

CONFIG-=app_bundle

MAKEFILE = Makefile
TEMPLATE = app
TARGET = bin/qtfuzzylite
DEPENDPATH += . src ui
INCLUDEPATH += ../fuzzylite/include ../fuzzylite/include/fl 
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp
UI_HEADERS_DIR = ui
UI_SOURCES_DIR = ui
RESOURCES += ui/resources.qrc
RCC_DIR = src/

LIBS += -L../fuzzylite/lib -lfuzzylite


HEADERS += src/Canvas.h
SOURCES += src/Canvas.cpp

SOURCES += src/main.cpp
