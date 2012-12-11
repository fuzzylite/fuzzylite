/*
 * Window.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Window.h"
#include "fl/qt/Configuration.h"
#include "fl/qt/Term.h"

#include <QtGui/QListWidgetItem>
#include <QtGui/QScrollBar>
#include <QtGui/QMessageBox>

namespace fl {
    namespace qt {
        Window::Window(QWidget* parent, Qt::WindowFlags flags)
                : QMainWindow(parent, flags),
                  _configuration(new Configuration),
                  ui(new Ui::Window) {

        }

        Window::~Window() {
            disconnect();
            delete _configuration;
            delete ui;
        }

        void Window::setup() {
            ui->setupUi(this);
            _configuration->setWindowFlags(_configuration->windowFlags()
                    | Qt::WindowCloseButtonHint
                    | Qt::CustomizeWindowHint);
            _configuration->setFixedSize(320, 270);
            _configuration->setup();
            ui->tab_container->setCurrentIndex(0);

            connect();
        }

        void Window::connect() {
            QObject::connect(ui->lvw_inputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeInputSelection()));
            QObject::connect(ui->lvw_outputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeOutputSelection()));
            QObject::connect(ui->lvw_inputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickInputItem(QListWidgetItem*)));
            QObject::connect(ui->lvw_outputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickOutputItem(QListWidgetItem*)));

            QObject::connect(ui->lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));
            QObject::connect(ui->lsw_test_rules_activation->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules->verticalScrollBar(), SLOT(setValue(int)));

            QObject::connect(ui->actionConfiguration, SIGNAL(triggered()),
                    this, SLOT(onMenuConfiguration()));
            QObject::connect(ui->actionTerms, SIGNAL(triggered()),
                                this, SLOT(onMenuTerms()));
            QObject::connect(ui->actionImport, SIGNAL(triggered()),
                    this, SLOT(onMenuImport()));
            QObject::connect(ui->actionExport, SIGNAL(triggered()),
                    this, SLOT(onMenuExport()));
            QObject::connect(ui->actionAbout, SIGNAL(triggered()),
                    this, SLOT(onMenuAbout()));
            QObject::connect(ui->actionQuit, SIGNAL(triggered()),
                    this, SLOT(onMenuQuit()));

        }

        void Window::disconnect() {
            QObject::disconnect(ui->lvw_inputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeInputSelection()));
            QObject::disconnect(ui->lvw_outputs, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onChangeOutputSelection()));
            QObject::disconnect(ui->lvw_inputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickInputItem(QListWidgetItem*)));
            QObject::disconnect(ui->lvw_outputs, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickOutputItem(QListWidgetItem*)));

            QObject::disconnect(ui->lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));
            QObject::disconnect(ui->lsw_test_rules->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lsw_test_rules_activation->verticalScrollBar(), SLOT(setValue(int)));

        }

        void Window::onChangeInputSelection() {
            ui->btn_remove_input->setEnabled(
                    ui->lvw_inputs->selectedItems().size() > 0);
            ui->btn_edit_input->setEnabled(
                    ui->lvw_inputs->selectedItems().size() > 0);
        }

        void Window::onChangeOutputSelection() {
            ui->btn_remove_output->setEnabled(
                    ui->lvw_outputs->selectedItems().size() > 0);
            ui->btn_edit_output->setEnabled(
                    ui->lvw_outputs->selectedItems().size() > 0);
        }

        void Window::onDoubleClickInputItem(QListWidgetItem* item) {
            (void) item;
            ui->btn_edit_input->click();
        }

        void Window::onDoubleClickOutputItem(QListWidgetItem* item) {
            (void) item;
            ui->btn_edit_output->click();
        }

        void Window::onSelectTestRule(int selected) {
            ui->lsw_test_rules_activation->item(selected)->setSelected(true);
        }

        void Window::onSelectTestActivation(int selected) {
            ui->lsw_test_rules->item(selected)->setSelected(true);
        }

        void Window::onMenuConfiguration() {
            this->_configuration->show();
        }

        void Window::onMenuTerms(){
            Term* termWindow  = new Term(this);
            termWindow->setModal(false);
            termWindow->setup();
            termWindow->exec();
            delete termWindow;

        }
        void Window::onMenuImport() {

        }
        void Window::onMenuExport() {

        }
        void Window::onMenuAbout() {
            std::ostringstream message;
            message << "qtfuzzylite v." << FL_VERSION <<
                    " (" << FL_DATE <<")" <<  std::endl;
            message << "http://code.google.com/p/fuzzylite" << std::endl << std::endl;
            message << "Developed by Juan Rada-Vilela." << std::endl;
            message << "jcrada@gmail.com" << std::endl;
            QMessageBox::about(this, "qtfuzzylite",
                    QString::fromStdString(message.str()));
        }
        void Window::onMenuQuit() {
            int result = QMessageBox::question(this, tr("qtfuzzylite"),
                    tr("Do you want to quit?"),
                    QMessageBox::Yes | QMessageBox::No);
            if (result == QMessageBox::Yes) {
                this->close();
            } else {
                // do nothing
            }
        }
        void Window::main() {
            Window* w = new Window;
            w->setup();
            w->show();
        }

    }
} /* namespace fl */
