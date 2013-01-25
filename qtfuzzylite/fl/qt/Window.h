/*
 * Window.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_WINDOW_H_
#define FLQT_WINDOW_H_

#include <QtGui/QMainWindow>
#include <QtGui/QAction>

#include "ui/ui_Window.h"
#include <fl/Headers.h>


namespace fl {
    namespace qt {
        class Viewer;
        class Preferences;

        class Window : public QMainWindow {
            Q_OBJECT

        signals:
            void processOutput();

        protected slots:
            void onChangeInputSelection();
            void onChangeOutputSelection();
            void onDoubleClickInputItem(QListWidgetItem* item);
            void onDoubleClickOutputItem(QListWidgetItem* item);

            void onClickAddInputVariable();
            void onClickRemoveInputVariable();
            void onClickEditInputVariable();

            void onClickAddOutputVariable();
            void onClickRemoveOutputVariable();
            void onClickEditOutputVariable();

            void onClickGenerateAllRules();
            void onClickParseAllRules();

            void onTabChange(int);

            void onSelectTestRule(int selected);
            void onSelectTestActivation(int selected);

            //Test
            void onInputValueChanged();

            //MenuBar
            void onMenuPreferences();
            void onMenuTerms();

            bool confirmImporting();
            void onMenuImport();
            void onMenuExport();
            void onMenuImportFromFCL();
            void onMenuExportToFCL();
            void onMenuImportFromFIS();
            void onMenuExportToFIS();
            void onMenuImportFromFile();
            void onMenuExportToCpp();

            void onMenuReset();
            void onMenuAbout();
            void onMenuQuit();

            void closeEvent(QCloseEvent* e);

        protected:
//            std::vector<QWidget*> _inputs, _outputs;
            QString _lastOpenedFilePath;
            void connect();
            void disconnect();

            void reloadModel();
            void removeRules();

            void reloadTest();
            void resetTest();

            void resizeEvent(QResizeEvent* e);
            void showEvent(QShowEvent* e);

            Window(QWidget* parent = NULL, Qt::WindowFlags flags = 0);
            ~Window();
            static Window* instance;
            
            Viewer* _inputViewer;
            Viewer* _outputViewer;
            
        public:
            Ui::Window* ui;
            Preferences* preferences;

            static Window* mainWindow();

            void fixDependencies();

            void setup();

            QFont typeWriterFont() const;

            static void main();
        };

        
    }
}
#endif /* FLQT_WINDOW_H_ */
