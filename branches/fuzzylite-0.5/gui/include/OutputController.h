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

#include <FuzzyEngine.h>
#include <QtCore/QObject>

using namespace fuzzy_lite;
namespace fuzzy_lite_gui {

    class OutputController : public QObject {
        Q_OBJECT

    protected slots:
        virtual void onClickAddOutput();
        virtual void onClickRemoveOutput();
        virtual void onClickEditOutput();
    private:
        FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual FuzzyEngine& getModel() const;
        virtual GuiMainWindow& getView() const;
    public:
        OutputController(FuzzyEngine& model, GuiMainWindow& view);
        virtual ~OutputController();

    };
}

#endif	/* _OUTPUTCONTROLLER_H */

