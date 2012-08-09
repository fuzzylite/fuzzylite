

CONFIG-=app_bundle

MAKEFILE = Makefile
TEMPLATE = app
TARGET = dist/gui
DEPENDPATH += . src ui
INCLUDEPATH += . ../fuzzylite/
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp
UI_HEADERS_DIR = gui
UI_SOURCES_DIR = ui
RESOURCES += ui/resources.qrc
RCC_DIR = src/

DEFINES *= FL_USE_LOG FL_USE_DEBUG FL_USE_ASSERT

LIBS += -L../fuzzylite/unix-static -lfuzzylite



#################### FORMS
FORMS += MainWindow.ui LinguisticVariable.ui LinguisticVariableWizard.ui
FORMS += LinguisticTerm.ui Grapher.ui Preferences.ui

#################### GUI
HEADERS += gui/GuiLinguisticVariable.h gui/GuiLinguisticTerm.h
SOURCES += src/GuiLinguisticVariable.cpp src/GuiLinguisticTerm.cpp

HEADERS += gui/GuiLVarWizard.h gui/GuiGrapher.h gui/GuiPreferences.h
SOURCES += src/GuiLVarWizard.cpp src/GuiGrapher.cpp src/GuiPreferences.cpp

HEADERS += gui/GuiCanvas.h
SOURCES += src/GuiCanvas.cpp

HEADERS += gui/GuiMainWindow.h
SOURCES += src/GuiMainWindow.cpp


##################### HEADERS AND SOURCES
HEADERS += gui/InputController.h gui/OutputController.h gui/RuleController.h
SOURCES += src/InputController.cpp src/OutputController.cpp src/RuleController.cpp

HEADERS += gui/TestController.h
SOURCES += src/TestController.cpp

HEADERS += gui/MainWindowController.h
SOURCES += src/MainWindowController.cpp



HEADERS += gui/LinguisticVariableController.h
SOURCES += src/LinguisticVariableController.cpp

HEADERS += gui/LinguisticTermController.h
SOURCES += src/LinguisticTermController.cpp

SOURCES += src/main.cpp
