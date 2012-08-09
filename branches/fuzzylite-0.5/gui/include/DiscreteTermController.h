/* 
 * File:   DiscreteTermController.h
 * Author: jcrada
 *
 * Created on November 14, 2009, 10:10 PM
 */

#ifndef _DISCRETETERMCONTROLLER_H
#define	_DISCRETETERMCONTROLLER_H

#include "ui_DiscreteTerm.h"
#include "GuiDiscreteTerm.h"

#include <QtCore/QObject>

#include <DiscreteTerm.h>
using namespace fuzzy_lite;
namespace fuzzy_lite_gui{
    class DiscreteTermController : QObject{
        Q_OBJECT
    private:
        DiscreteTerm* _model;
        GuiDiscreteTerm* _view;
    protected:
        virtual void connect();
        virtual void disconnect();

        virtual DiscreteTerm& getModel() const;
        virtual GuiDiscreteTerm& getView() const;
    public:
        DiscreteTermController(DiscreteTerm& model, GuiDiscreteTerm& view);
        virtual ~DiscreteTermController();

        virtual void saveModel();


    };
}

#endif	/* _DISCRETETERMCONTROLLER_H */

