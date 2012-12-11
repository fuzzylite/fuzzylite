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

namespace fl {
    namespace qt {
        class Configuration;

        class Window: public QMainWindow {
        Q_OBJECT

        protected slots:
            virtual void onChangeInputSelection();
            virtual void onChangeOutputSelection();
            virtual void onDoubleClickInputItem(QListWidgetItem* item);
            virtual void onDoubleClickOutputItem(QListWidgetItem* item);

            virtual void onSelectTestRule(int selected);
            virtual void onSelectTestActivation(int selected);

            //MenuBar
            virtual void onMenuConfiguration();
            virtual void onMenuImport();
            virtual void onMenuExport();
            virtual void onMenuAbout();
            virtual void onMenuQuit();

//            //Examples
//            virtual void onMenuExample

        protected:
            Configuration* _configuration;

            virtual void connect();
            virtual void disconnect();

        public:
            Ui::Window* ui;
            Window(QWidget* parent = NULL, Qt::WindowFlags flags = 0);
            virtual ~Window();

            virtual void setup();

            static void main();
        };
    }
} /* namespace fl */
#endif /* FLQT_WINDOW_H_ */
