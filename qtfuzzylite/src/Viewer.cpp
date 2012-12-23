/* 
 * File:   Viewer.cpp
 * Author: jcrada
 *
 * Created on 23 December 2012, 11:14 AM
 */

#include "fl/qt/Viewer.h"

#include "fl/qt/Model.h"


#include <QtGui/QGraphicsPolygonItem>
#include <QtSvg/QSvgGenerator>


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
            ui->led_x->setVisible(false);
            setMinimumSize(200, 170);
            ui->canvas->setScene(new QGraphicsScene(ui->canvas));
            ui->canvas->setRenderHints(ui->canvas->renderHints() | QPainter::Antialiasing
                    | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
            ui->canvas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            ui->canvas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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


        }

        void Viewer::disconnect() {
            QObject::disconnect(ui->sld_x, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeSliderValue(int)));

            QObject::disconnect(ui->sld_x, SIGNAL(sliderPressed()),
                    this, SLOT(onPressSlider()));

            QObject::disconnect(ui->sld_x, SIGNAL(sliderReleased()),
                    this, SLOT(onReleaseSlider()));

            QObject::disconnect(ui->sbx_x, SIGNAL(valueChanged(double)), this,
                    SLOT(onEditInputValue()));
            QObject::disconnect(this, SIGNAL(valueChanged(double)),
                    this, SLOT(refresh()));
        }

        void Viewer::showEvent(QShowEvent*) {
            refresh();
        }

        void Viewer::resizeEvent(QResizeEvent*) {
            refresh();
        }

        /**
         * Events:
         */

        void Viewer::onPressSlider() {
            ui->sld_x->setCursor(QCursor(Qt::ClosedHandCursor));
            exportToSvg("/tmp/qtfuzzylite.svg");
        }

        void Viewer::onReleaseSlider() {
            ui->sld_x->setCursor(QCursor(Qt::OpenHandCursor));
        }

        void Viewer::onChangeSliderValue(int position) {
            scalar value = fl::Op::Scale(position,
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    constVariable->getMinimum(), constVariable->getMaximum());
            double tolerance = 1.0 / std::pow(10, ui->sbx_x->decimals());
            if (not fl::Op::IsEq(value, ui->sbx_x->value(), tolerance)) {
                ui->sbx_x->setValue(value);
            }
        }

        void Viewer::onEditInputValue() {
            scalar value = ui->sbx_x->value();
            scalar sliderValue = fl::Op::Scale(ui->sld_x->value(),
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    constVariable->getMinimum(), constVariable->getMaximum());

            double tolerance = 1.0 / std::pow(10, ui->sbx_x->decimals());
            if (not fl::Op::IsEq(value, sliderValue, tolerance)) {
                int position = (int) fl::Op::Scale(value,
                        constVariable->getMinimum(), constVariable->getMaximum(),
                        ui->sld_x->minimum(), ui->sld_x->maximum());
                if (position != ui->sld_x->value()) {
                    ui->sld_x->setValue(position);
                }
            }
            emit valueChanged(value);
        }

        void Viewer::refresh() {
            scalar x = fl::Op::Scale(ui->sld_x->value(),
                    ui->sld_x->minimum(), ui->sld_x->maximum(),
                    constVariable->getMinimum(), constVariable->getMaximum());
            scalar y = 0;
            constVariable->highestMembership(x, &y);

            ui->lbl_min->setText(QString::number(constVariable->getMinimum(), 'f', 2));
            ui->lbl_max->setText(QString::number(constVariable->getMaximum(), 'f', 2));

            QString fuzzify;
            for (int i = 0; i < constVariable->numberOfTerms(); ++i) {
                fuzzify += QString::number(constVariable->getTerm(i)->membership(x), 'f', 2)
                        + "/" + QString::fromStdString(constVariable->getTerm(i)->getName());

                if (i < constVariable->numberOfTerms() - 1) fuzzify += " + ";
            }
            ui->lbl_fuzzy->setText(fuzzify);
            draw();
            drawGuide(x, y);
        }

        void Viewer::draw() {
            ui->canvas->scene()->clear();
            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());
            QColor from = QColor(255, 255, 0, 75);
            QColor to = QColor(255, 0, 0, 175);
            for (int i = 0; i < constVariable->numberOfTerms(); ++i) {
                int r, g, b, a;
                int degree = ((i + 1.0) / (constVariable->numberOfTerms())) * 255;
                ColorGradient(degree, r, g, b, a,
                        from.red(), from.green(), from.blue(), from.alpha(),
                        to.red(), to.green(), to.blue(), to.alpha());
                draw(constVariable->getTerm(i), QColor(r, g, b, a));
            }

        }

        void Viewer::draw(const fl::Term* term, const QColor& color) {
            int line_width = 1;
            QRect rect = ui->canvas->viewport()->rect();

            scalar minimum = constVariable->getMinimum();
            scalar maximum = constVariable->getMaximum();

            std::vector<scalar> xSamples, ySamples;
            Defuzzifier* defuzzifier = Model::Default()->configuration()->getDefuzzifier();
            int divisions = defuzzifier->getDivisions();
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

            QPolygon polygon;
            scalar start = Op::Scale(xSamples[0],
                    minimum, maximum,
                    rect.left(), rect.right());
            polygon.append(QPoint(start, rect.bottom()));
            for (std::size_t j = 0; j < xSamples.size(); ++j) {
                scalar x = Op::Scale(xSamples[j], minimum, maximum,
                        rect.left(), rect.right());

                scalar y = Op::Scale(ySamples[j], 0, 1, rect.bottom(), rect.top());

                //            FL_LOG("(" << xSamples[j] << ", " << ySamples[j] << ")"
                //                    << "->(" << x << ", " << y << ")");
                polygon.append(QPoint(x, y));
            }
            scalar end = Op::Scale(xSamples[xSamples.size() - 1],
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

            drawCentroid(xcentroid, ycentroid);
        }

        void Viewer::drawCentroid(scalar xcentroid, scalar ycentroid) {
            QRect rect = ui->canvas->viewport()->rect();
            scalar x = Op::Scale(xcentroid, constVariable->getMinimum(), constVariable->getMaximum(),
                    rect.left(), rect.right());
            scalar y = Op::Scale(ycentroid, 0, 1, rect.bottom(), rect.top());
            QPen pen;
            pen.setColor(QColor(Qt::blue));
            ui->canvas->scene()->addEllipse(x, y, 3, 3, pen);
        }

        void Viewer::drawGuide(scalar x, scalar y) {
            QRect rect = ui->canvas->viewport()->rect();
            x = Op::Scale(x, constVariable->getMinimum(), constVariable->getMaximum(),
                    rect.left(), rect.right());
            y = Op::Scale(y, 0, 1, rect.bottom(), rect.top());
            QPen pen;
            pen.setColor(QColor(0, 0, 255));
            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);
            ui->canvas->scene()->addLine(x, rect.bottom(), x, y, pen);
        }

        void Viewer::ColorGradient(int degree, int& red, int& green, int& blue, int& alpha,
                int from_r, int from_g, int from_b, int from_a,
                int to_r, int to_g, int to_b, int to_a) {
            red = (int) Op::Scale(degree, 0, 255, from_r, to_r);
            green = (int) Op::Scale(degree, 0, 255, from_g, to_g);
            blue = (int) Op::Scale(degree, 0, 255, from_b, to_b);
            alpha = (int) Op::Scale(degree, 0, 255, from_a, to_a);
        }

        void Viewer::exportToSvg(const std::string& filepath) {
            (void) filepath;
            QSvgGenerator svgGen;

            svgGen.setFileName(QString::fromStdString(filepath));
            svgGen.setSize(ui->canvas->viewport()->size());
            svgGen.setViewBox(ui->canvas->viewport()->rect());
            svgGen.setTitle("qtfuzzylite");
            svgGen.setDescription("A fuzzy logic controller graphic user interface written in Qt");

            QBrush background = QBrush(Qt::white);
            QPainter painter;
            painter.begin(&svgGen);
            painter.fillRect(ui->canvas->viewport()->rect(), background);
            ui->canvas->scene()->render(&painter);
            painter.end();
        }

        void Viewer::main() {
            Example1 ex1;

            Viewer* window = new Viewer;
            window->setup(ex1.engine->getInputVariable(0));
            window->show();
        }
    }


}