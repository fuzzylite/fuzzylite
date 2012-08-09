/* 
 * File:   InputController.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 9:38 PM
 */

#ifndef _INPUTCONTROLLER_H
#define	_INPUTCONTROLLER_H

#include "ui_MainWindow.h"
#include "GuiMainWindow.h"

#include <fuzzylite/FuzzyLite.h>

namespace fl_gui {

    class InputController : public QObject {
        Q_OBJECT

    protected slots:
        virtual void onClickAddInput();
        virtual void onClickRemoveInput();
        virtual void onClickEditInput();
    private:
        fl::FuzzyEngine* _model;
        GuiMainWindow* _view;
    protected:
        virtual void connect();
        virtual void disconnect();
        
        virtual fl::FuzzyEngine& getModel() const;
        virtual GuiMainWindow& getView() const;
    public:
        InputController(fl::FuzzyEngine& model, GuiMainWindow& view);
        virtual ~InputController();

        virtual void refreshModel();

    };
}

#endif	/* _INPUTCONTROLLER_H */

