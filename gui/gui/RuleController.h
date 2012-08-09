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
 * File:   RuleController.h
 * Author: jcrada
 *
 * Created on November 17, 2009, 1:22 AM
 */

#ifndef _RULECONTROLLER_H
#define	_RULECONTROLLER_H

#include "gui/GuiMainWindow.h"

#include <QtCore/QObject>
#include <fuzzylite/FuzzyLite.h>

namespace fl_gui{
    class RuleController : public QObject{
        Q_OBJECT
    protected slots:
        virtual void onClickGenerateRules();
        virtual void onClickCheckSpell();
    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual void removeAllRules();
        virtual fl::FuzzyEngine& model() const;
        virtual GuiMainWindow& view() const;

    public:
        RuleController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~RuleController();

        virtual void refreshModel();

    };
}

#endif	/* _RULECONTROLLER_H */

