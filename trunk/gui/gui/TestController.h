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
 * File:   TestController.h
 * Author: jcrada
 *
 * Created on December 6, 2009, 10:07 AM
 */

#ifndef _TESTCONTROLLER_H
#define	_TESTCONTROLLER_H

#include <QtCore/QObject>
#include <fuzzylite/FuzzyLite.h>

#include "gui/GuiMainWindow.h"

namespace fl_gui{
    class TestController : public QObject{
        Q_OBJECT
        signals:
        void forceUpdate();
    public slots:
        virtual void load();
        virtual void unload();
        virtual void onInputValueChanged();

    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual fl::FuzzyEngine& getModel() const;
        virtual GuiMainWindow& getView() const;
    public:
        TestController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~TestController();

        virtual void refreshModel();

    };
}

#endif	/* _TESTCONTROLLER_H */

