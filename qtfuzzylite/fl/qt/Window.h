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
#include <QWidgetAction>
#include <QtNetwork/QNetworkReply>
#include <QSettings>
#include "ui_Window.h"

#include <fl/Headers.h>
#include <utility>

#include "fl/qt/Model.h"
#include "fl/qt/Viewer.h"



namespace fl {
    namespace qt {
        class Viewer;

        class Window : public QMainWindow {
            Q_OBJECT

        signals:
            void processOutput();
            void engineVariableChanged();

        protected slots:
            void onChangeEngineName(const QString&);

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
            void onClickInputOutputButton();
            void onActionInputOutputButton();

            //MenuBar
            void onMenuAbout();
            void onMenuAboutQt();

            void onMenuNew();
            void onMenuOpen();
            void onMenuOpenRecent();
            void openFile(const QString& filename, const QString& unsavedChangedTitle = "Open Engine");

            void onMenuSave();
            void onMenuSaveAs();
            void saveFile(QString filename);
            void onMenuReload();

            void onMenuTerms();
            void onMenuSurface2D();

            bool confirmChanges(const QString& before);

            void onMenuImport();
            void onMenuExport();
            void onMenuImportFromFLL();
            void onMenuExportToFLL();
            void onMenuImportFromFCL();
            void onMenuExportToFCL();
            void onMenuImportFromFIS();
            void onMenuExportToFIS();
            void onMenuExportToCpp();
            void onMenuExportToJava();
            void onMenuExportToDatasetView();
            void onMenuExportToDatasetFile();

            void onMenuAskForHelp();
            void onMenuJoinTheCommunity();
            void onMenuFollowOnTwitter();
            void onMenuCheckForUpdates();

            void onMenuQuit();
            
            void automaticUpdates();
            void updatesReplyFinished(QNetworkReply* reply);
            void automaticUpdatesReplyFinished(QNetworkReply* reply);
            

            void closeEvent(QCloseEvent* e);

        protected:
            void setupMenuAndToolbar();
            void connect();
            std::pair<std::string, bool> onReplyFinished(QNetworkReply* reply);

            QMenu* _recentFiles;
            QString _currentFile;
            bool _currentFileModified;
            void setCurrentFile(bool modified, const QString* filename = NULL);
            void updateRecentFiles();
            void addRecentFile(const QString& filepath);
            void updateWindowTitle();

            void reloadModel();
            void removeRules();
            void fixVariableDependencies();

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

            static QColor gradient(int x, const QColor& min, const QColor& max);
            static QFont typeWriterFont();

            static QString toHtmlEscaped(const QString& x);

            static void main(int argc, char** argv);
        };

        class QScrollAreaFilter : public QObject {
        public:

            bool eventFilter(QObject* object, QEvent* event) {
                (void) object;
                if (event->type() == QEvent::Wheel) {
                    event->accept();
                    return true;
                }
                return false;
            }
        };

        class VariableContextMenu : public QObject {
        protected:
            QListWidget* _sender;
            std::string _type;
        public:

            VariableContextMenu(QListWidget* sender, const std::string& type) :
            _sender(sender), _type(type) {
            }

            bool eventFilter(QObject* object, QEvent* event) {
                (void) object;
                if (event->type() == QEvent::ContextMenu) {
                    std::vector<fl::Variable*> selectedVariables;
                    Engine* engine = Model::Default()->engine();
                    for (int i = 0; i < _sender->count(); ++i) {
                        if (_sender->item(i)->isSelected()) {
                            if (_type == "input")
                                selectedVariables.push_back(engine->getInputVariable(i));
                            else if (_type == "output")
                                selectedVariables.push_back(engine->getOutputVariable(i));
                            else
                                throw fl::Exception("[internal error] unrecognized variable type", FL_AT);
                        }
                    }

                    if (selectedVariables.size() > 0) {

                        QMenu menu(_sender);
                        for (std::size_t i = 0; i < selectedVariables.size(); ++i) {
                            Viewer* viewer = new Viewer(&menu);
                            viewer->setup(selectedVariables.at(i));
                            viewer->ui->mainLayout->setContentsMargins(3, 0, 3, 0);
                            QWidgetAction* action = new QWidgetAction(&menu);
                            action->setDefaultWidget(viewer);

                            menu.addAction(action);
                        }
                        menu.exec(QCursor::pos() + QPoint(1, 0));
                    }
                    event->accept();
                    return true;
                }
                return false;
            }
        };

    }
}
#endif /* FLQT_WINDOW_H_ */
