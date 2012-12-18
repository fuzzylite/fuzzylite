/*
 * Control.cpp
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include "../fl/qt/Control.h"

namespace fl {
    namespace qt {

        Control::Control(QWidget* parent, Qt::WindowFlags f)
                :
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

        }
        void Control::disconnect() {
        }

        void Control::updateOutputValue() {
        }

        void Control::onClickShowMoreInformation(int){
            ui->tbx_information->setVisible(ui->chx_show_more->isChecked());
        }
        void Control::onChangeSliderValue(int position) {

        }
        void Control::onEditInputValue() {
        }
        void Control::onChangeToolBoxPage(int dummy) {
            refreshModel();
        }

        void Control::showEvent(QShowEvent* e) {
//            refreshModel();
        }
        void Control::resizeEvent(QResizeEvent* e) {
//            refreshModel();
        }
        void Control::focusInEvent(QFocusEvent* e) {
        }

        void Control::refreshModel() {
            ui->canvas->clear();
            if (inputVariable) {
                ui->canvas->draw(inputVariable);
//                ui->canvas->drawGuide()
            }
            else if (outputVariable) {
                ui->canvas->draw(outputVariable);
                scalar x = fl::Op::Scale(outputVariable->defuzzifyIgnoreLock(),
                        outputVariable->minimum(), outputVariable->maximum(),
                        ui->canvas->rect().left(), ui->canvas->rect().right());
                scalar y = fl::Op::Scale(outputVariable->output()->membership(x),
                        0, 1, ui->canvas->rect().bottom(), ui->canvas->rect().top());
                ui->canvas->drawGuide(x, y, QColor(0, 0, 255, 255));
            }
        }

    } /* namespace qt */
} /* namespace fl */
