/* 
 * File:   GuiPreferences.h
 * Author: jcrada
 *
 * Created on December 7, 2009, 1:14 AM
 */

#ifndef _GUIPREFERENCES_H
#define	_GUIPREFERENCES_H

#include "ui_Preferences.h"

#include <QtGui/QDialog>
#include <FuzzyLite.h>

using namespace fuzzy_lite;
namespace fuzzy_lite_gui {

    class GuiPreferences : public QDialog {
        Q_OBJECT
    protected slots:
        virtual void onChangeTNorm(int index);
        virtual void onChangeSNorm(int index);
        virtual void onChangeModulation(int index);
        virtual void onChangeDefuzzification(int index);
        virtual void onChangeAggregation(int index);

    private:
        FuzzyOperator* _model;
        Ui::Preferences* _ui;
    protected:
        virtual void connect();
        virtual void disconnect();
    public:
        GuiPreferences(FuzzyOperator& model, QWidget* parent = NULL,
                Qt::WindowFlags f = 0);
        virtual ~GuiPreferences();

        virtual void setup();
        
        virtual Ui::Preferences& getUi() const;
        virtual FuzzyOperator& getModel() const;


    };
}

#endif	/* _GUIPREFERENCES_H */

