/*   Copyright 2010 Juan Rada-Vilela

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
/* 
 * File:   GuiMainWindow.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 1:34 PM
 */

#ifndef _GUIMAINWINDOW_H
#define	_GUIMAINWINDOW_H

#include <QtGui/QMainWindow>

#include "gui/ui_MainWindow.h"
#include "gui/GuiPreferences.h"
namespace fl_gui {

    class GuiMainWindow : public QMainWindow {
        Q_OBJECT
    protected slots:
        virtual void onShowPreferences();
        virtual void onClosePreferences(int result);
        virtual void onChangeInputSelection();
        virtual void onChangeOutputSelection();
        virtual void onDoubleClickInputItem(QListWidgetItem* item);
        virtual void onDoubleClickOutputItem(QListWidgetItem* item);

        virtual void onSelectTestRule(int selected);
        virtual void onSelectTestActivation(int selected);
    private:
        Ui::MainWindow* _ui;
        GuiPreferences* _preferences;
        QAction* _action_preferences;
    protected:
        virtual void connect();
        virtual void disconnect();
    public:
        GuiMainWindow(QWidget* parent = NULL, Qt::WindowFlags flags = 0);
        virtual ~GuiMainWindow();

        virtual Ui::MainWindow& getUi() const;

        virtual void setup();
    };
}

#endif	/* _GUIMAINWINDOW_H */

