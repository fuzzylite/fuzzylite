/* 
 * File:   GuiLinguisticVariable.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 6:24 PM
 */

#ifndef _GUILINGUISTICVARIABLE_H
#define	_GUILINGUISTICVARIABLE_H

#include <QtGui/QDialog>

#include "ui_LinguisticVariable.h"

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

