/* 
 * File:   Surface2D.cpp
 * Author: jcrada
 *
 * Created on 10 July 2013, 10:12 PM
 */

#include "fl/qt/Surface2D.h"

#include "fl/qt/Window.h"
#include "fl/qt/Model.h"

#include <QColorDialog>
#include <QSettings>

#include <fl/Headers.h>

namespace fl {
    namespace qt {

        Surface2D::Surface2D(QWidget* parent, Qt::WindowFlags f) :
        QDialog(parent, f), ui(new Ui::Surface2D) { }

        Surface2D::~Surface2D() { }

        void Surface2D::setup() {
            ui->setupUi(this);
            setWindowFlags(Qt::Dialog
                    | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowStaysOnTopHint);

            setGeometry(0, 0, 640, 480);

            QRect scr = Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());

            QObject::connect(ui->cbx_inputA, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeInputA(int)));
            QObject::connect(ui->cbx_inputB, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeInputB(int)));
            QObject::connect(ui->cbx_output, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeOutput(int)));

            QObject::connect(ui->btn_inputs, SIGNAL(clicked()),
                    this, SLOT(onClickInputs()));

            QObject::connect(ui->btn_swap_axes, SIGNAL(clicked()),
                    this, SLOT(onClickSwapAxes()));

            QObject::connect(ui->btn_min_color, SIGNAL(clicked()),
                    this, SLOT(onClickMinimumColor()));
            QObject::connect(ui->btn_max_color, SIGNAL(clicked()),
                    this, SLOT(onClickMaximumColor()));

            QObject::connect(ui->btn_generate, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()));

            Engine* engine = Model::Default()->engine();
            QStringList inputs, outputs;
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                inputs << QString::fromStdString(engine->getInputVariable(i)->getName());
            }
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                outputs << QString::fromStdString(engine->getOutputVariable(i)->getName());
            }


            ui->cbx_inputA->addItems(inputs);
            ui->cbx_inputB->addItems(inputs);
            if (inputs.size() > 1) {
                ui->cbx_inputA->setCurrentIndex(0);
                ui->cbx_inputB->setCurrentIndex(1);
            }

            ui->cbx_output->addItems(outputs);
            if (outputs.size() > 0) {
                ui->cbx_output->setCurrentIndex(0);
            }

            QSettings settings;

            QColor minColor = settings.value("view/surface2DMinColor", QColor(Qt::white)).value<QColor>();
            QPixmap minIcon(16, 16);
            minIcon.fill(minColor);
            ui->btn_min_color->setIcon(minIcon);

            QColor maxColor = settings.value("view/surface2DMaxColor", QColor(Qt::black)).value<QColor>();
            QPixmap maxIcon(16, 16);
            maxIcon.fill(maxColor);
            ui->btn_max_color->setIcon(maxIcon);

            ui->sbx_width->setValue(
                    settings.value("view/surface2DWidth", 640).toInt());
            ui->sbx_height->setValue(
                    settings.value("view/surface2DHeight", 480).toInt());
        }

        void Surface2D::onClickGenerate() {
            QSettings settings;
            settings.setValue("view/surface2DWidth", ui->sbx_width->value());
            settings.setValue("view/surface2DHeight", ui->sbx_height->value());

        }

        void Surface2D::onChangeInputA(int index) {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputVariable = engine->getInputVariable(index);
            ui->sbx_min_inputA->setValue(inputVariable->getMinimum());
            ui->sbx_max_inputA->setValue(inputVariable->getMaximum());
        }

        void Surface2D::onChangeInputB(int index) {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputVariable = engine->getInputVariable(index);
            ui->sbx_min_inputB->setValue(inputVariable->getMinimum());
            ui->sbx_max_inputB->setValue(inputVariable->getMaximum());
        }

        void Surface2D::onChangeOutput(int index) {
            Engine* engine = Model::Default()->engine();
            OutputVariable* outputVariable = engine->getOutputVariable(index);
            ui->sbx_min_output->setValue(outputVariable->getMinimum());
            ui->sbx_max_output->setValue(outputVariable->getMaximum());
        }

        void Surface2D::onClickInputs() { }

        void Surface2D::onClickMinimumColor() {
            QSettings settings;

            QColor minColor = settings.value("view/surface2DMinColor").value<QColor>();
            if (not minColor.isValid()) minColor = QColor(Qt::white);

            minColor = QColorDialog::getColor(minColor, this, "Select minimum color");
            if (minColor.isValid()) {
                settings.setValue("view/surface2DMinColor", minColor);
                QPixmap minIcon(16, 16);
                minIcon.fill(minColor);
                ui->btn_min_color->setIcon(minIcon);
            }
        }

        void Surface2D::onClickMaximumColor() {
            QSettings settings;

            QColor maxColor = settings.value("view/surface2DMaxColor").value<QColor>();
            if (not maxColor.isValid()) maxColor = QColor(Qt::black);

            maxColor = QColorDialog::getColor(maxColor, this, "Select maximum color");
            if (maxColor.isValid()) {
                settings.setValue("view/surface2DMaxColor", maxColor);
                QPixmap maxIcon(16, 16);
                maxIcon.fill(maxColor);
                ui->btn_max_color->setIcon(maxIcon);
            }
        }

        void Surface2D::onClickSwapAxes() { }
    }
}
