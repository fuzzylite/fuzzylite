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
            void onChangeToolBoxIndex(int index);
            void onChangeTab(int index);

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
            void loadFrom(const fl::Term* term);

            void showEvent(QShowEvent* event);
            void resizeEvent(QResizeEvent* event);

            std::vector<QDoubleSpinBox*> _sbx;
            std::vector<fl::Term*> _basicTerms;
            std::vector<fl::Term*> _extendedTerms;

        public:
            Ui::Term* ui;

            Term(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Term();

            virtual void setup();
            virtual fl::Term* copySelectedTerm() const;
            virtual void edit(const fl::Term* term);
            virtual void refresh();

            virtual fl::Term* getSelectedTerm() const;
            static void main();
        };

    } /* namespace qt */
} /* namespace fl */
#endif /* FLQT_TERM_H_ */
