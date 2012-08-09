/* 
 * File:   OutputController.h
 * Author: jcrada
 *
 * Created on November 16, 2009, 9:16 PM
 */

#ifndef _OUTPUTCONTROLLER_H
#define	_OUTPUTCONTROLLER_H

#include "ui_MainWindow.h"
#include "GuiMainWindow.h"

#include <fuzzylite/FuzzyLite.h>
#include <QtCore/QObject>


namespace fl_gui {

    class OutputController : public QObject {
        Q_OBJECT

    protected slots:
        virtual void onClickAddOutput();
        virtual void onClickRemoveOutput();
        virtual void onClickEditOutput();
    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual fl::FuzzyEngine& model() const;
        virtual GuiMainWindow& view() const;
    public:
        OutputController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~OutputController();

        virtual void refreshModel();

    };
}

#endif	/* _OUTPUTCONTROLLER_H */

