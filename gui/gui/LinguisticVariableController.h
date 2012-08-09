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
 * File:   LinguisticVariableController.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 9:53 PM
 */

#ifndef _LINGUISTICVARIABLECONTROLLER_H
#define	_LINGUISTICVARIABLECONTROLLER_H

#include <QtCore/QObject>

#include "gui/ui_LinguisticVariable.h"
#include "gui/GuiLinguisticVariable.h"
#include "gui/GuiGrapher.h"

#include <fuzzylite/FuzzyLite.h>

namespace fl_gui{
    class LinguisticVariableController : public QObject{
        Q_OBJECT
    protected slots:
        virtual void onClickAddTerm();
        virtual void onClickEditTerm();
        virtual void onClickRemoveTerm();
        virtual void onClickWizard();

        virtual void onChangeName(const QString&);

        virtual void onChangeTerm();
        virtual void onDoubleClickTerm(QListWidgetItem* item);

        virtual void draw();
    private:
        fl::LinguisticVariable* _model;
        GuiLinguisticVariable* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual fl::LinguisticVariable& model() const;
        virtual GuiLinguisticVariable& view() const;
    public:
        LinguisticVariableController(fl::LinguisticVariable& model, GuiLinguisticVariable& view);
        virtual ~LinguisticVariableController();

        virtual void saveModel();
        virtual void refreshModel();

    };
}

#endif	/* _LINGUISTICVARIABLECONTROLLER_H */

