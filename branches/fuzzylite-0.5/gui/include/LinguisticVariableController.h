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

#include <LinguisticVariable.h>

using namespace fuzzy_lite;
namespace fuzzy_lite_gui{
    class LinguisticVariableController : public QObject{
        Q_OBJECT
    protected slots:
        virtual void onClickAddTerm();
        virtual void onClickEditTerm();
        virtual void onClickRemoveTerm();
        virtual void onClickWizard();
    private:
        LinguisticVariable* _model;
        GuiLinguisticVariable* _view;
        GuiGrapher _grapher;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual LinguisticVariable& getModel() const;
        virtual GuiLinguisticVariable& getView() const;
    public:
        LinguisticVariableController(LinguisticVariable& model, GuiLinguisticVariable& view);
        virtual ~LinguisticVariableController();

        virtual void saveModel();
    };
}

#endif	/* _LINGUISTICVARIABLECONTROLLER_H */

