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
 * GuiCanvas.h
 *
 *  Created on: Dec 18, 2009
 *      Author: jcrada
 */

#ifndef _GUICANVAS_H_
#define _GUICANVAS_H_

#include <QtGui/QGraphicsView>
#include <QtGui/QResizeEvent>
#include <fuzzylite/FuzzyLite.h>
#include <QtGui/QColor>

namespace fl_gui {

    class GuiCanvas : public QGraphicsView {
        Q_OBJECT
    private:
        fl::flScalar _minimum, _maximum;
    protected:
        void resizeEvent(QResizeEvent *event);
        virtual void ensureVisible();
    public:
        GuiCanvas(QWidget* parent = NULL);
        virtual ~GuiCanvas();

        virtual fl::flScalar minimumLVar();
        virtual void setMinimumLVar(fl::flScalar min);
        virtual fl::flScalar maximumLVar();
        virtual void setMaximumLVar(fl::flScalar max);

        virtual QRect drawingRect() const;
        virtual void drawVariable(const fl::LinguisticVariable& var, const QColor& from =
                QColor(255, 255, 0, 75), const QColor& to = QColor(255, 0, 0, 175));
        virtual void drawTerm(const fl::LinguisticTerm& term, const QColor& color =
                QColor(0, 204, 0, 230));
        virtual void drawGuide(fl::flScalar x, fl::flScalar y, const QColor& color =
                QColor(0, 210, 0, 255));
        virtual void drawCentroid(fl::flScalar x, fl::flScalar y);

        virtual void clear();

        static void ColorGradientF(fl::flScalar from_r, fl::flScalar from_g, fl::flScalar from_b,
                fl::flScalar from_a, fl::flScalar to_r, fl::flScalar to_g, fl::flScalar to_b, fl::flScalar to_a,
                fl::flScalar degree, fl::flScalar& red, fl::flScalar& green, fl::flScalar& blue, fl::flScalar& alpha);
        static void ColorGradient(int from_r, int from_g, int from_b,
                int from_a, int to_r, int to_g, int to_b, int to_a,
                int degree, int& red, int& green, int& blue, int& alpha);

    };

} // namespace fl_gui
#endif /* _GUICANVAS_H_ */
