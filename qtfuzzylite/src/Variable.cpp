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
#include <QtGui/QListWidgetItem>
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
            setWindowTitle("Add variable");

            if (type == INPUT_VARIABLE)
                variable = new InputVariable;
            else if (type == OUTPUT_VARIABLE)
                variable = new OutputVariable;

            ui->canvas->adjustSize();
            ui->lvw_terms->adjustSize();

            if (type != OUTPUT_VARIABLE) {
                QSize thisSize = this->size();
                thisSize.setHeight(thisSize.height() - 85);
                setFixedSize(thisSize);
            } else {
                setFixedSize(size());
            }

            ui->gbx_output->setVisible(type == OUTPUT_VARIABLE);

            QRect scr = parentWidget()->geometry();
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
            QObject::connect(ui->lvw_terms, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickTerm(QListWidgetItem*)));

            QObject::connect(ui->btn_wizard, SIGNAL(clicked()),
                    this, SLOT(onClickWizard()));
        }

        void Variable::disconnect() {
            QObject::disconnect(ui->btn_add_term, SIGNAL(clicked()),
                    this, SLOT(onClickAddTerm()));
            QObject::disconnect(ui->btn_edit_term, SIGNAL(clicked()),
                    this, SLOT(onClickEditTerm()));
            QObject::disconnect(ui->btn_remove_term, SIGNAL(clicked()),
                    this, SLOT(onClickRemoveTerm()));

            QObject::disconnect(ui->btn_term_up, SIGNAL(clicked()),
                    this, SLOT(onClickMoveUp()));
            QObject::disconnect(ui->btn_term_down, SIGNAL(clicked()),
                    this, SLOT(onClickMoveDown()));

            QObject::disconnect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
            QObject::disconnect(ui->lvw_terms, SIGNAL(itemSelectionChanged()),
                    this, SLOT(onSelectTerm()));
            QObject::disconnect(ui->lvw_terms, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                    this, SLOT(onDoubleClickTerm(QListWidgetItem*)));


            QObject::disconnect(ui->btn_wizard, SIGNAL(clicked()),
                    this, SLOT(onClickWizard()));
        }

        void Variable::showEvent(QShowEvent* event) {
            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());
            ui->canvas->fitInView(0, 0, ui->canvas->scene()->width(),
                    ui->canvas->scene()->height(), Qt::IgnoreAspectRatio);
            reloadModel();
            QWidget::showEvent(event);

        }

        void Variable::edit(const InputVariable* inputVariable) {
            setWindowTitle("Edit variable");
            ui->led_name->setText(QString::fromStdString(inputVariable->getName()));
            for (int i = 0; i < inputVariable->numberOfTerms(); ++i) {
                //                FL_LOG("Copying " << inputVariable->getTerm(i)->toString());
                fl::Term* copy = inputVariable->getTerm(i)->copy();
                //                FL_LOG("Copied: " << copy->toString());
                this->variable->addTerm(copy);
            }
            reloadModel();
        }

        void Variable::edit(const OutputVariable* outputVariable) {
            setWindowTitle("Edit variable");
            ui->led_name->setText(QString::fromStdString(outputVariable->getName()));
            for (int i = 0; i < outputVariable->numberOfTerms(); ++i) {
                this->variable->addTerm(outputVariable->getTerm(i)->copy());
            }
            OutputVariable* editable = dynamic_cast<OutputVariable*> (this->variable);
            editable->setDefaultValue(outputVariable->getDefaultValue());
            editable->setDefuzzifier(outputVariable->getDefuzzifier());

            editable->setDefuzzifiedValue(outputVariable->getDefuzzifiedValue());
            editable->setLockDefuzzifiedValue(outputVariable->lockDefuzzifiedValue());

            editable->setMininumOutputRange(outputVariable->getMinimumOutputRange());
            editable->setMaximumOutputRange(outputVariable->getMaximumOutputRange());
            reloadModel();
        }

        /**
         * Button actions...
         */

        void Variable::accept() {
            OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (variable);
            if (outputVariable) {
                try {
                    outputVariable->setDefaultValue(
                            fl::Op::Scalar(ui->led_default->text().toStdString()));
                    outputVariable->setMininumOutputRange(
                            fl::Op::Scalar(ui->led_min_range->text().toStdString()));
                    outputVariable->setMaximumOutputRange(
                            fl::Op::Scalar(ui->led_max_range->text().toStdString()));
                } catch (fl::Exception& ex) {
                    std::ostringstream message;
                    message << ex.what() << std::endl <<
                            "Expected numeric values (including nan or inf)" << std::endl;
                    QMessageBox::critical(this, tr("Error"),
                            QString::fromStdString(message.str()),
                            QMessageBox::Ok);
                    return;
                }
                outputVariable->setLockDefuzzifiedValue(ui->chx_lock->isChecked());
            }
            variable->setName(ui->led_name->text().toStdString());
            QDialog::accept();
        }

        void Variable::reject() {
            FL_LOG("Deleting variable " << variable->toString());
            delete variable;
            variable = NULL;
            QDialog::reject();
        }

        void Variable::onClickWizard() {
            Wizard* window = new Wizard(this);
            window->setup(ui->led_name->text().toStdString());
            if (window->exec()) {
                for (std::size_t i = 0; i < window->terms.size(); ++i) {
                    variable->addTerm(window->terms[i]);
                }
                reloadModel();
            }
        }

        void Variable::onClickAddTerm() {
            Term* window = new Term(this);
            window->setup();
            if (window->exec()) {
                variable->addTerm(window->copySelectedTerm());
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
                for (int i = ui->lvw_terms->count() - 1; i >= 0; --i) {
                    if (ui->lvw_terms->item(i)->isSelected()) {
                        delete (variable->removeTerm(i));
                    }
                }
                reloadModel();
            }

        }

        void Variable::onClickEditTerm() {
            if (ui->lvw_terms->selectedItems().size() > 1) {
                std::ostringstream message;
                message << "Do you want to edit the following terms?"
                        << std::endl;
                for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                    if (ui->lvw_terms->item(i)->isSelected()) {
                        message << "<" << variable->getTerm(i)->getName() << ">: "
                                << variable->getTerm(i)->toString() << std::endl;
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::warning(this,
                        "Edit multiple terms",
                        QString::fromStdString(message.str()),
                        QMessageBox::Yes | QMessageBox::No);
                if (clicked == QMessageBox::No) {
                    return;
                }
            }

            std::vector<int> selected;
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    selected.push_back(i);
                    Term* window = new Term(this);
                    window->setup();
                    window->edit(variable->getTerm(i));
                    if (window->exec()) {
                        fl::Term* term = window->copySelectedTerm();
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

            ui->ptx_terms->clear();
            ui->canvas->clear();
            ui->canvas->draw(variable);
            scalar minimum = std::numeric_limits<scalar>::infinity();
            scalar maximum = -std::numeric_limits<scalar>::infinity();
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    ui->canvas->draw(variable->getTerm(i));
                    ui->ptx_terms->appendPlainText(QString::fromStdString(
                            variable->getTerm(i)->toString()));
                    minimum = fl::Op::Min(minimum, variable->getTerm(i)->minimum());
                    maximum = fl::Op::Max(maximum, variable->getTerm(i)->maximum());
                }
            }
            ui->led_min->setText(QString::number(minimum, 'g', 2));
            ui->led_max->setText(QString::number(maximum, 'g', 2));

        }

        void Variable::onDoubleClickTerm(QListWidgetItem* item) {
            if (item) {
                onClickEditTerm();
            }
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
                    } else
                        newPositions.push_back(newPosition - rotate);

                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
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
                    } else
                        newPositions.push_back(newPosition + rotate);
                    fl::Term* term = variable->removeTerm(i);
                    variable->insertTerm(term, newPosition);
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
            ui->led_min->setText(QString::number(variable->minimum(), 'g', 2));
            ui->led_max->setText(QString::number(variable->maximum(), 'g', 2));
            OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (variable);
            if (outputVariable) {
                ui->led_default->setText(QString::number(outputVariable->getDefaultValue()));
                ui->chx_lock->setChecked(outputVariable->lockDefuzzifiedValue());
                std::ostringstream min, max;
                min << outputVariable->getMinimumOutputRange();
                max << outputVariable->getMaximumOutputRange();
                ui->led_min_range->setText(QString::fromStdString(min.str()));
                ui->led_max_range->setText(QString::fromStdString(max.str()));
            }

        }

    } /* namespace qt */
} /* namespace fl */
