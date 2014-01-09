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
 * File:   Viewer.cpp
 * Author: jcrada
 *
 * Created on 23 December 2012, 11:14 AM
 */

#include "fl/qt/Viewer.h"

#include "fl/qt/Model.h"

#include "fl/qt/Window.h"
#include "fl/qt/qtfuzzylite.h"

#include <QGraphicsPolygonItem>

#include <QMenu>
#include <QSignalMapper>
#include <QAction>
#include <QSettings>

#include <vector>


//#define FL_EXPORT_SVG
#ifdef FL_EXPORT_SVG
#include <QtSvg/QSvgGenerator>
#endif

namespace fl {
    namespace qt {

        Viewer::Viewer(QWidget* parent, Qt::WindowFlags f) :
        QWidget(parent, f), constVariable(NULL), ui(new Ui::Viewer) {
        }

        Viewer::~Viewer() {
            delete ui;
        }

        void Viewer::setup(const fl::Variable* model) {
            this->constVariable = model;
            ui->setupUi(this);
            ui->sbx_x->setSingleStep(fl::Op::max(scalar(0.01),
                    model->getMaximum() - model->getMinimum()) / 100);
            ui->sbx_x->setDecimals(fuzzylite::decimals());

            ui->btn_name->setText(QString::fromStdString(model->getName()));
            if (constVariable->getName().empty())
                ui->btn_name->setVisible(false);
            ui->led_x->setVisible(false);
            setMinimumSize(200, 170);
            ui->canvas->setScene(new QGraphicsScene(ui->canvas));
            ui->canvas->setRenderHints(QPainter::Antialiasing
                    | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing |
                    QPainter::NonCosmeticDefaultPen);
            ui->canvas->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            ui->canvas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            ui->canvas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            ui->sbx_x->setFocus();
            ui->lbl_fuzzy_out->setVisible(false);

            QFont smallFont = ui->lbl_fuzzy->font();
#ifdef Q_OS_WIN
            smallFont.setPointSize(smallFont.pointSize() - 1);
#else
            smallFont.setPointSize(smallFont.pointSize() - 2);
#endif
            ui->lbl_fuzzy->setFont(smallFont);
            ui->lbl_fuzzy_out->setFont(smallFont);
            ui->btn_name->setEnabled(false);
            connect();
        }

        void Viewer::connect() {
            QObject::connect(ui->sld_x, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderValue(int)));

            QObject::connect(ui->sld_x, SIGNAL(sliderPressed()),
                    this, SLOT(onPressSlider()));

            QObject::connect(ui->sld_x, SIGNAL(sliderReleased()),
                    this, SLOT(onReleaseSlider()));

            QObject::connect(ui->sbx_x, SIGNAL(valueChanged(double)), this,
                    SLOT(onEditInputValue()));
            QObject::connect(this, SIGNAL(valueChanged(double)),
                    this, SLOT(refresh()));

            QObject::connect(this, SIGNAL(signalRefresh()),
                    this, SLOT(refresh()), Qt::QueuedConnection);

        }

        void Viewer::showEvent(QShowEvent*) {
            refresh();
        }

        void Viewer::resizeEvent(QResizeEvent*) {
            refresh();
        }

        void Viewer::enablePropertiesButton(bool show) {
            ui->btn_name->setVisible(show);
        }

        void Viewer::minimizeViewer() {
            ui->wdg_canvas->setVisible(false);
            ui->wdg_out->setVisible(false);
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            setFixedHeight(minimumSizeHint().height());
        }

        void Viewer::maximizeViewer() {
            ui->wdg_canvas->setVisible(true);
            ui->wdg_out->setVisible(true);

            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setMinimumSize(0, 0);
            setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        }

        bool Viewer::isMinimizedViewer() const {
            return not ui->wdg_canvas->isVisible();
        }

        bool Viewer::isMaximizedViewer() const {
            return ui->wdg_canvas->isVisible();
        }

        /**
         * Events:
         */

        void Viewer::onPressSlider() {
            ui->sld_x->setCursor(QCursor(Qt::ClosedHandCursor));
#ifdef FL_EXPORT_SVG
            ui->canvas->viewport()->setFixedSize(128, 128);
            exportToSvg("/tmp/qtfuzzylite.svg");
#endif
        }

        void Viewer::onReleaseSlider() {
            ui->sld_x->setCursor(QCursor(Qt::OpenHandCursor));
        }

        void Viewer::onChangeSliderValue(int position) {
            scalar value = fl::Op::scale(position,
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    constVariable->getMinimum(), constVariable->getMaximum());
            double tolerance = 1.0 / std::pow(10.0, ui->sbx_x->decimals());
            if (not fl::Op::isEq(value, ui->sbx_x->value(), tolerance)) {
                ui->sbx_x->setValue(value);
            }
        }

        void Viewer::onEditInputValue() {
            scalar value = ui->sbx_x->value();
            scalar sliderValue = fl::Op::scale(ui->sld_x->value(),
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    constVariable->getMinimum(), constVariable->getMaximum());

            double tolerance = 1.0 / std::pow(10.0, ui->sbx_x->decimals());
            if (not fl::Op::isEq(value, sliderValue, tolerance)) {
                int position = (int) fl::Op::scale(value,
                        constVariable->getMinimum(), constVariable->getMaximum(),
                        ui->sld_x->minimum(), ui->sld_x->maximum());
                if (position != ui->sld_x->value()) {
                    ui->sld_x->setValue(position);
                }
            }
            emit valueChanged(value);
        }

        void Viewer::refresh() {
            ui->sbx_x->setSingleStep(
                    (constVariable->getMaximum() - constVariable->getMinimum()) / 100);
            scalar x = fl::Op::scale(ui->sld_x->value(),
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    constVariable->getMinimum(), constVariable->getMaximum());
            scalar y = 0;
            constVariable->highestMembership(x, &y);

            ui->lbl_min->setText(QString::fromStdString(
                    fl::Op::str(constVariable->getMinimum())));
            ui->lbl_max->setText(QString::fromStdString(
                    fl::Op::str(constVariable->getMaximum())));

            QString fuzzify = QString::fromStdString(constVariable->fuzzify(x));

            ui->lbl_fuzzy->setText("&#956;=" + fuzzify);
            ui->btn_name->setText(QString::fromStdString(constVariable->getName()));
            if (constVariable->getName().empty())
                ui->btn_name->setVisible(false);
            draw();
#ifndef FL_EXPORT_SVG
            drawGuide(x, y);
#endif
        }

        void Viewer::draw() {
            ui->canvas->scene()->clear();
            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());

            QColor from = QColor(255, 255, 0, (0.33 + .1) * 255);
            QColor to = QColor(255, 0, 0, (0.66 + .1) * 255);
#ifdef FL_EXPORT_SVG
            //No transparency
            from = QColor(255, 255, 0, 255);
            to = QColor(255, 0, 0, 255);
#endif
            for (int i = 0; i < constVariable->numberOfTerms(); ++i) {
                QColor color;
                if (constVariable->numberOfTerms() == 1) {
                    color = to;
                } else {
                    int degree = (i / (constVariable->numberOfTerms() - 1.0)) * 255;
                    color = Window::mainWindow()->gradient(degree, from, to);
                }
                draw(constVariable->getTerm(i), color);
            }

        }

        void Viewer::draw(const fl::Term* term, const QColor& color) {
            if (term->className() == Constant().className() or
                    term->className() == Linear().className()) {
                return;
            }
            int line_width = 3;
            QRect rect = ui->canvas->viewport()->rect();

            scalar minimum = constVariable->getMinimum();
            scalar maximum = constVariable->getMaximum();

            QSettings settings;
            int divisions = settings.value("view/termResolution", 1000).toInt();

            std::vector<scalar> xSamples, ySamples;
            scalar dx = (maximum - minimum) / divisions;
            scalar area = 0;
            scalar xcentroid = 0, ycentroid = 0;
            for (int i = 0; i < divisions; ++i) {
                scalar x = minimum + (i + 0.5) * dx;
                scalar y = term->membership(x);
                xcentroid += x* y;
                ycentroid += y * y;
                area += y;
                xSamples.push_back(x);
                ySamples.push_back(y);
            }
            xcentroid /= area;
            ycentroid /= 2 * area;
            area *= dx;
            (void) area;
            QPolygon polygon;
            scalar start = fl::Op::scale(xSamples[0],
                    minimum, maximum,
                    rect.left(), rect.right());
            polygon.append(QPoint(start, rect.bottom()));
            for (std::size_t j = 0; j < xSamples.size(); ++j) {
                scalar x = fl::Op::scale(xSamples[j], minimum, maximum,
                        rect.left(), rect.right());

                scalar y = fl::Op::scale(ySamples[j], 0, 1, rect.bottom(), rect.top());

                polygon.append(QPoint(x, y));
            }
            scalar end = fl::Op::scale(xSamples[xSamples.size() - 1],
                    minimum, maximum,
                    rect.left(), rect.right());
            polygon.append(QPoint(end, rect.bottom()));

            QPen pen;
            pen.setWidth(line_width);
            pen.setColor(QColor(Qt::black));
            pen.setStyle(Qt::SolidLine);
            QBrush brush;
            brush.setColor(color);
            brush.setStyle(Qt::SolidPattern);
            QGraphicsPolygonItem* item = ui->canvas->scene()->addPolygon(polygon, pen, brush);
            static qreal zvalue = INT_MIN; //each figure will be on top.
            item->setZValue(++zvalue);

#ifndef FL_EXPORT_SVG
            drawCentroid(xcentroid, ycentroid);
#endif
        }

        void Viewer::drawCentroid(scalar xcentroid, scalar ycentroid) {
            QRect rect = ui->canvas->viewport()->rect();
            scalar x = fl::Op::scale(xcentroid, constVariable->getMinimum(), constVariable->getMaximum(),
                    rect.left(), rect.right());
            scalar y = fl::Op::scale(ycentroid, 0, 1, rect.bottom(), rect.top());
            QPen pen;
            pen.setColor(QColor(Qt::blue));
            ui->canvas->scene()->addEllipse(x, y, 3, 3, pen);
        }

        void Viewer::drawGuide(scalar x, scalar y) {
            QRect rect = ui->canvas->viewport()->rect();
            x = fl::Op::scale(x, constVariable->getMinimum(), constVariable->getMaximum(),
                    rect.left(), rect.right());
            y = fl::Op::scale(y, 0, 1, rect.bottom(), rect.top());
            QPen pen;
            pen.setColor(QColor(0, 0, 255));
            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);
            ui->canvas->scene()->addLine(x, rect.bottom(), x, y, pen);
        }

        void Viewer::exportToSvg(const std::string& filepath) {
            (void) filepath;
#ifdef FL_EXPORT_SVG
            QSvgGenerator svgGen;

            svgGen.setFileName(QString::fromStdString(filepath));
            svgGen.setSize(ui->canvas->viewport()->size());
            svgGen.setViewBox(ui->canvas->viewport()->rect());
            svgGen.setTitle("qtfuzzylite");
            svgGen.setDescription("A fuzzy logic controller graphic user interface written in Qt");

            /* To paint background white instead of transparent
            QBrush background = QBrush(Qt::white);
            painter.fillRect(ui->canvas->viewport()->rect(), background);
             */
            QPainter painter;
            painter.begin(&svgGen);
            ui->canvas->scene()->render(&painter);
            painter.end();
#endif
        }

    }


}
