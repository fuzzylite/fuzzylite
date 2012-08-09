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
 * File:   Grapher.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 1:05 PM
 */

#ifndef _GUIGRAPHER_H
#define	_GUIGRAPHER_H

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>

#include <fuzzylite/FuzzyLite.h>

#include "gui/ui_Grapher.h"

namespace fl_gui {

    class GuiGrapher : public QWidget {
        Q_OBJECT
    signals:
        void onChangeInputValue();
    public slots:
        virtual void updateUi();
    protected slots:
        virtual void onChangeSliderValue(int position);
        virtual void onEditInputValue();
        virtual void mouseDoubleClickEvent(QMouseEvent* e);
    private:
        fl::LinguisticVariable* _lvar;
        Ui::Grapher* _ui;
        bool _read_only;
        bool _lvar_enabled;
        bool _term_enabled;
        bool _is_input;
        bool _is_output;

    protected:
        virtual fl::LinguisticVariable& getLVar() const;

        virtual void connect();
        virtual void disconnect();
        virtual void contextMenuEvent(QContextMenuEvent* e);
        virtual void resizeEvent(QResizeEvent* e);
        virtual void focusInEvent(QFocusEvent* e);

    public:
        GuiGrapher(fl::LinguisticVariable& lvar, QWidget* parent = NULL, Qt::WindowFlags f = 0,
                bool enable_lvar = true, bool enable_term = true);
        virtual ~GuiGrapher();

        virtual Ui::Grapher& getUi() const;

        virtual void setup();

        virtual bool isInputLVar() const;
        virtual bool isOutputLVar() const;

        virtual bool isTermEnabled() const;
        virtual void setTermEnabled(bool enable);

        virtual bool isLVarEnabled() const;
        virtual void setLVarEnabled(bool enable);


    };
}

#endif	/* _GUIGRAPHER_H */

