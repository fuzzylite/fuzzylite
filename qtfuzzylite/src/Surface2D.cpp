/* 
 * File:   Surface2D.cpp
 * Author: jcrada
 *
 * Created on 10 July 2013, 10:12 PM
 */

#include "fl/qt/Surface2D.h"

#include "fl/qt/Window.h"
#include "fl/qt/Model.h"

#include <QMessageBox>
#include <QColorDialog>
#include <QSettings>

#include <fl/Headers.h>

namespace fl {
    namespace qt {

        Surface2D::Surface2D(QWidget* parent, Qt::WindowFlags f) :
        QDialog(parent, f), _swap(false), ui(new Ui::Surface2D) { }

        Surface2D::~Surface2D() { }
        
        void Surface2D::onEngineChanged(){
            close();
        }

        void Surface2D::setup() {
            ui->setupUi(this);
            setWindowFlags(Qt::Dialog
                    | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowStaysOnTopHint);

            setGeometry(0, 0, 480, 480);
//            adjustSize();
            
            QRect scr = Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());

            QList<int> sizes;
            sizes << .25 * size().width() << .75 * size().width();
            ui->splitter->setSizes(sizes);
            
            QObject::connect(Model::Default(), SIGNAL(engineChanged()),
                    this, SLOT(onEngineChanged()));

            QObject::connect(ui->cbx_inputA, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeInputA(int)));
            QObject::connect(ui->cbx_inputB, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeInputB(int)));
            QObject::connect(ui->cbx_output, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeOutput(int)));
            
            QObject::connect(ui->sbx_min_inputA, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBox(double)));
            QObject::connect(ui->sbx_max_inputA, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBox(double)));
            QObject::connect(ui->sbx_min_inputB, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBox(double)));
            QObject::connect(ui->sbx_max_inputB, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBox(double)));

            QObject::connect(ui->btn_inputs, SIGNAL(clicked()),
                    this, SLOT(onClickInputs()));

            QObject::connect(ui->btn_options, SIGNAL(clicked()),
                    this, SLOT(onClickOptions()));

            QObject::connect(ui->btn_min_color, SIGNAL(clicked()),
                    this, SLOT(onClickMinimumColor()));
            QObject::connect(ui->btn_max_color, SIGNAL(clicked()),
                    this, SLOT(onClickMaximumColor()));

            QObject::connect(ui->btn_generate, SIGNAL(clicked()),
                    this, SLOT(onClickGenerate()),Qt::QueuedConnection);

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

            QColor minColor = settings.value("view/surface2DMinColor", QColor(Qt::yellow)).value<QColor>();
            QPixmap minIcon(16, 16);
            minIcon.fill(minColor);
            ui->btn_min_color->setIcon(minIcon);

            QColor maxColor = settings.value("view/surface2DMaxColor", QColor(Qt::red)).value<QColor>();
            QPixmap maxIcon(16, 16);
            maxIcon.fill(maxColor);
            ui->btn_max_color->setIcon(maxIcon);

            ui->sbx_width->setValue(
                    settings.value("view/surface2DWidth", 64).toInt());
            ui->sbx_height->setValue(
                    settings.value("view/surface2DHeight", 64).toInt());

            ui->canvas->installEventFilter(new MouseTrackingFilter(this));
        }

        void Surface2D::onClickGenerate() {
            QSettings settings;
            settings.setValue("view/surface2DWidth", ui->sbx_width->value());
            settings.setValue("view/surface2DHeight", ui->sbx_height->value());

            ui->canvas->setPixmap(QPixmap());
            ui->canvas->repaint();

            Engine* engine = Model::Default()->engine();
            std::string status;
            if (not engine->isReady(&status)) {
                QMessageBox::critical(this, "Engine not ready",
                        "<qt>The following errors were encountered:<br><br>" +
                        Window::mainWindow()->toHtmlEscaped(
                        QString::fromStdString(status)).replace("\n", "<br>")
                        + "</qt>");

                return;
            }
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
            OutputVariable* output = engine->getOutputVariable(ui->cbx_output->currentIndex());

            if (output->isLockingOutputRange()) generateLockingRange();
            else generateRangeFree();

            QApplication::restoreOverrideCursor();
        }

        void Surface2D::generateRangeFree() {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputA = engine->getInputVariable(ui->cbx_inputA->currentIndex());
            InputVariable* inputB = engine->getInputVariable(ui->cbx_inputB->currentIndex());
            OutputVariable* output = engine->getOutputVariable(ui->cbx_output->currentIndex());
            output->setLastValidOutput(output->getDefaultValue());

            scalar minInputA = ui->sbx_min_inputA->value();
            scalar maxInputA = ui->sbx_max_inputA->value();
            scalar minInputB = ui->sbx_min_inputB->value();
            scalar maxInputB = ui->sbx_max_inputB->value();
            
            int width = ui->sbx_width->value();
            int height = ui->sbx_height->value();
            _matrix.resize(width);
            scalar minOutput = fl::inf, maxOutput = -fl::inf;
            for (int a = 0; a < width; ++a) {
                scalar aValue = fl::Op::scale(a, 0, ui->sbx_width->value(),
                        minInputA, maxInputA);
                inputA->setInput(aValue);
                _matrix.at(a).resize(height);
                for (int b = 0; b < height; ++b) {
                    scalar bValue = fl::Op::scale(b, 0, ui->sbx_height->value(),
                            minInputB, maxInputB);
                    inputB->setInput(bValue);

                    engine->process();

                    scalar oValue = output->defuzzify();
                    
                    _matrix.at(a).at(b) = tuple(aValue, bValue, oValue);
                    if (not fl::Op::isNan(oValue)) {
                        if (oValue < minOutput) minOutput = oValue;
                        if (oValue > maxOutput) maxOutput = oValue;
                    }
                }
            }

            QImage canvas(width, height, QImage::Format_RGB32);
            QSettings settings;
            QColor invalidColor = settings.value("view/surface2DInvalidColor",
                    QColor(Qt::blue)).value<QColor>();
            QColor minColor = settings.value("view/surface2DMinColor").value<QColor>();
            QColor maxColor = settings.value("view/surface2DMaxColor").value<QColor>();
            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    QColor pixel = invalidColor;
                    scalar output = _matrix.at(x).at(y).output;
                    if (not (fl::Op::isInf(output) or fl::Op::isNan(output))) {
                        int gradient = (int) fl::Op::scale(output, minOutput, maxOutput, 0, 255);
                        pixel = Window::mainWindow()->gradient(gradient, minColor, maxColor);
                    }
                    canvas.setPixel(x, y, pixel.rgb());
                }
            }

            ui->canvas->setPixmap(QPixmap::fromImage(canvas));
        }

        void Surface2D::generateLockingRange() {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputA = engine->getInputVariable(ui->cbx_inputA->currentIndex());
            InputVariable* inputB = engine->getInputVariable(ui->cbx_inputB->currentIndex());
            OutputVariable* output = engine->getOutputVariable(ui->cbx_output->currentIndex());
            output->setLastValidOutput(output->getDefaultValue());

            scalar minInputA = ui->sbx_min_inputA->value();
            scalar maxInputA = ui->sbx_max_inputA->value();
            scalar minInputB = ui->sbx_min_inputB->value();
            scalar maxInputB = ui->sbx_max_inputB->value();
            
            int width = ui->sbx_width->value();
            int height = ui->sbx_height->value();
            _matrix.resize(width);

            QImage canvas(width, height, QImage::Format_RGB32);
            QSettings settings;
            QColor invalidColor = settings.value("view/surface2DInvalidColor",
                    QColor(Qt::blue)).value<QColor>();
            QColor minColor = settings.value("view/surface2DMinColor").value<QColor>();
            QColor maxColor = settings.value("view/surface2DMaxColor").value<QColor>();

            for (int a = 0; a < width; ++a) {
                scalar aValue = fl::Op::scale(a, 0, ui->sbx_width->value(),
                        minInputA, maxInputA);
                inputA->setInput(aValue);
                _matrix.at(a).resize(height);
                for (int b = 0; b < height; ++b) {
                    scalar bValue = fl::Op::scale(b, 0, ui->sbx_height->value(),
                            minInputB, maxInputB);
                    inputB->setInput(bValue);

                    engine->process();

                    scalar oValue = output->defuzzify();
                    _matrix.at(a).at(b) = tuple(aValue, bValue, oValue);

                    QColor pixel = invalidColor;
                    if (not (fl::Op::isInf(oValue) or fl::Op::isNan(oValue))) {
                        int gradient = (int) fl::Op::scale(oValue,
                                output->getMinimum(), output->getMaximum(), 0, 255);
                        pixel = Window::mainWindow()->gradient(gradient, minColor, maxColor);
                    }
                    canvas.setPixel(a, b, pixel.rgb());
                }
            }

            ui->canvas->setPixmap(QPixmap::fromImage(canvas));
        }

        void Surface2D::onChangeInputA(int index) {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputVariable = engine->getInputVariable(index);
            ui->sbx_min_inputA->setValue(inputVariable->getMinimum());
            ui->sbx_max_inputA->setValue(inputVariable->getMaximum());
            updateWindowTitle();
        }

        void Surface2D::onChangeInputB(int index) {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputVariable = engine->getInputVariable(index);
            ui->sbx_min_inputB->setValue(inputVariable->getMinimum());
            ui->sbx_max_inputB->setValue(inputVariable->getMaximum());
            updateWindowTitle();
        }

        void Surface2D::onChangeOutput(int index) {
            Engine* engine = Model::Default()->engine();
            OutputVariable* outputVariable = engine->getOutputVariable(index);
            ui->sbx_min_output->setValue(outputVariable->getMinimum());
            ui->sbx_max_output->setValue(outputVariable->getMaximum());
            updateWindowTitle();
        }
        
        void Surface2D::onChangeSpinBox(double){
            ui->lbl_min_a->setText(ui->sbx_min_inputA->text());
            ui->lbl_max_a->setText(ui->sbx_max_inputA->text());
            ui->lbl_min_b->setText(ui->sbx_min_inputB->text());
            ui->lbl_max_b->setText(ui->sbx_max_inputB->text());
        }

        void Surface2D::updateWindowTitle() {
            Engine* engine = Model::Default()->engine();
            std::string title;
            if (ui->cbx_inputA->count() > 0) {
                title += engine->getInputVariable(ui->cbx_inputA->currentIndex())->getName();
            }
            if (ui->cbx_inputB->count() > 0) {
                title += "," + engine->getInputVariable(ui->cbx_inputB->currentIndex())->getName();
            }
            if (ui->cbx_output->count() > 0) {
                title += " -> " + engine->getOutputVariable(ui->cbx_output->currentIndex())->getName();
            }
            if (not title.empty()) title += " - ";
            title += "Surface 2D";
            setWindowTitle(QString::fromStdString(title));
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

        void Surface2D::onClickOptions() { }

        void Surface2D::onOptionTriggered() { }



    }
}
