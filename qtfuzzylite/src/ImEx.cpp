/* 
 * File:   ImEx.cpp
 * Author: jcrada
 *
 * Created on 6 July 2013, 9:09 PM
 */

#include "fl/qt/ImEx.h"

#include <QPushButton>
#include <QDialogButtonBox>

namespace fl {
    namespace qt {

        ImEx::ImEx(QWidget* parent, Qt::WindowFlags f) :
        QDialog(parent, f), ui(new Ui::ImEx) { }
        
        ImEx::~ImEx(){
            delete ui;
        }

        void ImEx::setup() {
            ui->setupUi(this);
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            QObject::connect(ui->pte_code, SIGNAL(textChanged()), 
                    this, SLOT(onTextChanged()));
        }
        
        void ImEx::onTextChanged(){
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(not 
            ui->pte_code->toPlainText().trimmed().isEmpty());
        }
        

    }
}