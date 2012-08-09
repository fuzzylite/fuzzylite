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

