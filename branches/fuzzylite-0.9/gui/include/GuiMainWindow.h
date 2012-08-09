/* 
 * File:   GuiMainWindow.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 1:34 PM
 */

#ifndef _GUIMAINWINDOW_H
#define	_GUIMAINWINDOW_H

#include <QtGui/QMainWindow>

#include "ui_MainWindow.h"
#include "GuiPreferences.h"
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

