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
        class Configuration;
        class Window: public QMainWindow {
        Q_OBJECT

        signals:
        void outputValueChanged();

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
            



            //Examples
            bool onMenuExample(const std::string& example);
            void onMenuExample1();
            void onMenuExample2();
            void onMenuExample3();
            void onMenuExample4();

            //MenuBar
            void onMenuConfiguration();
            void onMenuTerms();

            void onMenuImport();
            void onMenuExport();
            void onMenuReset();
            void onMenuAbout();
            void onMenuQuit();

//            //Examples
//            virtual void onMenuExample

        protected:
            std::vector<QWidget*> _inputs, _outputs;
            Configuration* _configurationWindow;
            void connect();
            void disconnect();

            void reloadModel();
            void removeRules();

            void fixDependencies();

            void reloadTest();
            void resetTest();

            void resizeEvent(QResizeEvent* e);
            void showEvent(QShowEvent* e);

        public:
            Ui::Window* ui;
            Window(QWidget* parent = NULL, Qt::WindowFlags flags = 0);
            ~Window();

            void setup();

            static void main();
        };
    }
} 
#endif /* FLQT_WINDOW_H_ */
