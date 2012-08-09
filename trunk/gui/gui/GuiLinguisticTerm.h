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
 * GuiLinguisticTerm.h
 *
 *  Created on: Dec 13, 2009
 *      Author: jcrada
 */

#ifndef _GUILINGUISTICTERM_H_
#define _GUILINGUISTICTERM_H_

#include <QtGui/QDialog>
#include <fuzzylite/FuzzyLite.h>

#include "gui/ui_LinguisticTerm.h"

namespace fl_gui {

    class GuiLinguisticTerm : public QDialog {
        Q_OBJECT
    public slots:
        virtual void updateUi();
    protected slots:
        

        virtual void onChangeTriangularA(double a);
        virtual void onChangeTriangularB(double b);
        virtual void onChangeTriangularC(double c);

        virtual void onChangeRectangularMin(double min);
        virtual void onChangeRectangularMax(double max);

        virtual void onChangeTrapezoidalA(double a);
        virtual void onChangeTrapezoidalB(double b);
        virtual void onChangeTrapezoidalC(double c);
        virtual void onChangeTrapezoidalD(double d);

        virtual void onChangeShoulderMin(double min);
        virtual void onChangeShoulderMax(double max);
        virtual void onChangeShoulderSide();

        virtual void onChangeSingletonValue(double value);

        virtual void onChangeFxFunction(const QString& fx);
        virtual void onChangeFxMin(double min);
        virtual void onChangeFxMax(double max);

    private:
        Ui::LinguisticTerm* _ui;
        fl::TriangularTerm _triangular;
        fl::RectangularTerm _rectangular;
        fl::TrapezoidalTerm _trapezoidal;
        fl::ShoulderTerm _shoulder;
        fl::SingletonTerm _singleton;
        fl::FunctionTerm _fx;

    protected:

        enum eMembershipFunction {
            MF_TRIANGULAR = 0, MF_RECTANGULAR, MF_TRAPEZOIDAL, MF_SHOULDER, MF_SINGLETON, MF_FUNCTION
        };
        virtual void connect();
        virtual void disconnect();

    public:
        GuiLinguisticTerm(QWidget* parent = NULL, Qt::WindowFlags f = 0);
        virtual ~GuiLinguisticTerm();

        virtual Ui::LinguisticTerm& getUi() const;

        virtual void setup();

        virtual void showTerm(const fl::LinguisticTerm& term);

        virtual fl::LinguisticTerm* selectedTerm() const;

    };

} // namespace fuzzy_lite
#endif /* _GUILINGUISTICTERM_H_ */
