#include "OutputController.h"
#include "ui_LinguisticVariable.h"
#include "GuiLinguisticVariable.h"
#include "LinguisticVariableController.h"

#include <OutputLVar.h>
#include <QtGui/QListWidgetItem>
namespace fuzzy_lite_gui {

    OutputController::OutputController(FuzzyEngine& model, GuiMainWindow& view)
    : _model(&model), _view(&view) {
        connect();
    }

    OutputController::~OutputController() {
        disconnect();
    }

    void OutputController::connect() {
        QObject::connect(getView().getUi().btn_add_output, SIGNAL(clicked()), this, SLOT(onClickAddOutput()));
        QObject::connect(getView().getUi().btn_remove_output, SIGNAL(clicked()), this, SLOT(onClickRemoveOutput()));
        QObject::connect(getView().getUi().btn_edit_output, SIGNAL(clicked()), this, SLOT(onClickEditOutput()));
        for (int i = 0 ; i < getModel().numberOfOutputLVars() ; ++i){
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(getModel().getOutputLVar(i).getName()));
            item->setToolTip(QString::fromStdString(getModel().getOutputLVar(i).toString()));
            getView().getUi().lvw_outputs->addItem(item);
        }
    }

    void OutputController::disconnect() {
        QObject::disconnect(getView().getUi().btn_add_output, SIGNAL(clicked()), this, SLOT(onClickAddOutput()));
        QObject::disconnect(getView().getUi().btn_remove_output, SIGNAL(clicked()), this, SLOT(onClickRemoveOutput()));
        QObject::disconnect(getView().getUi().btn_edit_output, SIGNAL(clicked()), this, SLOT(onClickEditOutput()));
    }

    FuzzyEngine& OutputController::getModel() const {
        return *this->_model;
    }

    GuiMainWindow& OutputController::getView() const {
        return *this->_view;
    }

    void OutputController::onClickAddOutput() {
        OutputLVar* model = new OutputLVar();
        GuiLinguisticVariable view;
        view.setup();
        LinguisticVariableController c(*model, view);
        if (view.exec() == QDialog::Accepted) {
            c.saveModel();
            getModel().addOutputLVar(*model);
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(QString::fromStdString(model->getName()));
            item->setToolTip(QString::fromStdString(model->toString()));
            getView().getUi().lvw_outputs->addItem(item);
        } else {
            delete model;
        }
    }

    void OutputController::onClickRemoveOutput() {
        QListWidgetItem* selected = getView().getUi().lvw_outputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        OutputLVar* output = getModel().removeOutputLVar(item);
        std::vector<FuzzyRule*> rules = getModel().removeAllRules();
        for (int i = 0; i < rules.size(); ++i) {
            delete rules[i];
        }
        delete output;

        getView().getUi().lvw_outputs->removeItemWidget(selected);
        delete selected;
    }

    void OutputController::onClickEditOutput() {
        QListWidgetItem* selected = getView().getUi().lvw_outputs->selectedItems().at(0);
        std::string item = selected->text().toStdString();
        OutputLVar* output = getModel().getOutputLVar(item);
        GuiLinguisticVariable view;
        view.setup();
        LinguisticVariableController c(*output,view);
        if (view.exec() == QDialog::Accepted){
            c.saveModel();
            selected->setText(QString::fromStdString(output->getName()));
            selected->setToolTip(QString::fromStdString(output->toString()));
        }
    }


}
