/*
 * Control.cpp
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Control.h"

#include <QtGui/QMessageBox>
namespace fl {
    namespace qt {

        Control::Control(QWidget* parent, Qt::WindowFlags f) :
                QWidget(parent, f), ui(new Ui::Control) {
        }

        Control::~Control() {
            delete ui;
        }

        void Control::setup(fl::Variable* model) {
            inputVariable = dynamic_cast<InputVariable*>(model);
            outputVariable = dynamic_cast<OutputVariable*>(model);

            ui->setupUi(this);
            ui->chx_show_more->setChecked(false);
            ui->tbx_information->setVisible(false);

            ui->sld_x->setEnabled(inputVariable != NULL);
            ui->led_x->setReadOnly(outputVariable != NULL);

            if (inputVariable) {
                ui->output->setHidden(true);
                ui->tbx_information->removeItem(0);
            }

            connect();
            ui->tbx_information->setCurrentIndex(0);
        }

        void Control::connect() {
            QObject::connect(ui->tbx_information, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxPage(int)), Qt::QueuedConnection);
            QObject::connect(ui->chx_show_more, SIGNAL(stateChanged(int)),
                    this, SLOT(onClickShowMoreInformation(int)));

            QObject::connect(ui->sld_x, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderValue(int)));
            QObject::connect(ui->led_x, SIGNAL(editingFinished()), this,
                    SLOT(onEditInputValue()));
            QObject::connect(this, SIGNAL(inputValueChanged(double)),
                    this, SLOT(refreshModel()), Qt::QueuedConnection);

        }
        void Control::disconnect() {
        }

        void Control::updateOutputValue() {
            refreshModel();
        }

        void Control::onClickShowMoreInformation(int) {
            ui->tbx_information->setVisible(ui->chx_show_more->isChecked());
        }
        void Control::onChangeSliderValue(int position) {
            if (inputVariable) {
                scalar input = fl::Op::Scale(ui->sld_x->value(),
                        ui->sld_x->minimum(), ui->sld_x->maximum(),
                        inputVariable->minimum(), inputVariable->maximum());
                inputVariable->setInput(input);
                emit(inputValueChanged(input));
            }
        }
        void Control::onEditInputValue() {
            if (inputVariable) {
                scalar input;
                try {
                    input = fl::Op::Scalar(ui->led_x->text().toStdString());
                    inputVariable->setInput(input);
                    emit(inputValueChanged(input));
                } catch (fl::Exception& ex) {
                    QMessageBox::critical(this, "Error",
                            QString::fromStdString(ex.what()),
                            QMessageBox::Ok);
                    return;
                }
            }
        }
        void Control::onChangeToolBoxPage(int dummy) {
            refreshModel();
        }

        void Control::showEvent(QShowEvent* e) {
            FL_LOG("showEvent");
//            refreshModel();
        }
        void Control::resizeEvent(QResizeEvent* e) {
            FL_LOG("resizeEvent");
//            refreshModel();
        }
        void Control::focusInEvent(QFocusEvent* e) {
        }

        void Control::refreshModel() {
            ui->canvas->clear();
            if (inputVariable) {
                ui->canvas->draw(inputVariable);
                scalar y;
                inputVariable->highestMembership(inputVariable->getInput(), &y);
                ui->canvas->drawGuide(inputVariable->getInput(), 1,
                        QColor(0,0,255));
            }
            else if (outputVariable) {
//                ui->canvas->draw(outputVariable);
//                ui->canvas->draw(outputVariable->output());
//                scalar x = outputVariable->defuzzifyIgnoreLock();
//                scalar y = outputVariable->output()->membership(x);
//                ui->canvas->drawGuide(x, y, QColor(0, 0, 255, 255));
            }
        }

    } /* namespace qt */
} /* namespace fl */
