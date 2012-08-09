/* 
 * File:   GuiDiscreteTerm.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 10:11 PM
 */

#ifndef _GUIDISCRETETERM_H
#define	_GUIDISCRETETERM_H

#include <QtGui/QDialog>

#include "ui_DiscreteTerm.h"

namespace fuzzy_lite_gui {

    class GuiDiscreteTerm : public QDialog {
        Q_OBJECT
    protected slots:
        virtual void accept();
        virtual void onClickAddTriangle();
        virtual void onClickAddTrapezoid();
        virtual void onChangeTriangleA(double a);
        virtual void onChangeTriangleB(double b);
        virtual void onChangeTrapezoidA(double a);
        virtual void onChangeTrapezoidB(double b);
        virtual void onChangeTrapezoidC(double c);
    private:
        Ui::DiscreteTerm* _ui;
    protected:
        virtual void connect();
        virtual void disconnect();
        
        virtual std::string isValid(bool& ok);



    public:
        GuiDiscreteTerm(QWidget* parent = NULL, Qt::WindowFlags flags = 0);
        virtual ~GuiDiscreteTerm();

        virtual Ui::DiscreteTerm& getUi() const;

        virtual void setup();

    };
}

#endif	/* _GUIDISCRETETERM_H */

