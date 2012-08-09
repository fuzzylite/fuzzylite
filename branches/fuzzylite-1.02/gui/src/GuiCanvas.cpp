/*   Copyright 2010 Juan Rada-Vilela

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
/*
 * GuiCanvas.cpp
 *
 *  Created on: Dec 20, 2009
 *      Author: jcrada
 */

#include "gui/GuiCanvas.h"

#include <QtGui/QGraphicsPolygonItem>
#include <limits.h>
namespace fl_gui {

    GuiCanvas::GuiCanvas(QWidget* parent) :
    QGraphicsView(new QGraphicsScene, parent), _minimum(-INFINITY),
    _maximum(INFINITY) {
        setRenderHints(renderHints() | QPainter::Antialiasing
                | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    GuiCanvas::~GuiCanvas() {
        delete scene();
    }

    fl::flScalar GuiCanvas::minimumLVar() {
        return _minimum;
    }

    void GuiCanvas::setMinimumLVar(fl::flScalar min) {
        this->_minimum = min;
    }

    fl::flScalar GuiCanvas::maximumLVar() {
        return _maximum;
    }

    void GuiCanvas::setMaximumLVar(fl::flScalar max) {
        this->_maximum = max;
    }

    void GuiCanvas::clear() {
        scene()->clear();
    }

    void GuiCanvas::resizeEvent(QResizeEvent* e) {
        (void) e;
        ensureVisible();
    }

    void GuiCanvas::ensureVisible() {
        fitInView(drawingRect(), Qt::IgnoreAspectRatio);
        centerOn(drawingRect().center());
    }

    QRect GuiCanvas::drawingRect() const {
        int line_width = 2;
        int slider = 15; //Slider ball in mac 10.6
        QRect result = viewport()->rect();
        fl::flScalar sides = 0.0;
        result.translate(-result.width() / 2, -result.height() / 2);
        int min_x_canvas = result.left() + slider / 2 + sides, max_x_canvas =
                result.right() - slider / 2 - sides, //15 is size of slider ball in macosx
                min_y_canvas = 0 + (result.height() * 1 / 2) - line_width, max_y_canvas = 0
                - (result.height() * 1 / 2) + line_width;
        result.setLeft(min_x_canvas);
        result.setRight(max_x_canvas);
        result.setTop(max_y_canvas);
        result.setBottom(min_y_canvas);
        return result;
    }

    void GuiCanvas::drawVariable(const fl::LinguisticVariable& var, const QColor& from,
            const QColor& to) {
        setMinimumLVar(var.minimum());
        setMaximumLVar(var.maximum());
        for (int i = 0; i < var.numberOfTerms(); ++i) {
            int r, g, b, a;
            int degree = ((i + 1.0) / (var.numberOfTerms())) * 255;
            ColorGradient(from.red(), from.green(), from.blue(), from.alpha(),
                    to.red(), to.green(), to.blue(), to.alpha(),
                    degree, r, g, b, a);
            drawTerm(*var.term(i), QColor(r, g, b, a));
        }
    }

    void GuiCanvas::drawTerm(const fl::LinguisticTerm& term, const QColor& color) {
        int line_width = 1;
        QRect rect = drawingRect();

        std::vector<fl::flScalar> samples_x, samples_y;
//        term.samples(samples_x, samples_y, term.fuzzyOperator().numberOfSamples(), 2);
        term.samples(samples_x, samples_y, term.fuzzyOperator().numberOfSamples(), 0);
        QPolygon polygon;
        polygon.append(QPoint(rect.left(), rect.bottom()));
        for (size_t j = 0; j < samples_x.size(); ++j) {
            fl::flScalar x = fl::FuzzyOperation::Scale(minimumLVar(), maximumLVar(),
                    samples_x[j], rect.left(), rect.right());
            fl::flScalar y = fl::FuzzyOperation::Scale(0, 1, samples_y[j], rect.bottom(),
                    rect.top());
            if (isnan(x + y) || isinf(x + y) ){
//                FL_LOG("NaN or Inf found: x=" << x << "\ty=" << y);
                continue;
            }
            polygon.append(QPoint(x, y));
        }
        polygon.append(QPoint(rect.right(), rect.bottom()));

        QPen pen;
        pen.setWidth(line_width);
        pen.setColor(QColor(Qt::black));
        pen.setStyle(Qt::SolidLine);
        QBrush brush;
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);
        QGraphicsPolygonItem* item = scene()->addPolygon(polygon, pen, brush);
        static qreal zvalue = INT_MIN; //each figure will be on top.
        item->setZValue(++zvalue);

//        for (size_t i = 0 ; i < samples_x.size() ; ++i){
//            scene()->addEllipse(samples_x[i],samples_y[i],1, 1, pen);
//        }

        fl::flScalar centroid_x, centroid_y;
        term.centroid(centroid_x, centroid_y);
        drawCentroid(centroid_x, centroid_y);
        ensureVisible();
    }

    void GuiCanvas::drawGuide(fl::flScalar x, fl::flScalar y, const QColor& color) {
        QRect rect = drawingRect();
        x = fl::FuzzyOperation::Scale(minimumLVar(), maximumLVar(), x, rect.left(),
                rect.right());
        y = fl::FuzzyOperation::Scale(0, 1, y, rect.bottom(), rect.top());
        QPen pen;
        pen.setColor(color);
        pen.setStyle(Qt::DashLine);
        pen.setWidth(1);
        scene()->addLine(x, rect.bottom(), x, y, pen);
    }

    void GuiCanvas::drawCentroid(fl::flScalar x, fl::flScalar y) {
        QRect rect = drawingRect();
        x = fl::FuzzyOperation::Scale(minimumLVar(), maximumLVar(), x, rect.left(),
                rect.right());
        y = fl::FuzzyOperation::Scale(0, 1, y, rect.bottom(), rect.top());
        QPen pen;
        pen.setColor(QColor(Qt::blue));
        scene()->addEllipse(x, y, 3, 3, pen);
    }

    void GuiCanvas::ColorGradientF(fl::flScalar from_r, fl::flScalar from_g, fl::flScalar from_b, fl::flScalar from_a,
            fl::flScalar to_r, fl::flScalar to_g, fl::flScalar to_b, fl::flScalar to_a, fl::flScalar degree,
            fl::flScalar& red, fl::flScalar& green, fl::flScalar& blue, fl::flScalar& alpha) {
        red = fl::FuzzyOperation::Scale(0, 1, degree, from_r, to_r);
        green = fl::FuzzyOperation::Scale(0, 1, degree, from_g, to_g);
        blue = fl::FuzzyOperation::Scale(0, 1, degree, from_b, to_b);
        alpha = fl::FuzzyOperation::Scale(0, 1, degree, from_a, to_a);
    }

    void GuiCanvas::ColorGradient(int from_r, int from_g, int from_b, int from_a,
            int to_r, int to_g, int to_b, int to_a, int degree,
            int& red, int& green, int& blue, int& alpha) {
        red = (int) fl::FuzzyOperation::Scale(0, 255, degree, from_r, to_r);
        green = (int) fl::FuzzyOperation::Scale(0, 255, degree, from_g, to_g);
        blue = (int) fl::FuzzyOperation::Scale(0, 255, degree, from_b, to_b);
        alpha = (int) fl::FuzzyOperation::Scale(0, 255, degree, from_a, to_a);
    }

} // namespace fl_gui
