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
        QWidget(parent, f), ui(new Ui::Control), canvas(NULL) {
        }

        Control::~Control() {
            delete ui;
        }

        void Control::setup(fl::Variable* model) {
            this->variable = model;

            ui->setupUi(this);
            canvas = ui->canvas;
            ui->sld_x->setEnabled(true);
            ui->led_x->setReadOnly(false);

            connect();
        }

        void Control::connect() {
            QObject::connect(ui->sld_x, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderValue(int)));

            QObject::connect(ui->sld_x, SIGNAL(sliderPressed()),
                    this, SLOT(onPressSlider()));

            QObject::connect(ui->sld_x, SIGNAL(sliderReleased()),
                    this, SLOT(onReleaseSlider()));

            QObject::connect(ui->led_x, SIGNAL(editingFinished()), this,
                    SLOT(onEditInputValue()));
            QObject::connect(this, SIGNAL(crispValueChanged()),
                    this, SLOT(drawVariable()), Qt::QueuedConnection);

        }

        void Control::disconnect() {
            QObject::disconnect(ui->sld_x, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderValue(int)));

            QObject::disconnect(ui->sld_x, SIGNAL(sliderPressed()),
                    this, SLOT(onPressSlider()));

            QObject::disconnect(ui->sld_x, SIGNAL(sliderReleased()),
                    this, SLOT(onReleaseSlider()));

            QObject::disconnect(ui->led_x, SIGNAL(editingFinished()), this,
                    SLOT(onEditInputValue()));
            QObject::disconnect(this, SIGNAL(crispValueChanged()),
                    this, SLOT(drawVariable()));
        }

        void Control::setReadOnly(bool readOnly){
            ui->led_x->setReadOnly(readOnly);
            ui->sld_x->setEnabled(not readOnly);
        }
        
        bool Control::isReadOnly() const{
            return ui->led_x->isReadOnly() and not ui->sld_x->isEnabled();
        }

        void Control::onPressSlider() {
            ui->sld_x->setCursor(QCursor(Qt::ClosedHandCursor));
        }

        void Control::onReleaseSlider() {
            ui->sld_x->setCursor(QCursor(Qt::OpenHandCursor));
        }

        void Control::onChangeSliderValue(int position) {
            scalar value = fl::Op::Scale(position,
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    variable->getMinimum(), variable->getMaximum());
            ui->led_x->setText(QString::number(value, 'f', 2));
            onEditInputValue();
        }

        void Control::onEditInputValue() {
            scalar value;
            try {
                value = fl::Op::Scalar(ui->led_x->text().toStdString());
                if (not fl::Op::IsInf(value) and not fl::Op::IsNan(value)) {
                    int position = (int) fl::Op::Scale(value,
                            variable->getMinimum(), variable->getMaximum(),
                            ui->sld_x->minimum(), ui->sld_x->maximum());
                    if (position != ui->sld_x->value()) {
                        ui->sld_x->setValue(position);
                    }
                }
                if (not ui->led_x->isReadOnly()) {
                    fl::InputVariable* inputVariable = dynamic_cast<fl::InputVariable*> (variable);
                    if (inputVariable) {
                        inputVariable->setInput(value);
                    }
                    emit(crispValueChanged());
                }

            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Error",
                        QString::fromStdString(ex.what()),
                        QMessageBox::Ok);
                return;
            }
        }

        void Control::showEvent(QShowEvent*) {
            drawVariable();
        }

        void Control::resizeEvent(QResizeEvent*) {
            drawVariable();
        }

        void Control::drawVariable() {
            ui->canvas->clear();
            ui->canvas->draw(variable);

            scalar x = fl::Op::Scale(ui->sld_x->value(),
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    variable->getMinimum(), variable->getMaximum());
            scalar y = 0;
            variable->highestMembership(x, &y);
            ui->canvas->drawGuide(x, y, QColor(0, 0, 255));

            ui->lbl_min->setText(QString::number(variable->getMinimum(), 'f', 2));
            ui->lbl_max->setText(QString::number(variable->getMaximum(), 'f', 2));

            QString fuzzify;
            for (int i = 0; i < variable->numberOfTerms(); ++i) {
                fuzzify += QString::number(variable->getTerm(i)->membership(x), 'f', 2)
                        + "/" + QString::fromStdString(variable->getTerm(i)->getName());

                if (i < variable->numberOfTerms() - 1) fuzzify += " + ";
            }
            ui->lbl_fuzzy->setText(fuzzify);
        }

        void Control::drawOutputVariable() {
            ui->canvas->clear();
            ui->canvas->draw(variable);
            fl::OutputVariable* outputVariable = dynamic_cast<fl::OutputVariable*> (variable);
            ui->canvas->draw(outputVariable->output());

            scalar x = outputVariable->defuzzify();
            scalar y = 0;
            outputVariable->highestMembership(x, &y);
            ui->canvas->drawGuide(x, y, QColor(0, 0, 255));

            ui->lbl_min->setText(QString::number(variable->getMinimum(), 'f', 2));
            ui->lbl_max->setText(QString::number(variable->getMaximum(), 'f', 2));

            QString fuzzify;
            for (int i = 0; i < variable->numberOfTerms(); ++i) {
                fuzzify += QString::number(variable->getTerm(i)->membership(x), 'f', 2)
                        + "/" + QString::fromStdString(variable->getTerm(i)->getName());

                if (i < variable->numberOfTerms() - 1) fuzzify += " + ";
            }
            ui->lbl_fuzzy->setText(fuzzify);

            ui->sld_x->setValue((int) fl::Op::Scale(x,
                    outputVariable->getMinimum(), outputVariable->getMaximum(),
                    ui->sld_x->minimum(), ui->sld_x->maximum()));
        }

    }
}
