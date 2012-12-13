/*
 * InputVariable.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Variable.h"
#include "fl/qt/Term.h"
#include <fl/Headers.h>

#include <QtGui/QMessageBox>

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

            QObject::connect(ui->btn_term_up, SIGNAL(clicked()),
                    this, SLOT(onClickMoveUp()));
            QObject::connect(ui->btn_term_down, SIGNAL(clicked()),
                    this, SLOT(onClickMoveDown()));

            QObject::connect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
            QObject::connect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
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
                reloadModel();
//                ui->lvw_terms->addItem(QString::fromStdString(term->getName()));
                ui->lvw_terms->setFocus();
                ui->lvw_terms->item(ui->lvw_terms->count() - 1)->setSelected(true);
            }
        }

        void Variable::onClickRemoveTerm() {
            std::ostringstream message;
            message << "Do you want to delete the following terms?"
                    << std::endl;

            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    message << "<" << variable->getTerm(i)->getName() << ">: "
                            << variable->getTerm(i)->toString() << std::endl;
                }
            }

            QMessageBox::StandardButton clicked = QMessageBox::warning(this, "Delete term",
                    QString::fromStdString(message.str()),
                    QMessageBox::Yes | QMessageBox::No);
            if (clicked == QMessageBox::Yes) {
                FL_LOG("The answer was: " << "yes");
                for (int i = ui->lvw_terms->count() - 1; i >= 0; --i) {
                    if (ui->lvw_terms->item(i)->isSelected()) {
                        delete (variable->removeTerm(i));
                    }
                }
                reloadModel();
            }

        }
        void Variable::onClickEditTerm() {

        }

        void Variable::onSelectTerm() {
            ui->btn_edit_term->setEnabled(
                    ui->lvw_terms->selectedItems().size() == 1);
            ui->btn_remove_term->setEnabled(
                    ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_term_down->setEnabled(ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_term_up->setEnabled(ui->lvw_terms->selectedItems().size() > 0);

            ui->canvas->clear();
            ui->canvas->draw(variable);
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    ui->canvas->draw(variable->getTerm(i));
                }
            }

            ui->led_lvar_min->setText(QString::number(variable->minimum()));
            ui->led_lvar_max->setText(QString::number(variable->maximum()));
        }

        void Variable::onClickMoveUp() {
            std::vector<int> newPositions;
            bool rotate = false;
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    int newPosition = i - 1 + rotate;
                    if (newPosition < 0) {
                        newPosition = ui->lvw_terms->count() - 1;
                        rotate = true;
                    }
                    newPositions.push_back(newPosition);
                    FL_LOG(i << "->" << newPosition);
                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
                    for (int x = 0 ; x < variable->numberOfTerms(); ++x){
                        FL_LOG(variable->getTerm(x)->getName());
                    }
                }
            }
            reloadModel();
            for (std::size_t i = 0; i < newPositions.size(); ++i) {
                ui->lvw_terms->item(newPositions[i])->setSelected(true);
            }
        }

        void Variable::onClickMoveDown() {
            std::vector<int> newPositions;
            for (int i = ui->lvw_terms->count() - 1; i >= 0; --i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    int newPosition = i + 1;
                    if (newPosition >= ui->lvw_terms->count()) newPosition = 0;
                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
                    newPositions.push_back(newPosition);
                }
            }
            reloadModel();
            for (std::size_t i = 0; i < newPositions.size(); ++i) {
                ui->lvw_terms->item(newPositions[i])->setSelected(true);
            }
        }

        void Variable::reloadModel() {
            ui->lvw_terms->clear();
            for (int i = 0; i < variable->numberOfTerms(); ++i) {
                ui->lvw_terms->addItem(QString::fromStdString(variable->getTerm(i)->getName()));
            }
            ui->canvas->clear();
            if (variable->numberOfTerms() > 0) {
                ui->canvas->draw(variable);
            }
        }

        void Variable::refresh() {

        }

    } /* namespace qt */
} /* namespace fl */
