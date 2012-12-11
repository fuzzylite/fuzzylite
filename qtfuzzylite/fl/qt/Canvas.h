/*
 * Canvas.h
 *
 *  Created on: 10/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_CANVAS_H_
#define FLQT_CANVAS_H_

#include <QtGui/QGraphicsView>
#include <QtGui/QResizeEvent>
#include <QtGui/QColor>

#include <fl/Headers.h>

namespace fl {
    namespace qt {
        class Canvas: public QGraphicsView {
        Q_OBJECT

        protected:
            scalar _minimum;
            scalar _maximum;
            void resizeEvent(QResizeEvent *event);

        public:
            Canvas(QWidget* parent = NULL);
            virtual ~Canvas();

            virtual void setMinimum(scalar minimum);
            virtual scalar getMinimum() const;

            virtual void setMaximum(scalar maximum);
            virtual scalar getMaximum() const;

            virtual QRect drawingRect() const;

            virtual void draw(const Variable* variable,
                    const QColor& from = QColor(255, 255, 0, 75),
                    const QColor& to = QColor(255, 0, 0, 175));

            virtual void draw(const Term* term,
                    const QColor& color = QColor(0, 204, 0, 230));

            virtual void drawGuide(scalar x, scalar y, const QColor& color =
                    QColor(0, 210, 0, 255));

            virtual void drawCentroid(scalar x, scalar y);

            virtual void clear();

            static void ColorGradientF(scalar from_r, scalar from_g, scalar from_b,
                    scalar from_a, scalar to_r, scalar to_g, scalar to_b, scalar to_a,
                    scalar degree, scalar& red, scalar& green, scalar& blue, scalar& alpha);

            static void ColorGradient(int degree, int& red, int& green, int& blue, int& alpha,
                    int from_r, int from_g, int from_b,
                    int from_a, int to_r, int to_g, int to_b, int to_a);

            static void main();
        };
    }
} /* namespace fl */
#endif /* FLQT_CANVAS_H_ */
