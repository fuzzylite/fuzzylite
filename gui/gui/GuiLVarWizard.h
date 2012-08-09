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
 * File:   GuiLVarWizard.h
 * Author: jcrada
 *
 * Created on November 16, 2009, 9:57 PM
 */

#ifndef _GUILVARWIZARD_H
#define	_GUILVARWIZARD_H

#include <QtGui/QDialog>

#include <fuzzylite/FuzzyLite.h>
#include "gui/ui_LinguisticVariableWizard.h"


namespace fl_gui{
    class GuiLVarWizard : public QDialog{
        Q_OBJECT
    protected slots:
        virtual void onChangeMinimum(double min);
        virtual void onChangeMaximum(double max);
        virtual void onClickCreateLabels();
        virtual void accept();
    private:
        fl::LinguisticVariable* _model;
        Ui::LinguisticVariableWizard* _ui;
    protected:
        virtual fl::LinguisticVariable& getModel() const;
        virtual Ui::LinguisticVariableWizard& getUi() const;

        virtual void connect();
        virtual void disconnect();
    public:
        GuiLVarWizard(fl::LinguisticVariable& model,
                QWidget* parent = NULL, Qt::WindowFlags f = 0);
        virtual ~GuiLVarWizard();

        virtual void setup();


    };
}

#endif	/* _GUILVARWIZARD_H */

