#include "OutputController.h"
#include "ui_LinguisticVariable.h"
#include "GuiLinguisticVariable.h"
#include "LinguisticVariableController.h"

#include <QtGui/QListWidgetItem>
namespace fl_gui {

	OutputController::OutputController(fl::FuzzyEngine& model, GuiMainWindow& view) :
		_model(&model), _view(&view) {
		connect();
	}

	OutputController::~OutputController() {
		disconnect();
	}

	void OutputController::connect() {
		QObject::connect(view().getUi().btn_add_output, SIGNAL(clicked()), this,
			SLOT(onClickAddOutput()));
		QObject::connect(view().getUi().btn_remove_output, SIGNAL(clicked()), this,
			SLOT(onClickRemoveOutput()));
		QObject::connect(view().getUi().btn_edit_output, SIGNAL(clicked()), this,
			SLOT(onClickEditOutput()));
		refreshModel();
	}

	void OutputController::disconnect() {
		QObject::disconnect(view().getUi().btn_add_output, SIGNAL(clicked()), this,
			SLOT(onClickAddOutput()));
		QObject::disconnect(view().getUi().btn_remove_output, SIGNAL(clicked()), this,
			SLOT(onClickRemoveOutput()));
		QObject::disconnect(view().getUi().btn_edit_output, SIGNAL(clicked()), this,
			SLOT(onClickEditOutput()));
	}

	void OutputController::refreshModel() {
		view().getUi().lvw_outputs->clear();
		for (int i = 0; i < model().numberOfOutputLVars(); ++i) {
			QListWidgetItem* item = new QListWidgetItem();
			item->setText(QString::fromStdString(model().outputLVar(i)->name()));
			item->setToolTip(QString::fromStdString(model().outputLVar(i)->toString()));
			view().getUi().lvw_outputs->addItem(item);
		}
	}
	fl::FuzzyEngine& OutputController::model() const {
		return *this->_model;
	}

	GuiMainWindow& OutputController::view() const {
		return *this->_view;
	}

	void OutputController::onClickAddOutput() {
		fl::OutputLVar* lvar = new fl::OutputLVar();
		GuiLinguisticVariable lvar_view;
		lvar_view.setup();
		LinguisticVariableController c(*lvar, lvar_view);
		if (lvar_view.exec() == QDialog::Accepted) {
			c.saveModel();
			model().addOutputLVar(*lvar);
			QListWidgetItem* item = new QListWidgetItem();
			item->setText(QString::fromStdString(lvar->name()));
			item->setToolTip(QString::fromStdString(lvar->toString()));
			view().getUi().lvw_outputs->addItem(item);
		} else {
			delete lvar;
		}
	}

	void OutputController::onClickRemoveOutput() {
		QListWidgetItem* selected = view().getUi().lvw_outputs->selectedItems().at(0);
		std::string item = selected->text().toStdString();
		fl::OutputLVar* output = model().removeOutputLVar(item);
		std::vector<fl::FuzzyRule*> rules = model().removeAllRules();
		for (size_t i = 0; i < rules.size(); ++i) {
			delete rules[i];
		}
		delete output;

		view().getUi().lvw_outputs->removeItemWidget(selected);
		delete selected;
	}

	void OutputController::onClickEditOutput() {
		QListWidgetItem* selected = view().getUi().lvw_outputs->selectedItems().at(0);
		std::string item = selected->text().toStdString();
		fl::OutputLVar* lvar = model().outputLVar(item);
		GuiLinguisticVariable lvar_view;
		lvar_view.setup();
		LinguisticVariableController c(*lvar, lvar_view);
		if (lvar_view.exec() == QDialog::Accepted) {
			c.saveModel();
			selected->setText(QString::fromStdString(lvar->name()));
			selected->setToolTip(QString::fromStdString(lvar->toString()));
		}
	}

}
