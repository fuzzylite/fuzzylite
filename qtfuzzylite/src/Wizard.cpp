/*
 * Wizard.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Wizard.h"

#include <QtGui/QMessageBox>

namespace fl {
    namespace qt {

        Wizard::Wizard(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f), ui(new Ui::Wizard) {
            setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowSystemMenuHint);
        }

        Wizard::~Wizard() {
            disconnect();
            delete ui;
        }

        void Wizard::setup(const std::string& prefix) {
            this->prefix = prefix;
            ui->setupUi(this);
            QRect scr = parentWidget()->geometry();
            move(scr.center() - rect().center());
            
            autoGenerate(ui->sbx_copies->value());
            
            connect();
        }

        void Wizard::connect() {
            QObject::connect(ui->sbx_copies, SIGNAL(valueChanged(int)),
                    this, SLOT(autoGenerate(int)));
        }

        void Wizard::disconnect() {
            QObject::connect(ui->sbx_copies, SIGNAL(valueChanged(int)),
                    this, SLOT(autoGenerate(int)));
        }

        void Wizard::autoGenerate(int copies) {
            if (not ui->chx_auto_generate->isChecked()) return;
            
            ui->ptx_terms->clear();
            char name = 'A';
            std::string separator = prefix.empty() ? "" : ".";
            for (int i = 0; i < copies; ++i) {
                ui->ptx_terms->appendPlainText(
                        QString::fromStdString(prefix + separator) + name++);
            }
        }
    }
}
