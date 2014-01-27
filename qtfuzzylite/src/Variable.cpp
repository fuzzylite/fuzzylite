/**
    This file is part of qtfuzzylite.

    qtfuzzylite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qtfuzzylite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qtfuzzylite.  If not, see <http://www.gnu.org/licenses/>.

    Juan Rada-Vilela, 01 February 2013
    jcrada@fuzzylite.com
 **/

/*
 * InputVariable.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Variable.h"
#include "fl/qt/Term.h"
#include "fl/qt/Wizard.h"
#include "fl/qt/Viewer.h"
#include "fl/qt/Window.h"
#include "fl/qt/qtfuzzylite.h"

#include <QMessageBox>
#include <QListWidgetItem>
#include <QSplitter>

namespace fl {
    namespace qt {

        Variable::Variable(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f), ui(new Ui::Variable), viewer(new Viewer),
        variable(NULL) {
            setWindowFlags(Qt::Dialog
                    | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowStaysOnTopHint);
        }

        Variable::~Variable() {
            delete ui;
        }

        void Variable::setup(VariableType type) {
            if (type == INPUT_VARIABLE)
                variable = new InputVariable("", 0, 1);
            else if (type == OUTPUT_VARIABLE)
                variable = new OutputVariable("", 0, 1);
            ui->setupUi(this);

            ui->sbx_min->setSingleStep(0.01);
            ui->sbx_max->setSingleStep(0.01);

            ui->sbx_min->setDecimals(fuzzylite::decimals());
            ui->sbx_max->setDecimals(fuzzylite::decimals());

            QList<int> sizes;
            sizes << .75 * size().width() << .25 * size().width();
            ui->splitter2->setSizes(sizes);

            viewer->setup(variable);
            viewer->enablePropertiesButton(false);
            ui->splitter->addWidget(viewer);
            setWindowTitle("Add variable");

            ui->gbx_output->setVisible(type == OUTPUT_VARIABLE);

            ui->cbx_enabled->setChecked(true);

            std::vector<std::string> accumulation = FactoryManager::instance()->snorm()->available();
            ui->cbx_accumulation->addItem("");
            for (std::size_t i = 0; i < accumulation.size(); ++i) {
                ui->cbx_accumulation->addItem(QString::fromStdString(
                        accumulation.at(i)));
            }

            std::vector<std::string> defuzzifiers = FactoryManager::instance()->defuzzifier()->available();
            ui->cbx_defuzzifier->addItem("");
            for (std::size_t i = 0; i < defuzzifiers.size(); ++i) {
                ui->cbx_defuzzifier->addItem(QString::fromStdString(
                        defuzzifiers.at(i)));
            }
            int indexOfCentroid = ui->cbx_defuzzifier->findText(
                    QString::fromStdString(fl::Centroid().className()));
            ui->cbx_defuzzifier->setCurrentIndex(indexOfCentroid);
            ui->cbx_defuzzifier->insertSeparator(ui->cbx_defuzzifier->findText(
                    QString::fromStdString(fl::WeightedAverage().className())));

            ui->sbx_accuracy->setValue(fl::IntegralDefuzzifier::defaultResolution());

            QRect scr = Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());

            connect();

        }

        void Variable::connect() {
            QObject::connect(ui->btn_add_term, SIGNAL(clicked()),
                    this, SLOT(onClickAddTerm()));
            QObject::connect(ui->btn_edit_term, SIGNAL(clicked()),
                    this, SLOT(onClickEditTerm()));
            QObject::connect(ui->btn_remove_term, SIGNAL(clicked()),
                    this, SLOT(onClickRemoveTerm()));

            QObject::connect(ui->btn_sort_centroid, SIGNAL(clicked()),
                    this, SLOT(reloadModel()));
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
            QObject::connect(ui->lvw_terms, SIGNAL(itemClicked(QListWidgetItem*)),
                    this, SLOT(onClickTerm(QListWidgetItem*)));

            QObject::connect(ui->cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onSelectDefuzzifier(int)));

            QObject::connect(ui->sbx_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeMinRange(double)));
            QObject::connect(ui->sbx_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeMaxRange(double)));

            QObject::connect(viewer, SIGNAL(valueChanged(double)),
                    this, SLOT(showSelectedTerms()), Qt::QueuedConnection);
        }

        void Variable::showEvent(QShowEvent* event) {
            //            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());
            //            ui->canvas->fitInView(0, 0, ui->canvas->scene()->width(),
            //                    ui->canvas->scene()->height(), Qt::IgnoreAspectRatio);
            reloadModel();
            QWidget::showEvent(event);

        }

        void Variable::edit(const InputVariable* inputVariable) {
            variable->setMinimum(inputVariable->getMinimum());
            variable->setMaximum(inputVariable->getMaximum());
            for (int i = 0; i < inputVariable->numberOfTerms(); ++i) {
                this->variable->addTerm(inputVariable->getTerm(i)->copy());
            }
            scalar singleStep = (variable->getMaximum() - variable->getMinimum()) / 100;
            ui->sbx_min->setSingleStep(std::max(scalar(0.01), singleStep));
            ui->sbx_max->setSingleStep(std::max(scalar(0.01), singleStep));

            setWindowTitle("Edit variable");
            ui->led_name->setText(QString::fromStdString(inputVariable->getName()));
            ui->cbx_enabled->setChecked(inputVariable->isEnabled());
            reloadModel();
        }

        void Variable::edit(const OutputVariable* outputVariable) {
            variable->setMinimum(outputVariable->getMinimum());
            variable->setMaximum(outputVariable->getMaximum());
            for (int i = 0; i < outputVariable->numberOfTerms(); ++i) {
                this->variable->addTerm(outputVariable->getTerm(i)->copy());
            }

            scalar singleStep = (variable->getMaximum() - variable->getMinimum()) / 100;
            ui->sbx_min->setSingleStep(std::max(scalar(0.01), singleStep));
            ui->sbx_max->setSingleStep(std::max(scalar(0.01), singleStep));

            setWindowTitle("Edit variable");
            ui->led_name->setText(QString::fromStdString(outputVariable->getName()));
            ui->cbx_enabled->setChecked(outputVariable->isEnabled());

            OutputVariable* editable = dynamic_cast<OutputVariable*> (this->variable);
            editable->setDefaultValue(outputVariable->getDefaultValue());
            editable->setLastValidOutput(fl::nan);
            editable->setLockValidOutput(outputVariable->isLockingValidOutput());
            editable->setLockOutputRange(outputVariable->isLockingOutputRange());

            editable->fuzzyOutput()->setAccumulation(outputVariable->fuzzyOutput()->getAccumulation());
            Defuzzifier* defuzzifier = outputVariable->getDefuzzifier();
            //            if (not defuzzifier) {
            //                defuzzifier = Factory::instance()->defuzzifier()->
            //                        create(Centroid().className(), fl::fuzzylite::defaultDivisions());
            //            }
            editable->setDefuzzifier(defuzzifier);

            reloadModel();
        }

        void Variable::redraw() {
            viewer->refresh();
            showSelectedTerms();
        }

        void Variable::showSelectedTerms() {
            //            ui->ptx_terms->clear();
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    viewer->draw(variable->getTerm(i));
                    //                    ui->ptx_terms->appendPlainText(QString::fromStdString(
                    //                            variable->getTerm(i)->toString()));
                }
            }
        }

        /**
         * Button actions...
         */

        void Variable::accept() {
            OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (variable);
            if (outputVariable) {
                try {
                    outputVariable->setDefaultValue(
                            fl::Op::toScalar(ui->led_default->text().toStdString()));
                } catch (fl::Exception& ex) {
                    std::ostringstream message;
                    message << ex.what() << std::endl <<
                            "Expected numeric values (including nan or inf)" << std::endl;
                    QMessageBox::critical(this, tr("Error"),
                            fl::qt::Window::toHtmlEscaped(
                            QString::fromStdString(message.str())),
                            QMessageBox::Ok);
                    return;
                }
                outputVariable->setLockValidOutput(ui->chx_lock_valid->isChecked());
                outputVariable->setLockOutputRange(ui->chx_lock_range->isChecked());

                SNorm* accumulation = FactoryManager::instance()->snorm()->createInstance(
                        ui->cbx_accumulation->currentText().toStdString());
                outputVariable->fuzzyOutput()->setAccumulation(accumulation);
                Defuzzifier* defuzzifier = NULL;
                if (ui->cbx_defuzzifier->currentIndex() >= 0) {
                    defuzzifier = FactoryManager::instance()->defuzzifier()->createInstance(
                            ui->cbx_defuzzifier->currentText().toStdString());
                    IntegralDefuzzifier* integralDefuzzifier =
                            dynamic_cast<IntegralDefuzzifier*> (defuzzifier);
                    if (integralDefuzzifier) {
                        integralDefuzzifier->setResolution(ui->sbx_accuracy->value());
                    }
                }
                outputVariable->setDefuzzifier(defuzzifier);

            }
            variable->setName(fl::Op::makeValidId(ui->led_name->text().toStdString()));
            variable->setEnabled(ui->cbx_enabled->isChecked());
            QDialog::accept();
        }

        void Variable::reject() {
            delete variable;
            variable = NULL;
            QDialog::reject();
        }

        void Variable::onChangeMinRange(double) {
            if (fl::Op::isGE(ui->sbx_min->value(), ui->sbx_max->value())) {
                ui->sbx_max->setValue(ui->sbx_min->value() + .1);
            }
            variable->setMinimum(ui->sbx_min->value());

            redraw();
        }

        void Variable::onChangeMaxRange(double) {
            if (fl::Op::isLE(ui->sbx_max->value(), ui->sbx_min->value())) {
                ui->sbx_min->setValue(ui->sbx_max->value() - .1);
            }
            variable->setMaximum(ui->sbx_max->value());

            redraw();
        }

        void Variable::onClickAddTerm() {
            Term* window = new Term(this);
            window->setup(*variable);
            if (window->exec()) {
                for (int i = variable->numberOfTerms() - 1; i >= 0; --i) {
                    delete variable->removeTerm(i);
                }

                for (int i = 0; i < window->dummyVariable->numberOfTerms(); ++i) {
                    variable->addTerm(window->dummyVariable->getTerm(i)->copy());
                }
                reloadModel();
            }
            delete window;
        }

        void Variable::onClickRemoveTerm() {
            std::ostringstream message;
            message << "Do you want to delete the following terms?\n\n";

            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    message << "- " << variable->getTerm(i)->toString();
                    if (i + 1 < ui->lvw_terms->selectedItems().size()) message << "\n\n";
                }
            }

            QMessageBox::StandardButton clicked = QMessageBox::warning(this, "Delete term",
                    QString::fromStdString(message.str()),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
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
                message << "Do you want to edit the following terms?\n\n";
                for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                    if (ui->lvw_terms->item(i)->isSelected()) {
                        message << "- " << variable->getTerm(i)->toString();
                        if (i + 1 < ui->lvw_terms->selectedItems().size()) message << "\n\n";
                    }
                }
                QMessageBox::StandardButton clicked = QMessageBox::information(this,
                        "Edit multiple terms",
                        QString::fromStdString(message.str()),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);
                if (clicked == QMessageBox::No) {
                    return;
                }
            }

            bool reload = false;
            for (int i = 0; i < ui->lvw_terms->count(); ++i) {
                if (ui->lvw_terms->item(i)->isSelected()) {
                    Term* window = new Term(this);
                    window->setup(*variable, variable->getTerm(i));
                    if (window->exec()) {
                        reload |= true;
                        for (int t = variable->numberOfTerms() - 1; t >= 0; --t) {
                            delete variable->removeTerm(t);
                        }
                        for (int t = 0; t < window->dummyVariable->numberOfTerms(); ++t) {
                            variable->addTerm(window->dummyVariable->getTerm(t)->copy());
                        }
                    }
                    delete window;
                }
            }
            if (reload) reloadModel();
        }

        void Variable::onSelectTerm() {
            ui->btn_edit_term->setEnabled(
                    ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_remove_term->setEnabled(
                    ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_term_down->setEnabled(not ui->btn_sort_centroid->isChecked()
                    and ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_term_up->setEnabled(not ui->btn_sort_centroid->isChecked()
                    and ui->lvw_terms->selectedItems().size() > 0);
            redraw();
        }

        void Variable::onDoubleClickTerm(QListWidgetItem* item) {
            if (item) {
                onClickEditTerm();
            }
        }

        void Variable::onClickTerm(QListWidgetItem*) {
            redraw();
        }

        void Variable::onSelectDefuzzifier(int index) {
            (void) index;
            std::string defuzzifier = ui->cbx_defuzzifier->currentText().toStdString();
            if (defuzzifier == fl::WeightedAverage().className() or
                    defuzzifier == fl::WeightedSum().className()) {
                ui->sbx_accuracy->setEnabled(false);
                //                ui->cbx_accumulation->setEnabled(false);
                //                fl::OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (variable);
                //                outputVariable->output()->setAccumulation(NULL);
                //                ui->cbx_accumulation->setCurrentIndex(-1);
            } else {
                ui->sbx_accuracy->setEnabled(true);
                //                ui->cbx_accumulation->setEnabled(true);
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
                    } else {
                        newPositions.push_back(newPosition - rotate);
                    }
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
                    } else {
                        newPositions.push_back(newPosition + rotate);
                    }
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
            if (ui->btn_sort_centroid->isChecked()) {
                variable->sort();
            }

            ui->lvw_terms->clear();
            for (int i = 0; i < variable->numberOfTerms(); ++i) {
                fl::Term* term = variable->getTerm(i);
                std::string text = term->getName() + " " + term->className()
                        + " " + term->parameters();
                QListWidgetItem* item = new QListWidgetItem;
                item->setText(QString::fromStdString(text));
                item->setToolTip(QString::fromStdString(text));
                ui->lvw_terms->addItem(item);
            }

            OutputVariable* outputVariable = dynamic_cast<OutputVariable*> (variable);
            if (outputVariable) {
                ui->led_default->setText(QString::number(outputVariable->getDefaultValue()));
                ui->chx_lock_range->setChecked(outputVariable->isLockingOutputRange());
                ui->chx_lock_valid->setChecked(outputVariable->isLockingValidOutput());
                if (outputVariable->fuzzyOutput()->getAccumulation()) {
                    ui->cbx_accumulation->setCurrentIndex(
                            ui->cbx_accumulation->findText(
                            QString::fromStdString(
                            outputVariable->fuzzyOutput()->getAccumulation()->className())));
                } else {
                    ui->cbx_accumulation->setCurrentIndex(-1);
                }
                if (outputVariable->getDefuzzifier()) {
                    ui->cbx_defuzzifier->setCurrentIndex(
                            ui->cbx_defuzzifier->findText(
                            QString::fromStdString(
                            outputVariable->getDefuzzifier()->className())));
                    IntegralDefuzzifier* integralDefuzzifier =
                            dynamic_cast<IntegralDefuzzifier*> (outputVariable->getDefuzzifier());
                    if (integralDefuzzifier) {
                        ui->sbx_accuracy->setValue(integralDefuzzifier->getResolution());
                    }
                } else {
                    ui->cbx_defuzzifier->setCurrentIndex(-1);
                }
            }
            scalar minimum = variable->getMinimum();
            scalar maximum = variable->getMaximum();
            ui->sbx_min->setValue(minimum);
            ui->sbx_max->setValue(maximum);

            ui->btn_term_down->setEnabled(not ui->btn_sort_centroid->isChecked()
                    and ui->lvw_terms->selectedItems().size() > 0);
            ui->btn_term_up->setEnabled(not ui->btn_sort_centroid->isChecked()
                    and ui->lvw_terms->selectedItems().size() > 0);

            redraw();

        }

    }
}
