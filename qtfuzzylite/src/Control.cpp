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

        void Control::setup() {

        }

        void Control::setup(InputVariable* inputVariable) {

        }
        void Control::setup(OutputVariable* outputVariable) {

        }

        void Control::updateOutputValue() {
        }

        void Control::onChangeSliderValue(int position) {
        }
        void Control::onEditInputValue() {
        }

        void Control::connect() {
        }
        void Control::disconnect() {
        }
        void Control::resizeEvent(QResizeEvent* e) {
        }
        void Control::focusInEvent(QFocusEvent* e) {
        }

    } /* namespace qt */
} /* namespace fl */
