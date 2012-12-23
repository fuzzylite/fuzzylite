/* 
 * File:   Viewer.h
 * Author: jcrada
 *
 * Created on 23 December 2012, 11:13 AM
 */

#ifndef FLQT_VIEWER_H
#define	FLQT_VIEWER_H

#include <QtGui/QWidget>
#include "ui/ui_Viewer.h"

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

        protected:
            virtual void connect();
            virtual void disconnect();
            virtual void showEvent(QShowEvent*);
            virtual void resizeEvent(QResizeEvent*);



        public:
            const fl::Variable* constVariable;
            Ui::Viewer* ui;

            Viewer(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Viewer();

            virtual void setup(const fl::Variable* variable);

            virtual void draw();
            virtual void draw(const fl::Term* term, const QColor& color = QColor(0, 210, 0, 200));
            virtual void drawCentroid(scalar xcentroid, scalar ycentroid);
            virtual void drawGuide(scalar x, scalar y);

            virtual void exportToSvg(const std::string& filepath);

            static void ColorGradient(int degree, int& red, int& green, int& blue, int& alpha,
                    int from_r, int from_g, int from_b,
                    int from_a, int to_r, int to_g, int to_b, int to_a);

            static void main();

        };
    }
}

#endif	/* FLQT_VIEWER_H */

