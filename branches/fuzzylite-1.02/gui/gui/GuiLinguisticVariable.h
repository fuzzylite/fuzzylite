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
 * File:   GuiLinguisticVariable.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 6:24 PM
 */

#ifndef _GUILINGUISTICVARIABLE_H
#define	_GUILINGUISTICVARIABLE_H

#include <QtGui/QDialog>

#include "gui/ui_LinguisticVariable.h"

namespace fl_gui {

    class GuiLinguisticVariable : public QDialog {
        Q_OBJECT
    signals:
        void onUpdateUi();
    public slots:
        virtual void update(){
            QDialog::update();
            emit(onUpdateUi());
        }
    protected slots:
        virtual void accept();
        virtual void onChangeTermSelection();
        virtual void onDoubleClickDiscreteItem(QListWidgetItem*);
    private:
        Ui::LinguisticVariable* _ui;
    protected:
        virtual void connect();
        virtual void disconnect();

    public:
        GuiLinguisticVariable(QWidget* parent = NULL, Qt::WindowFlags f = 0);
        virtual ~GuiLinguisticVariable();

        virtual Ui::LinguisticVariable& getUi() const;

        virtual void setup();


    };
}

#endif	/* _GUILINGUISTICVARIABLE_H */

