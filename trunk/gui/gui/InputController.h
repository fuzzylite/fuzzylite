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
 * File:   InputController.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 9:38 PM
 */

#ifndef _INPUTCONTROLLER_H
#define	_INPUTCONTROLLER_H

#include "gui/ui_MainWindow.h"
#include "gui/GuiMainWindow.h"

#include <fuzzylite/FuzzyLite.h>

namespace fl_gui {

    class InputController : public QObject {
        Q_OBJECT

    protected slots:
        virtual void onClickAddInput();
        virtual void onClickRemoveInput();
        virtual void onClickEditInput();
    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();
        
        virtual fl::FuzzyEngine& getModel() const;
        virtual GuiMainWindow& getView() const;
    public:
        InputController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~InputController();

        virtual void refreshModel();

    };
}

#endif	/* _INPUTCONTROLLER_H */

