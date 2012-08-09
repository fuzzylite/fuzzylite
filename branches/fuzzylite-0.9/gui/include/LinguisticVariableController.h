/* 
 * File:   LinguisticVariableController.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 9:53 PM
 */

#ifndef _LINGUISTICVARIABLECONTROLLER_H
#define	_LINGUISTICVARIABLECONTROLLER_H

#include <QtCore/QObject>

#include "ui_LinguisticVariable.h"
#include "GuiLinguisticVariable.h"
#include "GuiGrapher.h"

#include <fuzzylite/FuzzyLite.h>

namespace fl_gui{
    class LinguisticVariableController : public QObject{
        Q_OBJECT
    protected slots:
        virtual void onClickAddTerm();
        virtual void onClickEditTerm();
        virtual void onClickRemoveTerm();
        virtual void onClickWizard();

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

