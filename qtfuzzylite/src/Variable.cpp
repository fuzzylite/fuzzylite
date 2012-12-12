/*
 * InputVariable.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Variable.h"
#include "fl/qt/Term.h"
#include <fl/Headers.h>

namespace fl {
    namespace qt {

        Variable::Variable(QWidget* parent, Qt::WindowFlags f)
                : QDialog(parent, f), ui(new Ui::Variable), variable(NULL) {
        }

        Variable::~Variable() {
            disconnect();
            delete ui;
        }

        void Variable::setup(VariableType type) {
            ui->setupUi(this);
            ui->lbl_default->setVisible(type == OUTPUT_VARIABLE);
            ui->led_default->setVisible(type == OUTPUT_VARIABLE);
            ui->chx_lock->setVisible(type == OUTPUT_VARIABLE);

            if (type == INPUT_VARIABLE)
                variable = new InputVariable;
            else if (type == OUTPUT_VARIABLE)
                variable = new OutputVariable;

            connect();

        }

        void Variable::connect() {
            QObject::connect(ui->btn_add_term, SIGNAL(clicked()),
                    this, SLOT(onClickAddTerm()));
            QObject::connect(ui->btn_edit_term, SIGNAL(clicked()),
                    this, SLOT(onClickEditTerm()));
            QObject::connect(ui->btn_remove_term, SIGNAL(clicked()),
                    this, SLOT(onClickRemoveTerm()));
        }

        void Variable::disconnect() {

        }

        /**
         * Button actions...
         */

        void Variable::onClickAddTerm() {
            Term* window = new Term;
            window->setup();
            int result = window->exec();
            if (result) {
                fl::Term* term = window->getSelectedTerm();
                term->setName(window->ui->led_name->text().toStdString());
                variable->addTerm(term);
                refresh();
                ui->lvw_terms->item(ui->lvw_terms->count() - 1)->setSelected(true);
            }
        }
        void Variable::onClickRemoveTerm() {

        }
        void Variable::onClickEditTerm() {

        }

        void Variable::refresh() {
            ui->lvw_terms->clear();
            for (int i = 0; i < variable->numberOfTerms(); ++i) {
                ui->lvw_terms->addItem(QString::fromStdString(variable->getTerm(i)->getName()));
            }

        }

    } /* namespace qt */
} /* namespace fl */
