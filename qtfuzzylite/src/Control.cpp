/*
 * Control.cpp
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Control.h"

#include <QtGui/QMessageBox>

#include "fl/qt/Model.h"
namespace fl {
    namespace qt {

        Control::Control(QWidget* parent, Qt::WindowFlags f) :
        QWidget(parent, f), ui(new Ui::Control) {
        }

        Control::~Control() {
            delete ui;
        }

        void Control::setup(fl::Variable* model) {
            inputVariable = dynamic_cast<InputVariable*> (model);
            outputVariable = dynamic_cast<OutputVariable*> (model);

            ui->setupUi(this);
            ui->chx_show_more->setChecked(false);
            ui->frm_more_information->setVisible(false);

            ui->sld_x->setEnabled(inputVariable != NULL);
            ui->led_x->setReadOnly(outputVariable != NULL);

            if (inputVariable) {
                ui->grx_output->setVisible(false);
            }

            connect();
        }

        void Control::connect() {
            QObject::connect(ui->chx_show_more, SIGNAL(stateChanged(int)),
                    this, SLOT(onClickShowMoreInformation(int)));

            QObject::connect(ui->sld_x, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderValue(int)));
            QObject::connect(ui->led_x, SIGNAL(editingFinished()), this,
                    SLOT(onEditInputValue()));
            QObject::connect(this, SIGNAL(inputValueChanged()),
                    this, SLOT(refreshModel()), Qt::QueuedConnection);

        }

        void Control::disconnect() {
        }

        void Control::onClickShowMoreInformation(int) {
            ui->frm_more_information->setVisible(ui->chx_show_more->isChecked());
            adjustSize();
        }

        void Control::onChangeSliderValue(int position) {
            if (inputVariable) {
                scalar input = fl::Op::Scale(ui->sld_x->value(),
                        ui->sld_x->minimum(), ui->sld_x->maximum(),
                        inputVariable->minimum(), inputVariable->maximum());
                inputVariable->setInput(input);
                emit(inputValueChanged());
            }
        }

        void Control::onEditInputValue() {
            if (inputVariable) {
                scalar input;
                try {
                    input = fl::Op::Scalar(ui->led_x->text().toStdString());
                    inputVariable->setInput(input);
                    emit(inputValueChanged());
                } catch (fl::Exception& ex) {
                    QMessageBox::critical(this, "Error",
                            QString::fromStdString(ex.what()),
                            QMessageBox::Ok);
                    return;
                }
            }
        }

        void Control::onOutputValueChanged() {
            refreshModel();
        }

        void Control::onChangeToolBoxPage(int dummy) {
            refreshModel();
        }

        void Control::showEvent(QShowEvent* e) {
            FL_LOG("showEvent");
            refreshModel();
        }

        void Control::resizeEvent(QResizeEvent* e) {
            FL_LOG("resizeEvent");
            refreshModel();
        }

        void Control::focusInEvent(QFocusEvent* e) {
        }

        void Control::refreshModel() {
            //            return;
            ui->canvas->clear();
            if (inputVariable) {
                //                return ;
                ui->canvas->draw(inputVariable);
                scalar x = inputVariable->getInput();
                scalar y;
                fl::Term* highest = inputVariable->highestMembership(x, &y);
                ui->canvas->drawGuide(inputVariable->getInput(), y,
                        QColor(0, 0, 255));

                QString fuzzify;
                for (int i = 0; i < inputVariable->numberOfTerms(); ++i) {
                    fuzzify += QString::number(inputVariable->getTerm(i)->membership(x), 'g', 2)
                            + "/" + QString::fromStdString(inputVariable->getTerm(i)->getName());
                    if (i < inputVariable->numberOfTerms() - 1) fuzzify += " + ";
                }
                //Controls...
                ui->lbl_fuzzy->setText(fuzzify);

                ui->led_x->setText(QString::number(x, 'g', 2));
                ui->led_mu->setText(QString::number(y, 'g', 2));

                ui->led_x_term->setText(QString::number(x, 'g', 2));

                if (highest) {
                    ui->grx_term->setTitle("Term: " +
                            QString::fromStdString(highest->getName()));
                    QString muTerm = QString::number(highest->membership(x), 'g', 2);
                    ui->led_mu_term->setText(muTerm);
                    QString range = "Range: (" + QString::number(highest->minimum(), 'g', 2)
                            + ", " + QString::number(highest->maximum(), 'g', 2) + ")";
                    ui->lbl_term_range->setText(range);
                }
                ui->grx_variable->setTitle("Variable: " +
                        QString::fromStdString(inputVariable->getName()));

                QString range = "Range: (" + QString::number(inputVariable->minimum(), 'g', 2)
                        + ", " + QString::number(inputVariable->maximum(), 'g', 2) + ")";
                ui->lbl_var_range->setText(range);

            } else if (outputVariable) {
                //                return ;
                ui->canvas->draw(outputVariable);
                ui->canvas->draw(outputVariable->output());
                scalar x = outputVariable->defuzzify();
                scalar y = outputVariable->output()->membership(x);
                ui->canvas->drawGuide(x, y, QColor(0, 0, 255, 255));

                ui->sld_x->setValue((int) fl::Op::Scale(x,
                        outputVariable->minimum(), outputVariable->maximum(),
                        ui->sld_x->minimum(), ui->sld_x->maximum()));
                QString fuzzify;
                for (int i = 0; i < outputVariable->numberOfTerms(); ++i) {
                    fuzzify += QString::number(outputVariable->getTerm(i)->membership(x), 'g', 2)
                            + "/" + QString::fromStdString(outputVariable->getTerm(i)->getName());
                    if (i < outputVariable->numberOfTerms() - 1) fuzzify += " + ";
                }
                //Controls...
                ui->lbl_fuzzy->setText(fuzzify);
                ui->led_x->setText(QString::number(x, 'g', 2));
                ui->led_mu->setText(QString::number(y, 'g', 2));

                ui->led_x_term->setText(QString::number(x, 'g', 2));

                fl::Term* highest = outputVariable->highestMembership(x, &y);

                if (highest) {
                    ui->grx_term->setTitle("Term: " +
                            QString::fromStdString(highest->getName()));
                    QString muTerm = QString::number(highest->membership(x), 'g', 2);
                    ui->led_mu_term->setText(muTerm);
                    QString range = "Range: (" + QString::number(highest->minimum(), 'g', 2)
                            + ", " + QString::number(highest->maximum(), 'g', 2) + ")";
                    ui->lbl_term_range->setText(range);
                }


                QString range = "Range: (" + QString::number(outputVariable->minimum(), 'g', 2)
                        + ", " + QString::number(outputVariable->maximum(), 'g', 2) + ")";
                ui->lbl_var_range->setText(range);

                range = "Range: (" + QString::number(outputVariable->output()->minimum(), 'g', 2)
                        + ", " + QString::number(outputVariable->output()->maximum(), 'g', 2) + ")";
                ui->lbl_output_range->setText(range);
                
                ui->grx_variable->setTitle("Variable: " +
                        QString::fromStdString(outputVariable->getName()));
                ui->led_output_prev->setText(QString::number(
                outputVariable->getDefuzzifiedValue()));
            }
        }

    } /* namespace qt */
} /* namespace fl */
