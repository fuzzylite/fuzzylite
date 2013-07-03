/**
    This file is part of qtfuzzylite.

    qtfuzzylite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qtfuzzylite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qtfuzzylite.  If not, see <http://www.gnu.org/licenses/>.

    Juan Rada-Vilela, 01 February 2013
    jcrada@fuzzylite.com
 **/

/*
 * Window.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_WINDOW_H_
#define FLQT_WINDOW_H_

#include <QMainWindow>
#include <QAction>
#include "ui_Window.h"
#include <fl/Headers.h>



namespace fl {
    namespace qt {
        class Viewer;

        class Window : public QMainWindow {
            Q_OBJECT

        signals:
            void processOutput();

        protected slots:
            void onContextMenuRequest(const QPoint&);

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

            void onSelectTnorm(int selected);
            void onSelectSnorm(int selected);
            void onSelectActivation(int selected);
            void onClickHedges();
            void onActionHedge(const QString& action);

            //Test
            void onInputValueChanged();
            void onClickInputButton();
            void onClickOutputButton();
            void onActionInputButton(const QString& action);
            void onActionOutputButton(const QString& action);

            //MenuBar
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

            void onMenuNew();
            void onMenuAbout();
            void onMenuQuit();

            void closeEvent(QCloseEvent* e);

        protected:
            //            std::vector<QWidget*> _inputs, _outputs;
            QString _lastOpenedFilePath;
            void setupMenuAndToolbar();
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

            static Window* mainWindow();

            void fixDependencies();

            void setup();

            QFont typeWriterFont() const;

            static QString toHtmlEscaped(const QString& x);

            static void main();
        };

        class QScrollAreaFilter : public QObject {
        public:

            bool eventFilter(QObject* object, QEvent* event) {
                if (event->type() == QEvent::Wheel) {
                    event->accept();
                    return true;
                }
                return false;
            }
        };
    }
}
#endif /* FLQT_WINDOW_H_ */
