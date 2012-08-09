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
 * File:   OutputController.h
 * Author: jcrada
 *
 * Created on November 16, 2009, 9:16 PM
 */

#ifndef _OUTPUTCONTROLLER_H
#define	_OUTPUTCONTROLLER_H

#include "gui/ui_MainWindow.h"
#include "gui/GuiMainWindow.h"

#include <fuzzylite/FuzzyLite.h>
#include <QtCore/QObject>


namespace fl_gui {

    class OutputController : public QObject {
        Q_OBJECT

    protected slots:
        virtual void onClickAddOutput();
        virtual void onClickRemoveOutput();
        virtual void onClickEditOutput();
    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual fl::FuzzyEngine& model() const;
        virtual GuiMainWindow& view() const;
    public:
        OutputController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~OutputController();

        virtual void refreshModel();

    };
}

#endif	/* _OUTPUTCONTROLLER_H */

