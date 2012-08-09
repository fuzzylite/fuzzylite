/* 
 * File:   TestController.h
 * Author: jcrada
 *
 * Created on December 6, 2009, 10:07 AM
 */

#ifndef _TESTCONTROLLER_H
#define	_TESTCONTROLLER_H

#include <QtCore/QObject>
#include <fuzzylite/FuzzyLite.h>

#include "GuiMainWindow.h"

namespace fl_gui{
    class TestController : public QObject{
        Q_OBJECT
        signals:
        void forceUpdate();
    protected slots:
        virtual void onPageChange(int page);
        virtual void onInputValueChanged();

    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual fl::FuzzyEngine& getModel() const;
        virtual GuiMainWindow& getView() const;
    public:
        TestController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~TestController();

    };
}

#endif	/* _TESTCONTROLLER_H */

