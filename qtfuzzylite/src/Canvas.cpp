/*
 * Canvas.cpp
 *
 *  Created on: 10/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Canvas.h"

#include "fl/qt/Model.h"

#include <QtGui/QGraphicsPolygonItem>
#include <unistd.h>
namespace fl {
    namespace qt {

        Canvas::Canvas(QWidget* parent)
        : QGraphicsView(new QGraphicsScene, parent),
        _minimum(-std::numeric_limits<scalar>::infinity()),
        _maximum(std::numeric_limits<scalar>::infinity()) {
            setRenderHints(renderHints() | QPainter::Antialiasing
                    | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }

        Canvas::~Canvas() {
            delete scene();
        }

        void Canvas::setMinimum(scalar minimum) {
            this->_minimum = minimum;
        }

        scalar Canvas::getMinimum() const {
            return this->_minimum;
        }

        void Canvas::setMaximum(scalar maximum) {
            this->_maximum = maximum;
        }

        scalar Canvas::getMaximum() const {
            return this->_maximum;
        }

        void Canvas::clear() {
            scene()->clear();
            scene()->setSceneRect(viewport()->rect());
            setMinimum(-std::numeric_limits<scalar>::infinity());
            setMaximum(std::numeric_limits<scalar>::infinity());
        }

        void Canvas::resizeEvent(QResizeEvent*) {
            //            if (e->size() - e->oldSize() != QSize(0, 0)) {
            //                QSizeF ratio((qreal) e->size().width() / e->oldSize().width(),
            //                        (qreal) e->size().height() / e->oldSize().height());
            //                if (ratio.width() > 0 or ratio.height() > 0)
            //                    scale(ratio.width(), ratio.height());
            //            }
            //            FL_LOG("Canvas resized"); 
        }

        QRect Canvas::drawingRect() const {
            return viewport()->rect();
        }

        void Canvas::draw(const fl::Variable* variable,
                const QColor& from, const QColor& to) {
            draw(variable->terms(), variable->getMinimum(), variable->getMaximum(),
                    from, to);
//            setMinimum(variable->getMinimum());
//            setMaximum(variable->getMaximum());
//            for (int i = 0; i < variable->numberOfTerms(); ++i) {
//                int r, g, b, a;
//                int degree = ((i + 1.0) / (variable->numberOfTerms())) * 255;
//                ColorGradient(degree, r, g, b, a,
//                        from.red(), from.green(), from.blue(), from.alpha(),
//                        to.red(), to.green(), to.blue(), to.alpha());
//                draw(variable->getTerm(i), QColor(r, g, b, a));
//            }
        }

        void Canvas::draw(const std::vector<fl::Term*>& terms,
                scalar minimum, scalar maximum, const QColor& from, const QColor& to) {
            setMinimum(minimum);
            setMaximum(maximum);
            for (std::size_t i = 0; i < terms.size(); ++i) {
                int r, g, b, a;
                int degree = ((i + 1.0) / (terms.size())) * 255;
                ColorGradient(degree, r, g, b, a,
                        from.red(), from.green(), from.blue(), from.alpha(),
                        to.red(), to.green(), to.blue(), to.alpha());
                draw(terms[i], QColor(r, g, b, a));
            }
        }

        void Canvas::draw(const Term* term, const QColor& color) {
            int line_width = 1;
            QRect rect = drawingRect();

            std::vector<scalar> xSamples, ySamples;
            Defuzzifier* defuzzifier = Model::Default()->configuration()->getDefuzzifier();
            int divisions = defuzzifier->getDivisions();
            scalar dx = (_maximum - _minimum) / divisions;
            scalar area = 0;
            scalar ycentroid = 0;
            for (int i = 0; i < divisions; ++i) {
                scalar x = _minimum + (i + 0.5) * dx;
                scalar y = term->membership(x);
                ycentroid += y * y;
                area += y;
                xSamples.push_back(x);
                ySamples.push_back(y);
            }
            ycentroid /= 2 * area;
            area *= dx;

            scalar xcentroid = defuzzifier->defuzzify(term, _minimum, _maximum);

            QPolygon polygon;
            scalar start = Op::Scale(xSamples[0],
                    _minimum, _maximum,
                    rect.left(), rect.right());
            polygon.append(QPoint(start, rect.bottom()));
            for (std::size_t j = 0; j < xSamples.size(); ++j) {
                scalar x = Op::Scale(xSamples[j], _minimum, _maximum,
                        rect.left(), rect.right());

                scalar y = Op::Scale(ySamples[j], 0, 1, rect.bottom(), rect.top());

                //            FL_LOG("(" << xSamples[j] << ", " << ySamples[j] << ")"
                //                    << "->(" << x << ", " << y << ")");
                polygon.append(QPoint(x, y));
            }
            scalar end = Op::Scale(xSamples[xSamples.size() - 1],
                    _minimum, _maximum,
                    rect.left(), rect.right());
            polygon.append(QPoint(end, rect.bottom()));

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

            //        for (std::size_t i = 0 ; i < samples_x.size() ; ++i){
            //            scene()->addEllipse(samples_x[i],samples_y[i],1, 1, pen);
            //        }

            drawCentroid(xcentroid, ycentroid);
        }

        void Canvas::drawGuide(scalar x, scalar y, const QColor& color) {
            QRect rect = drawingRect();
            x = Op::Scale(x, _minimum, _maximum, rect.left(),
                    rect.right());
            y = Op::Scale(y, 0, 1, rect.bottom(), rect.top());
            QPen pen;
            pen.setColor(color);
            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);
            scene()->addLine(x, rect.bottom(), x, y, pen);
        }

        void Canvas::drawCentroid(scalar x, scalar y) {
            QRect rect = drawingRect();
            x = Op::Scale(x, _minimum, _maximum, rect.left(), rect.right());
            y = Op::Scale(y, 0, 1, rect.bottom(), rect.top());
            QPen pen;
            pen.setColor(QColor(Qt::blue));
            scene()->addEllipse(x, y, 3, 3, pen);
        }

        void Canvas::ColorGradientF(scalar from_r, scalar from_g, scalar from_b, scalar from_a,
                scalar to_r, scalar to_g, scalar to_b, scalar to_a, scalar degree,
                scalar& red, scalar& green, scalar& blue, scalar& alpha) {
            red = Op::Scale(degree, 0, 1, from_r, to_r);
            green = Op::Scale(degree, 0, 1, from_g, to_g);
            blue = Op::Scale(degree, 0, 1, from_b, to_b);
            alpha = Op::Scale(degree, 0, 1, from_a, to_a);
        }

        void Canvas::ColorGradient(int degree, int& red, int& green, int& blue, int& alpha,
                int from_r, int from_g, int from_b, int from_a,
                int to_r, int to_g, int to_b, int to_a) {
            red = (int) Op::Scale(degree, 0, 255, from_r, to_r);
            green = (int) Op::Scale(degree, 0, 255, from_g, to_g);
            blue = (int) Op::Scale(degree, 0, 255, from_b, to_b);
            alpha = (int) Op::Scale(degree, 0, 255, from_a, to_a);
        }

        void Canvas::main() {
            Canvas* c = new Canvas;
            c->show();
            Example1 sm;
            c->draw(sm.engine->getInputVariable(0));

        }
    }
}
