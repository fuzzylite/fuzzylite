/*
 * Wizard.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "../fl/qt/Wizard.h"

#include <QtGui/QDesktopWidget>


namespace fl {
    namespace qt {

        Wizard::Wizard(QWidget* parent, Qt::WindowFlags f)
                : QDialog(parent, f), ui(new Ui::Wizard) {
            setWindowFlags(Qt::Tool);
        }

        Wizard::~Wizard() {
            disconnect();
            delete ui;
        }

        void Wizard::setup() {
            ui->setupUi(this);
            QRect scr = QApplication::desktop()->screenGeometry();
            move(scr.center() - rect().center());

            connect();
        }

        void Wizard::connect() {
            QObject::connect(ui->btn_create_labels, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()));
        }

        void Wizard::disconnect() {
            QObject::disconnect(ui->btn_create_labels, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()));
        }

        void Wizard::onClickGenerate(){

        }

        void Wizard::reject(){
            FL_LOG("On reject");
            for (std::size_t i = 0; i < terms.size(); ++i){
                delete terms[i];
            }
            close();
        }



    } /* namespace qt */
} /* namespace fl */
