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
 * Term.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_TERM_H_
#define FLQT_TERM_H_

#include <QDialog>
#include <QToolBox>
#include "ui_Term.h"



#include <QDoubleSpinBox>
#include <vector>

#include <fl/Headers.h>

namespace fl {
    namespace qt {
        class Viewer;

        class Term : public QDialog {
            Q_OBJECT

        protected slots:
            void onEngineChanged();
            
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
            
            void onChangeSpinBoxConstant(double);

            void onChangeLinearCoefficient(QListWidgetItem*);
            
            void onClickFunctionProcess();
            void onClickFunctionVariable();
            void onClickVariable(const QString&);
            void onClickFunctionBuiltIn();
            void onClickBuiltIn(const QString&);
            
            void onClickWizard();

            void redraw();
            void showSelectedTerm();

            void accept();

        protected:

            void loadTerms(scalar min, scalar max);
            void loadFrom(const fl::Term* term);
            
            fl::Term* selectedTerm() const;
            
            void connect();
            
            void setCurrentToolbox(int index);
            

            void showEvent(QShowEvent* event);
            void resizeEvent(QResizeEvent* event);

            std::vector<QDoubleSpinBox*> _sbx;
            std::vector<fl::Term*> _basicTerms;
            std::vector<fl::Term*> _extendedTerms;
            std::vector<fl::Term*> _edgeTerms;
            std::vector<fl::Term*> _fxTerms;
            
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
