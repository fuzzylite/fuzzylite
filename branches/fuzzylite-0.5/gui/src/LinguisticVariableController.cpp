#include "LinguisticVariableController.h"
#include "DiscreteTermController.h"
#include "GuiLVarWizard.h"

namespace fuzzy_lite_gui {

    LinguisticVariableController::LinguisticVariableController(LinguisticVariable& model,
            GuiLinguisticVariable& view)
    : _model(&model), _view(&view), _grapher(model) {
        connect();
    }

    LinguisticVariableController::~LinguisticVariableController() {
        disconnect();
    }

    void LinguisticVariableController::connect() {
        QObject::connect(getView().getUi().btn_add_term, SIGNAL(clicked()),
                this, SLOT(onClickAddTerm()));
        QObject::connect(getView().getUi().btn_edit_term, SIGNAL(clicked()),
                this, SLOT(onClickEditTerm()));
        QObject::connect(getView().getUi().btn_remove_term, SIGNAL(clicked()),
                this, SLOT(onClickRemoveTerm()));
        QObject::connect(getView().getUi().btn_wizard, SIGNAL(clicked()),
                this, SLOT(onClickWizard()));

        getView().getUi().led_name->setText(QString::fromStdString(getModel().getName()));
        for (int i = 0; i < getModel().numberOfTerms(); ++i) {
            getView().getUi().lvw_terms->addItem(QString::fromStdString(getModel().getTerm(i).getName()));
        }
        _grapher.setup();
        _grapher.getUi().grx_control->setVisible(false);
        getView().getUi().grx_lvar->layout()->addWidget(&_grapher);
        getView().getUi().grx_lvar->layout()->update();

    }

    void LinguisticVariableController::disconnect() {
        QObject::disconnect(getView().getUi().btn_add_term, SIGNAL(clicked()),
                this, SLOT(onClickAddTerm()));
        QObject::disconnect(getView().getUi().btn_edit_term, SIGNAL(clicked()),
                this, SLOT(onClickEditTerm()));
        QObject::disconnect(getView().getUi().btn_remove_term, SIGNAL(clicked()),
                this, SLOT(onClickRemoveTerm()));
        QObject::disconnect(getView().getUi().btn_wizard, SIGNAL(clicked()),
                this, SLOT(onClickWizard()));
    }

    LinguisticVariable& LinguisticVariableController::getModel() const {
        return *this->_model;
    }

    GuiLinguisticVariable& LinguisticVariableController::getView() const {
        return *this->_view;
    }

    void LinguisticVariableController::onClickAddTerm() {
        DiscreteTerm* model = new DiscreteTerm();
        GuiDiscreteTerm view(&getView());
        view.setup();
        DiscreteTermController c(*model, view);
        if (view.exec() == QDialog::Accepted) {
            c.saveModel();
            getModel().addTerm(*model);
            QListWidgetItem* item = new QListWidgetItem;
            item->setText(QString::fromStdString(model->getName()));
            item->setToolTip(QString::fromStdString(model->toString()));
            getView().getUi().lvw_terms->addItem(item);
        } else {
            delete model;
        }
        _grapher.updateUi();
    }

    void LinguisticVariableController::onClickEditTerm() {
        QListWidgetItem* selected = getView().getUi().lvw_terms->selectedItems().at(0);
        std::string name = selected->text().toStdString();
        DiscreteTerm* model = getModel().getTerm(name);
        GuiDiscreteTerm view(&getView());
        view.setup();
        DiscreteTermController c(*model, view);
        if (view.exec() == QDialog::Accepted) {
            c.saveModel();
        }
        selected->setText(QString::fromStdString(model->getName()));
        selected->setToolTip(QString::fromStdString(model->toString()));
        _grapher.updateUi();
    }

    void LinguisticVariableController::onClickRemoveTerm() {
        QListWidgetItem* selected = getView().getUi().lvw_terms->selectedItems().at(0);
        std::string name = selected->text().toStdString();
        delete getModel().removeTerm(name);
        getView().getUi().lvw_terms->removeItemWidget(selected);
        delete selected;
        _grapher.updateUi();
    }

    void LinguisticVariableController::saveModel() {
        //TODO: Implement cancellation of changes of DiscreteTerms...
        getModel().setName(getView().getUi().led_name->text().toStdString());
    }

    void LinguisticVariableController::onClickWizard() {
        GuiLVarWizard wizard(getModel(), &getView());
        wizard.setup();
        if (wizard.exec() == QDialog::Accepted) {
            getView().getUi().lvw_terms->clear();
            for (int i = 0; i < getModel().numberOfTerms(); ++i) {
                QListWidgetItem* item = new QListWidgetItem();
                item->setText(QString::fromStdString(getModel().getTerm(i).getName()));
                item->setToolTip(QString::fromStdString(getModel().getTerm(i).toString()));
                getView().getUi().lvw_terms->addItem(item);
            }
        }
        _grapher.updateUi();
    }

   
}
