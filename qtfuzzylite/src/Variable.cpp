/*
 * InputVariable.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Variable.h"
#include "fl/qt/Term.h"
#include "fl/qt/Wizard.h"
#include <fl/Headers.h>

#include <QtGui/QMessageBox>
#include <QtGui/QDesktopWidget>
namespace fl {
    namespace qt {

        Variable::Variable(QWidget* parent, Qt::WindowFlags f)
                : QDialog(parent, f),
                  ui(new Ui::Variable), variable(NULL) {
            setWindowFlags(Qt::Tool);
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

//            layout()->s/etSizeConstraint(QLayout::SetFixedSize);
//            adjustSize();
//            setFixedSize(sizeHint());
            QRect scr = QApplication::desktop()->screenGeometry();
            move(scr.center() - rect().center());
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

            QObject::connect(ui->btn_wizard, SIGNAL(clicked()),
                    this, SLOT(onClickWizard()));
        }

        void Variable::disconnect() {

        }

        void Variable::showEvent(QShowEvent* event) {
            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());
            ui->canvas->fitInView(0, 0, ui->canvas->scene()->width(),
                    ui->canvas->scene()->height(), Qt::IgnoreAspectRatio);
            QWidget::showEvent(event);
        }


        /**
         * Button actions...
         */

        void Variable::onClickWizard(){
            Wizard* window=  new Wizard(this);
            window->setup();
            int result = window->exec();
            FL_LOG("The answer was " << result);

        }

        void Variable::onClickAddTerm() {
            Term* window = new Term;
            window->setup();
            int result = window->exec();
            if (result) {
                fl::Term* term = window->copySelectedTerm();
                term->setName(window->ui->led_name->text().toStdString());
                variable->addTerm(term);
                reloadModel();
                ui->lvw_terms->setFocus();
                ui->lvw_terms->item(ui->lvw_terms->count() - 1)->setSelected(true);
            }
            delete window;
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
            std::vector<int> selected;
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    selected.push_back(i);
                    Term* window = new Term;
                    window->setup();
                    window->edit(variable->getTerm(i));
                    int result = window->exec();
                    if (result) {
                        fl::Term* term = window->copySelectedTerm();
                        term->setName(window->ui->led_name->text().toStdString());
                        delete variable->removeTerm(i);
                        variable->insertTerm(term, i);
                    }
                    delete window;
                }
            }
            reloadModel();
            for (std::size_t i = 0; i < selected.size(); ++i) {
                ui->lvw_terms->item(selected[i])->setSelected(true);
            }
        }

        void Variable::onSelectTerm() {
            ui->btn_edit_term->setEnabled(
                    ui->lvw_terms->selectedItems().size() > 0);
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
                        newPositions.push_back(newPosition);
                        rotate = true;
                    } else newPositions.push_back(newPosition - rotate);

//                    FL_LOG(i << "->" << newPosition);
                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
//                    for (int x = 0; x < variable->numberOfTerms(); ++x) {
//                        FL_LOG(variable->getTerm(x)->getName());
//                    }
                }
            }
            reloadModel();
            for (std::size_t i = 0; i < newPositions.size(); ++i) {
                ui->lvw_terms->item(newPositions[i])->setSelected(true);
            }
        }

        void Variable::onClickMoveDown() {
            std::vector<int> newPositions;
            bool rotate = false;
            for (int i = ui->lvw_terms->count() - 1; i >= 0; --i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    int newPosition = i + 1 - rotate;
                    if (newPosition >= ui->lvw_terms->count()) {
                        newPosition = 0;
                        newPositions.push_back(newPosition);
                        rotate = true;
                    } else newPositions.push_back(newPosition + rotate);
//                    FL_LOG(i << "->" << newPosition);
                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
//                    for (int x = 0; x < variable->numberOfTerms(); ++x) {
//                        FL_LOG(variable->getTerm(x)->getName());
//                    }
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


    } /* namespace qt */
} /* namespace fl */
