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


            void onSelectTestRule(int selected);
            void onSelectTestActivation(int selected);

            //MenuBar
            void onMenuConfiguration();
            void onMenuTerms();

            void onMenuImport();
            void onMenuExport();
            void onMenuAbout();
            void onMenuQuit();

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
