

CONFIG-=app_bundle

MAKEFILE = Makefile
TEMPLATE = app
TARGET = dist/qtfuzzylite
DEPENDPATH += . src ui
INCLUDEPATH += ../fuzzylite/
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp
UI_HEADERS_DIR = ui
UI_SOURCES_DIR = ui
RESOURCES += ui/resources.qrc
RCC_DIR = src/

LIBS += -L../fuzzylite/unix-static -lfuzzylite



#################### FORMS
FORMS += MainWindow.ui LinguisticVariable.ui LinguisticVariableWizard.ui
FORMS += LinguisticTerm.ui Grapher.ui Preferences.ui

#################### GUI
HEADERS += src/GuiLinguisticVariable.h src/GuiLinguisticTerm.h
SOURCES += src/GuiLinguisticVariable.cpp src/GuiLinguisticTerm.cpp

HEADERS += src/GuiLVarWizard.h src/GuiGrapher.h src/GuiPreferences.h
SOURCES += src/GuiLVarWizard.cpp src/GuiGrapher.cpp src/GuiPreferences.cpp

HEADERS += src/GuiCanvas.h
SOURCES += src/GuiCanvas.cpp

HEADERS += src/GuiMainWindow.h
SOURCES += src/GuiMainWindow.cpp


##################### HEADERS AND SOURCES
HEADERS += src/InputController.h src/OutputController.h src/RuleController.h
SOURCES += src/InputController.cpp src/OutputController.cpp src/RuleController.cpp

HEADERS += src/TestController.h
SOURCES += src/TestController.cpp

HEADERS += src/MainWindowController.h
SOURCES += src/MainWindowController.cpp



HEADERS += src/LinguisticVariableController.h
SOURCES += src/LinguisticVariableController.cpp

HEADERS += src/LinguisticTermController.h
SOURCES += src/LinguisticTermController.cpp

SOURCES += src/main.cpp
