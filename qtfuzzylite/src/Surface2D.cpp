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

    Juan Rada-Vilela, 21 July 2013
    jcrada@fuzzylite.com
 **/

/* 
 * File:   Surface2D.cpp
 * Author: jcrada
 *
 * Created on 10 July 2013, 10:12 PM
 */

#include "fl/qt/Surface2D.h"

#include "fl/qt/Window.h"
#include "fl/qt/Model.h"
#include "fl/qt/Control.h"


#include <QMessageBox>
#include <QColorDialog>
#include <QSettings>
#include <QPainter>
#include <QInputDialog>
#include <QFileDialog>

#include <fl/Headers.h>

namespace fl {
    namespace qt {

        Surface2D::Surface2D(QWidget* parent, Qt::WindowFlags f) :
        QDialog(parent, f), ui(new Ui::Surface2D) {
            setWindowFlags(Qt::Dialog
                    | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowStaysOnTopHint
                    | Qt::WindowMaximizeButtonHint);
        }

        Surface2D::~Surface2D() {
        }

        void Surface2D::onEngineChanged() {
            close();
        }

        void Surface2D::setup() {
            ui->setupUi(this);

            setGeometry(0, 0, 480, 480);

            QRect scr = Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());

            QList<int> sizes;
            sizes << .25 * size().width() << .75 * size().width();
            ui->splitter->setSizes(sizes);

            QObject::connect(Model::Default(), SIGNAL(engineChanged()),
                    this, SLOT(onEngineChanged()));
            QObject::connect(Window::mainWindow(), SIGNAL(engineVariableChanged()),
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
                    this, SLOT(onClickGenerate()));

            Engine* engine = Model::Default()->engine();
            engine->restart();
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

            QColor minColor = settings.value("surface2D/minColor",
                    QColor(Qt::yellow)).value<QColor>();
            QPixmap minIcon(16, 16);
            minIcon.fill(minColor);
            ui->btn_min_color->setIcon(minIcon);

            QColor maxColor = settings.value("surface2D/maxColor",
                    QColor(Qt::red)).value<QColor>();
            QPixmap maxIcon(16, 16);
            maxIcon.fill(maxColor);
            ui->btn_max_color->setIcon(maxIcon);

            ui->sbx_width->setValue(
                    settings.value("surface2D/width", 128).toInt());
            ui->sbx_height->setValue(
                    settings.value("surface2D/height", 128).toInt());

            ui->canvas->installEventFilter(new MouseTrackingFilter(this));
            updateWindowTitle();
        }

        void Surface2D::onClickGenerate() {
            QSettings settings;
            settings.setValue("surface2D/width", ui->sbx_width->value());
            settings.setValue("surface2D/height", ui->sbx_height->value());

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

            InputVariable* inputA = engine->getInputVariable(ui->cbx_inputA->currentIndex());
            InputVariable* inputB = engine->getInputVariable(ui->cbx_inputB->currentIndex());
            OutputVariable* output = engine->getOutputVariable(ui->cbx_output->currentIndex());

            minInputA = ui->sbx_min_inputA->value();
            maxInputA = ui->sbx_max_inputA->value();
            minInputB = ui->sbx_min_inputB->value();
            maxInputB = ui->sbx_max_inputB->value();

            if (output->isLockingOutputRange()) {
                minOutput = output->getMinimum();
                maxOutput = output->getMaximum();
            } else {
                minOutput = fl::inf;
                maxOutput = -fl::inf;
            }
            int width = ui->sbx_width->value();
            int height = ui->sbx_height->value();
            _matrix.resize(width);

            _surface = QImage(width, height, QImage::Format_RGB32);

            QColor minColor = settings.value("surface2D/minColor",
                    QColor(Qt::yellow)).value<QColor>();
            QColor maxColor = settings.value("surface2D/maxColor",
                    QColor(Qt::red)).value<QColor>();
            QColor invalidColor = Window::mainWindow()->gradient(255 / 2, minColor, maxColor);
            invalidColor.setRed(255 - invalidColor.red());
            invalidColor.setGreen(255 - invalidColor.green());
            invalidColor.setBlue(255 - invalidColor.blue());
            invalidColor = settings.value("surface2D/invalidColor",
                    invalidColor).value<QColor>();

            for (int a = 0; a < width; ++a) {
                scalar aValue = fl::Op::scale(a, 0, width - 1,
                        minInputA, maxInputA);
                inputA->setInputValue(aValue);
                _matrix.at(a).resize(height);
                int progress = fl::Op::scale(a, 0, width, 0, 100);
                ui->canvas->setText(QString::number(progress) + "%");
                ui->canvas->repaint();
                for (int b = 0; b < height; ++b) {
                    scalar bValue = fl::Op::scale(b, 0, height - 1,
                            minInputB, maxInputB);
                    inputB->setInputValue(bValue);

                    engine->process();
                    
                    scalar oValue = fl::nan;
                    for (int o = 0 ; o < engine->numberOfOutputVariables(); ++o){
                        OutputVariable* outputVariable = engine->getOutputVariable(o);
                        if (outputVariable == output){
                            oValue = output->defuzzify();
                        }else{
                            if (outputVariable->isEnabled()){
                                outputVariable->defuzzify();
                            }
                        }
                    }
                    
                    _matrix.at(a).at(b) = tuple(aValue, bValue, oValue);

                    if (output->isLockingOutputRange()) {
                        QColor pixel = invalidColor;
                        if (not (fl::Op::isInf(oValue) or fl::Op::isNan(oValue))) {
                            int gradient = (int) fl::Op::scale(oValue,
                                    minOutput, maxOutput, 0, 255);
                            pixel = Window::mainWindow()->gradient(gradient, minColor, maxColor);
                        }
                        _surface.setPixel(a, b, pixel.rgb());
                    } else {
                        if (not fl::Op::isNan(oValue)) {
                            if (oValue < minOutput) minOutput = oValue;
                            if (oValue > maxOutput) maxOutput = oValue;
                        }
                    }
                }
            }

            if (not output->isLockingOutputRange()) {
                for (int x = 0; x < width; ++x) {
                    for (int y = 0; y < height; ++y) {
                        QColor pixel = invalidColor;
                        scalar output = _matrix.at(x).at(y).output;
                        if (not (fl::Op::isInf(output) or fl::Op::isNan(output))) {
                            int gradient = (int) fl::Op::scale(output, minOutput, maxOutput, 0, 255);
                            pixel = Window::mainWindow()->gradient(gradient, minColor, maxColor);
                        }
                        _surface.setPixel(x, y, pixel.rgb());
                    }
                }
            }

            int contours = settings.value("surface2D/contours", 3).toInt();
            if (contours > 0) drawContours();
            else {
                QPixmap pixmap = QPixmap::fromImage(_surface);
                ui->canvas->setPixmap(pixmap);
            }
            ui->canvas->setText("");
            QApplication::restoreOverrideCursor();
            ui->btn_options->setEnabled(true);
        }

        void Surface2D::drawContours() {
            if (_surface.isNull()) return;

            QSettings settings;
            int contours = settings.value("surface2D/contours", 0).toInt();
            QList<double> levels;
            for (int i = 0; i < contours; ++i) {
                levels << fl::Op::scale(i, 0, contours, minOutput, maxOutput);
            }
            QPixmap pixmap = QPixmap::fromImage(_surface);

            QwtRasterData::ContourLines contourLines = RasterData(this).contourLines(
                    pixmap.rect(), pixmap.size(), levels,
                    QwtRasterData::IgnoreAllVerticesOnLevel);

            QPen pen;
            pen.setBrush(QBrush(settings.value("surface2D/contourColor",
                    QColor(Qt::black)).value<QColor>()));
            pen.setWidth(settings.value("surface2D/contourWidth", 1).toInt());

            QPainter painter;
            painter.begin(&pixmap);
            painter.setPen(pen);
            for (QMap<double, QPolygonF>::iterator it = contourLines.begin();
                    it != contourLines.end(); ++it) {
                painter.drawPolyline(it.value());
            }
            painter.end();

            ui->canvas->setPixmap(QPixmap());
            ui->canvas->repaint();
            ui->canvas->setPixmap(pixmap);
            ui->canvas->repaint();
        }

        void Surface2D::onChangeInputA(int index) {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputVariable = engine->getInputVariable(index);
            ui->sbx_min_inputA->setValue(inputVariable->getMinimum());
            ui->sbx_max_inputA->setValue(inputVariable->getMaximum());
            ui->lbl_min_a->setText(ui->sbx_min_inputA->text());
            ui->lbl_max_a->setText(ui->sbx_max_inputA->text());
            updateWindowTitle();
        }

        void Surface2D::onChangeInputB(int index) {
            Engine* engine = Model::Default()->engine();
            InputVariable* inputVariable = engine->getInputVariable(index);
            ui->sbx_min_inputB->setValue(inputVariable->getMinimum());
            ui->sbx_max_inputB->setValue(inputVariable->getMaximum());
            ui->lbl_min_b->setText(ui->sbx_min_inputB->text());
            ui->lbl_max_b->setText(ui->sbx_max_inputB->text());
            updateWindowTitle();
        }

        void Surface2D::onChangeOutput(int index) {
            Engine* engine = Model::Default()->engine();
            OutputVariable* outputVariable = engine->getOutputVariable(index);
            ui->sbx_min_output->setValue(outputVariable->getMinimum());
            ui->sbx_max_output->setValue(outputVariable->getMaximum());
            updateWindowTitle();
        }

        void Surface2D::onChangeSpinBox(double) {
            ui->lbl_min_a->setText(ui->sbx_min_inputA->text());
            ui->lbl_max_a->setText(ui->sbx_max_inputA->text());
            ui->lbl_min_b->setText(ui->sbx_min_inputB->text());
            ui->lbl_max_b->setText(ui->sbx_max_inputB->text());

            //TODO: Scale pixmap
            //            QPixmap pixmap = QPixmap::fromImage(_surface);
            //            int width = ui->sbx_width->value();
            //            int height = ui->sbx_height->value();
            //            int newWidth = fl::Op::scale()
            //            pixmap.scaled()
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

            _surface = QImage();
            ui->canvas->setPixmap(QPixmap());
            ui->canvas->repaint();
            ui->btn_options->setEnabled(false);

            ui->lbl_min_a->setText(ui->sbx_min_inputA->text());
            ui->lbl_max_a->setText(ui->sbx_max_inputA->text());
            ui->lbl_min_b->setText(ui->sbx_min_inputB->text());
            ui->lbl_max_b->setText(ui->sbx_max_inputB->text());
        }

        void Surface2D::onClickInputs() {
            Engine* engine = Model::Default()->engine();
            QMenu menu(this);
            for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                if (ui->cbx_inputA->currentIndex() == i) continue;
                if (ui->cbx_inputB->currentIndex() == i) continue;

                Control* control = new Control(&menu);
                control->setup(engine->getInputVariable(i));
                control->ui->mainLayout->setContentsMargins(3, 0, 3, 0);
                control->ui->btn_name->setEnabled(false);
                control->ui->btn_name->setIcon(QIcon());
                control->ui->wdg_canvas->setVisible(false);
                control->ui->wdg_out->setVisible(false);
                control->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                control->setFixedHeight(control->minimumSizeHint().height());

                control->ui->sbx_x->setValue(engine->getInputVariable(i)->getInputValue());

                QWidgetAction* action = new QWidgetAction(&menu);
                action->setDefaultWidget(control);

                menu.addAction(action);
            }
            if (menu.actions().empty()) {
                QAction* empty = new QAction("(empty)", this);
                empty->setEnabled(false);
                menu.addAction(empty);
            }
            menu.exec(QCursor::pos() + QPoint(1, 0));
            ui->btn_inputs->setChecked(false);
        }

        void Surface2D::onClickMinimumColor() {
            QSettings settings;

            QColor minColor = settings.value("surface2D/minColor").value<QColor>();
            if (not minColor.isValid()) minColor = QColor(Qt::yellow);

            minColor = QColorDialog::getColor(minColor, this, "Select minimum color");
            if (minColor.isValid()) {
                settings.setValue("surface2D/minColor", minColor);
                QPixmap minIcon(16, 16);
                minIcon.fill(minColor);
                ui->btn_min_color->setIcon(minIcon);
            }
        }

        void Surface2D::onClickMaximumColor() {
            QSettings settings;

            QColor maxColor = settings.value("surface2D/maxColor").value<QColor>();
            if (not maxColor.isValid()) maxColor = QColor(Qt::red);

            maxColor = QColorDialog::getColor(maxColor, this, "Select maximum color");
            if (maxColor.isValid()) {
                settings.setValue("surface2D/maxColor", maxColor);
                QPixmap maxIcon(16, 16);
                maxIcon.fill(maxColor);
                ui->btn_max_color->setIcon(maxIcon);
            }
        }

        void Surface2D::setNumberOfContours(int number) {
            QSettings settings;
            settings.setValue("surface2D/contours", number);
            drawContours();
        }

        void Surface2D::setWidthOfContours(int width) {
            QSettings settings;
            settings.setValue("surface2D/contourWidth", width);
            drawContours();
        }

        void Surface2D::onClickOptions() {
            QMenu menu(this);

            menu.addAction("Save As...", this, SLOT(onOptionTriggered()));
            menu.addSeparator();
            menu.addAction("Swap axes", this, SLOT(onOptionTriggered()));
            menu.addSeparator();
            menu.addAction("Contours...", this, SLOT(onOptionTriggered()));
            menu.addAction("Color...", this, SLOT(onOptionTriggered()));
            menu.addAction("Width...", this, SLOT(onOptionTriggered()));
            menu.addAction("About...", this, SLOT(onOptionTriggered()));

            menu.exec(QCursor::pos() + QPoint(1, 0));
            ui->btn_options->setChecked(false);
        }

        void Surface2D::onOptionTriggered() {
            QAction* action = qobject_cast<QAction *>(sender());
            if (not action) return;
            QSettings settings;
            if (action->text() == "Save As...") {
                QString recentLocation = settings.value("surface2D/recentLocation", ".").toString();
                QStringList filters;
                filters << "All files (*.*)"
                        << "Supported formats (*.png *.jpg *.jpeg *.tiff *.bmp)";

                QString filter = filters.last();
                QString filename = QFileDialog::getSaveFileName(this,
                        "Save image as", recentLocation,
                        filters.join(";;"),
                        &filter);
                if (filename.size() == 0) return;
                settings.setValue("surface2D/recentLocation", QFileInfo(filename).absoluteFilePath());
                saveAs(filename);

            } else if (action->text() == "Swap axes") {
                int tmpIndexA = ui->cbx_inputA->currentIndex();
                scalar tmpMinIndexA = ui->sbx_min_inputA->value();
                scalar tmpMaxIndexA = ui->sbx_max_inputA->value();

                ui->cbx_inputA->setCurrentIndex(ui->cbx_inputB->currentIndex());
                ui->sbx_min_inputA->setValue(ui->sbx_min_inputB->value());
                ui->sbx_max_inputA->setValue(ui->sbx_max_inputB->value());

                ui->cbx_inputB->setCurrentIndex(tmpIndexA);
                ui->sbx_min_inputB->setValue(tmpMinIndexA);
                ui->sbx_max_inputB->setValue(tmpMaxIndexA);

                ui->btn_generate->click();

            } else if (action->text() == "Contours...") {
                int contours = settings.value("surface2D/contours", 0).toInt();
                QMenu menu(this);
                QSpinBox* sbxContours = new QSpinBox(&menu);
                sbxContours->setMinimum(0);
                sbxContours->setMaximum(1000);
                sbxContours->setValue(contours);
                sbxContours->setSingleStep(1);
                sbxContours->setToolTip("Number of contours");
                QObject::connect(sbxContours, SIGNAL(valueChanged(int)),
                        this, SLOT(setNumberOfContours(int)));
                QWidgetAction* action = new QWidgetAction(&menu);
                action->setDefaultWidget(sbxContours);
                menu.addAction(action);
                menu.exec(ui->btn_options->parentWidget()->mapToGlobal(ui->btn_options->pos()));

            } else if (action->text() == "Color...") {
                QColor color = settings.value("surface2D/contourColor",
                        QColor(Qt::black)).value<QColor>();
                if (not color.isValid()) color = QColor(Qt::black);

                color = QColorDialog::getColor(color, this, "Select the color of contours");
                if (color.isValid()) {
                    settings.setValue("surface2D/contourColor", color);
                    drawContours();
                }

            } else if (action->text() == "Width...") {
                int width = settings.value("surface2D/contourWidth", 1).toInt();
                QMenu menu(this);
                QSpinBox* contourWidth = new QSpinBox(&menu);
                contourWidth->setMinimum(1);
                contourWidth->setMaximum(100);
                contourWidth->setValue(width);
                contourWidth->setSingleStep(1);
                contourWidth->setToolTip("Width of contours");
                QObject::connect(contourWidth, SIGNAL(valueChanged(int)),
                        this, SLOT(setWidthOfContours(int)));
                QWidgetAction* action = new QWidgetAction(&menu);
                action->setDefaultWidget(contourWidth);
                menu.addAction(action);
                menu.exec(ui->btn_options->parentWidget()->mapToGlobal(ui->btn_options->pos()));
            } else if (action->text() == "About...") {
                QMessageBox::information(this, "About contours",
                        "The contours are based in part on the work of "
                        "the Qwt project (http://qwt.sf.net).");
            }
        }

        void Surface2D::saveAs(const QString& filename) {
            QString format = QFileInfo(filename).suffix();
            bool append = false;
            if (format.compare("png", Qt::CaseInsensitive) != 0 and
                    format.compare("jpg", Qt::CaseInsensitive) != 0 and
                    format.compare("jpeg", Qt::CaseInsensitive) != 0 and
                    format.compare("bmp", Qt::CaseInsensitive) != 0 and
                    format.compare("tiff", Qt::CaseInsensitive) != 0) {
                QSettings settings;
                QString recentFormat = settings.value("surface2D/recentFormat").toString();
                bool ok;
                QStringList formats;
                formats << "Portable Network Graphics (PNG)"
                        << "Joint Photographic Experts Group (JPEG)"
                        << "Tagged Image File Format (TIFF)"
                        << "Windows Bitmap (BMP)";
                int recentFormatIndex = formats.indexOf(recentFormat);
                if (recentFormatIndex < 0) recentFormatIndex = 0;
                QString selectedFormat = QInputDialog::getItem(this, "Image format",
                        "Please select the image format",
                        formats, recentFormatIndex, false, &ok);
                if (not ok) return;
                settings.setValue("surface2D/recentFormat", selectedFormat);
                if (selectedFormat == formats.at(0)) format = "png";
                else if (selectedFormat == formats.at(1)) format = "jpeg";
                else if (selectedFormat == formats.at(2)) format = "tiff";
                else if (selectedFormat == formats.at(3)) format = "bmp";
                else format = "png";
                append = true;
            }
            const QPixmap* pixmap = ui->canvas->pixmap();
            if (append) {
                pixmap->save(filename + "." + format, format.toUpper().toStdString().c_str(), 100);
            } else {
                pixmap->save(filename, format.toUpper().toStdString().c_str(), 100);
            }

        }

    }
}
