/*
 * Term.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_TERM_H_
#define FLQT_TERM_H_

#include <QtGui/QDialog>
#include <QtGui/QToolBox>
#include "ui/ui_Term.h"

#include <QtGui/QDoubleSpinBox>
#include <vector>

namespace fl {
    namespace qt {

        class Term: public QDialog {
        Q_OBJECT

        protected slots:
            void onChangeSpinBoxTriangle(double dummyValue);
            void onChangeSpinBoxTrapezoid(double dummyValue);
            void onChangeSpinBoxRectangle(double dummyValue);
            void onChangeSpinBoxLeftShoulder(double dummyValue);
            void onChangeSpinBoxRightShoulder(double dummyValue);
            void onClickDiscreteParser();
            void onChangeSpinBoxGaussian(double dummyValue);
            void onChangeSpinBoxBell(double dummyValue);
            void onChangeSpinBoxSigmoid(double dummyValue);

        protected:
            QToolBox* _termToolBox;
            void connect();
            void disconnect();

            std::vector<QDoubleSpinBox*> _sbx;
            std::vector<fl::Term*> _basicTerms;
            std::vector<fl::Term*> _advancedTerms;

        public:
            Ui::Term* ui;

            Term(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Term();

            virtual void setup();

            virtual void refresh();

            virtual fl::Term* getSelectedTerm() const;
            static void main();
        };

    } /* namespace qt */
} /* namespace fl */
#endif /* FLQT_TERM_H_ */
