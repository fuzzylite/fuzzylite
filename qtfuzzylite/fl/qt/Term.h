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
#include "ui_Term.h"



#include <QtGui/QDoubleSpinBox>
#include <vector>

#include <fl/Headers.h>

namespace fl {
    namespace qt {
        class Viewer;

        class Term : public QDialog {
            Q_OBJECT

        protected slots:
            void onChangeToolBoxIndex(int index);
            void onChangeTab(int index);

            void onChangeSpinBoxTriangle(double);
            void onChangeSpinBoxTrapezoid(double);
            void onChangeSpinBoxRectangle(double);
            void onClickDiscreteParser();

            void onChangeSpinBoxGaussian(double);
            void onChangeSpinBoxGaussianProduct(double);
            void onChangeSpinBoxBell(double);
            void onChangeSpinBoxPiShape(double);
            void onChangeSpinBoxSigmoidDiff(double);
            void onChangeSpinBoxSigmoidProd(double);
            
            void onChangeSpinBoxRamp(double);
            void onChangeSpinBoxSigmoid(double);
            void onChangeSpinBoxSShape(double);
            void onChangeSpinBoxZShape(double);
            
            void onClickWizard();

            void redraw();
            void showSelectedTerm();

            void accept();

        protected:

            void loadTerms(scalar min, scalar max);
            void loadFrom(const fl::Term* term);
            
            fl::Term* selectedTerm() const;
            
            void connect();
            void disconnect();
            
            void setCurrentToolbox(int index);
            

            void showEvent(QShowEvent* event);
            void resizeEvent(QResizeEvent* event);

            std::vector<QDoubleSpinBox*> _sbx;
            std::vector<fl::Term*> _basicTerms;
            std::vector<fl::Term*> _extendedTerms;
            std::vector<fl::Term*> _edgeTerms;

        public:
            Ui::Term* ui;
            Viewer* viewer;
            fl::Variable* dummyVariable;
            int indexOfEditingTerm;

            Term(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            ~Term();

            void setup(const fl::Variable& variable, const fl::Term* edit = NULL);

            static void main();
        };

    }
}
#endif /* FLQT_TERM_H_ */
