/* 
 * File:   RuleController.h
 * Author: jcrada
 *
 * Created on November 17, 2009, 1:22 AM
 */

#ifndef _RULECONTROLLER_H
#define	_RULECONTROLLER_H

#include "GuiMainWindow.h"

#include <QtCore/QObject>
#include <FuzzyEngine.h>

using namespace fuzzy_lite;
namespace fuzzy_lite_gui{
    class RuleController : public QObject{
        Q_OBJECT
    protected slots:
        virtual void onClickGenerateRules();
        virtual void onClickCheckSpell();
    private:
        FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual void removeAllRules();
        virtual FuzzyEngine& getModel() const;
        virtual GuiMainWindow& getView() const;

    public:
        RuleController(FuzzyEngine& model, GuiMainWindow& view);
        virtual ~RuleController();

    };
}

#endif	/* _RULECONTROLLER_H */

