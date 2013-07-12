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
 * File:   Viewer.h
 * Author: jcrada
 *
 * Created on 23 December 2012, 11:13 AM
 */

#ifndef FLQT_VIEWER_H
#define	FLQT_VIEWER_H

#include <QWidget>
#include "ui_Viewer.h"

#include <fl/Headers.h>


namespace fl {
    namespace qt {

        class Viewer : public QWidget {
            Q_OBJECT



        protected slots:
            virtual void onChangeSliderValue(int position);
            virtual void onEditInputValue();

            virtual void onPressSlider();
            virtual void onReleaseSlider();

        public slots:
            virtual void refresh();

        signals:
            void valueChanged(double);
            void signalRefresh();

        protected:

            virtual void connect();
            virtual void showEvent(QShowEvent*);
            virtual void resizeEvent(QResizeEvent*);



        public:
            const fl::Variable* constVariable;
            Ui::Viewer* ui;

            Viewer(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Viewer();

            virtual void setup(const fl::Variable* variable);

            virtual void enablePropertiesButton(bool show);
            virtual void minimizeViewer();
            virtual void maximizeViewer();
            bool isMinimizedViewer() const ;
            bool isMaximizedViewer() const ;
            
            virtual void draw();
            virtual void draw(const fl::Term* term, const QColor& color = QColor(0, 210, 0, 200));
            virtual void drawCentroid(scalar xcentroid, scalar ycentroid);
            virtual void drawGuide(scalar x, scalar y);

            virtual void exportToSvg(const std::string& filepath);
        };
    }
}

#endif	/* FLQT_VIEWER_H */

