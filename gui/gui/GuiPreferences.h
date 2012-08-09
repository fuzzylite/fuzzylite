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
 * File:   GuiPreferences.h
 * Author: jcrada
 *
 * Created on December 7, 2009, 1:14 AM
 */

#ifndef _GUIPREFERENCES_H
#define	_GUIPREFERENCES_H

#include "gui/ui_Preferences.h"

#include <QtGui/QDialog>
#include <fuzzylite/FuzzyLite.h>

namespace fl_gui {

    class GuiPreferences : public QDialog {
        Q_OBJECT
    protected slots:
        virtual void onChangeTNorm(int index);
        virtual void onChangeSNorm(int index);
        virtual void onChangeModulation(int index);
        virtual void onChangeAggregation(int index);
        virtual void onChangeAlgorithm(int index);
        virtual void onChangeDefuzzifier(int index);
        virtual void onChangeSampleSize(int index);

    private:
        fl::FuzzyOperator* _model;
        Ui::Preferences* _ui;
    protected:
        virtual void connect();
        virtual void disconnect();
        virtual void load();
    public:
        GuiPreferences(fl::FuzzyOperator& model, QWidget* parent = NULL,
                Qt::WindowFlags f = 0);
        virtual ~GuiPreferences();

        virtual void setup();
        
        virtual Ui::Preferences& getUi() const;
        virtual fl::FuzzyOperator& getModel() const;


    };
}

#endif	/* _GUIPREFERENCES_H */

