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
 * File:   Surface2D.h
 * Author: jcrada
 *
 * Created on 10 July 2013, 10:09 PM
 */

#ifndef FLQT_SURFACE2D_H
#define	FLQT_SURFACE2D_H

#include <QDialog>
#include <QMouseEvent>
#include <QToolTip>

#include "ui_Surface2D.h"
#include "qwt/qwt_raster_data.h"

#include <fl/Headers.h>


namespace fl {
    namespace qt {

        class Surface2D : public QDialog {
            Q_OBJECT

        protected slots:
            void onChangeInputA(int);
            void onChangeInputB(int);
            void onChangeOutput(int);

            void onChangeSpinBox(double);

            void onClickInputs();

            void onClickOptions();
            void onOptionTriggered();
            
            void setNumberOfContours(int);
            void setWidthOfContours(int);

            void onClickMinimumColor();
            void onClickMaximumColor();

            void onClickGenerate();

            void onEngineChanged();

        protected:

            struct tuple {
                scalar inputA, inputB, output;

                tuple() {
                }

                tuple(scalar a, scalar b, scalar o) : inputA(a), inputB(b), output(o) {
                }
            };
            std::vector<std::vector<tuple> > _matrix;
            QImage _surface;
            scalar minInputA, maxInputA;
            scalar minInputB, maxInputB;
            scalar minOutput, maxOutput;
            void drawContours();

            void updateWindowTitle();
            
            void saveAs(const QString& filename);

        public:
            Ui::Surface2D* ui;
            Surface2D(QWidget* parent = 0, Qt::WindowFlags f = 0);
            ~Surface2D();

            void setup();

            class RasterData : public QwtRasterData {
            protected:
                Surface2D* _owner;
            public:

                RasterData(Surface2D* owner) : _owner(owner) {
                }

                double value(double x, double y) const {
                    return _owner->_matrix.at(x).at(y).output;
                }
            };

            class MouseTrackingFilter : public QObject {
            protected:
                Surface2D* _owner;
            public:

                MouseTrackingFilter(Surface2D* owner) : _owner(owner) {

                }

                bool eventFilter(QObject* object, QEvent* event) {
                    if (object == _owner->ui->canvas and
                            event->type() == QEvent::MouseMove) {
                        if (_owner->_matrix.size() > 0) {
                            QMouseEvent* e = (QMouseEvent*) (event);
                            QLabel* canvas = _owner->ui->canvas;
                            int x = fl::Op::scale(e->x(), 0, canvas->width(),
                                    0, _owner->_matrix.size());
                            int y = fl::Op::scale(e->y(), 0, canvas->height(),
                                    0, _owner->_matrix.at(x).size());
                            std::ostringstream ss;
                            tuple t = _owner->_matrix.at(x).at(y);
                            ss << "[" << fl::Op::str(t.inputA) << ", "
                                    << fl::Op::str(t.inputB) << "]->"
                                    << fl::Op::str(t.output);
                            QToolTip::showText(e->globalPos(),
                                    QString::fromStdString(ss.str()),
                                    _owner->ui->canvas);
                        }
                        return true;
                    }
                    return false;
                }
            };

        };
    }

}

#endif	/* FLQT_SURFACE2D_H */

