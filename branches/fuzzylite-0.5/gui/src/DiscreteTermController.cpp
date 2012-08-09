#include "DiscreteTermController.h"

#include <StringOperator.h>

namespace fuzzy_lite_gui {

    DiscreteTermController::DiscreteTermController(DiscreteTerm& model,
            GuiDiscreteTerm& view)
    : _model(&model), _view(&view) {
        connect();
    }

    DiscreteTermController::~DiscreteTermController() {
        disconnect();
    }

    void DiscreteTermController::connect() {
        getView().getUi().led_name->setText(QString::fromStdString(getModel().getName()));
        getView().getUi().ckx_from_infinity->setChecked(getModel().isFromInfinity());
        getView().getUi().ckx_to_infinity->setChecked(getModel().isToInfinity());
        for (int i = 0; i < getModel().numberOfCoords(); ++i) {
            getView().getUi().ptx_coords->appendPlainText(
                    QString::number(getModel().getCoordX(i)) + ":" +
                    QString::number(getModel().getCoordY(i)));
        }
    }

    void DiscreteTermController::disconnect() {
    }


    DiscreteTerm& DiscreteTermController::getModel() const {
        return *this->_model;
    }

    GuiDiscreteTerm& DiscreteTermController::getView() const {
        return *this->_view;
    }

    void DiscreteTermController::saveModel() {
        getModel().setName(getView().getUi().led_name->text().toStdString());
        getModel().setFromInfinity(getView().getUi().ckx_from_infinity->isChecked());
        getModel().setToInfinity(getView().getUi().ckx_to_infinity->isChecked());
        getModel().clear();
        QStringList coords = getView().getUi().ptx_coords->toPlainText().split("\n", QString::SkipEmptyParts);
        flScalar x, y;
        for (int i = 0; i < coords.size(); ++i) {
            QStringList xy = coords.at(i).split(":", QString::SkipEmptyParts);
            x = (flScalar) xy.at(0).toDouble();
            y = (flScalar) xy.at(1).toDouble();
            getModel().addCoord(x, y);
        }
    }


}
