/* 
 * File:   GuiLVarWizard.h
 * Author: jcrada
 *
 * Created on November 16, 2009, 9:57 PM
 */

#ifndef _GUILVARWIZARD_H
#define	_GUILVARWIZARD_H

#include <QtGui/QDialog>

#include <fuzzylite/FuzzyLite.h>
#include "ui_LinguisticVariableWizard.h"


namespace fl_gui{
    class GuiLVarWizard : public QDialog{
        Q_OBJECT
    protected slots:
        virtual void onChangeMinimum(double min);
        virtual void onChangeMaximum(double max);
        virtual void onClickCreateLabels();
        virtual void accept();
    private:
        fl::LinguisticVariable* _model;
        Ui::LinguisticVariableWizard* _ui;
    protected:
        virtual fl::LinguisticVariable& getModel() const;
        virtual Ui::LinguisticVariableWizard& getUi() const;

        virtual void connect();
        virtual void disconnect();
    public:
        GuiLVarWizard(fl::LinguisticVariable& model,
                QWidget* parent = NULL, Qt::WindowFlags f = 0);
        virtual ~GuiLVarWizard();

        virtual void setup();


    };
}

#endif	/* _GUILVARWIZARD_H */

