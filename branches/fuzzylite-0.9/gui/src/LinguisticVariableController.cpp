#include "LinguisticVariableController.h"
#include "GuiLVarWizard.h"

#include "GuiLinguisticTerm.h"
namespace fl_gui {

	LinguisticVariableController::LinguisticVariableController(
		fl::LinguisticVariable& model, GuiLinguisticVariable& view) :
		_model(&model), _view(&view) {
		connect();
	}

	LinguisticVariableController::~LinguisticVariableController() {
		disconnect();
	}

	void LinguisticVariableController::connect() {
		QObject::connect(view().getUi().btn_add_term, SIGNAL(clicked()), this,
			SLOT(onClickAddTerm()));
		QObject::connect(view().getUi().btn_edit_term, SIGNAL(clicked()), this,
			SLOT(onClickEditTerm()));
		QObject::connect(view().getUi().btn_remove_term, SIGNAL(clicked()), this,
			SLOT(onClickRemoveTerm()));
		QObject::connect(view().getUi().btn_wizard, SIGNAL(clicked()), this,
			SLOT(onClickWizard()));
		QObject::connect(view().getUi().lvw_terms, SIGNAL(itemSelectionChanged()), this,
			SLOT(onChangeTerm()));
		QObject::connect(view().getUi().lvw_terms,
			SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
			SLOT(onDoubleClickTerm(QListWidgetItem*)));
		refreshModel();
	}

	void LinguisticVariableController::disconnect() {
		QObject::disconnect(view().getUi().btn_add_term, SIGNAL(clicked()), this,
			SLOT(onClickAddTerm()));
		QObject::disconnect(view().getUi().btn_edit_term, SIGNAL(clicked()), this,
			SLOT(onClickEditTerm()));
		QObject::disconnect(view().getUi().btn_remove_term, SIGNAL(clicked()), this,
			SLOT(onClickRemoveTerm()));
		QObject::disconnect(view().getUi().btn_wizard, SIGNAL(clicked()), this,
			SLOT(onClickWizard()));
		QObject::disconnect(view().getUi().lvw_terms, SIGNAL(itemSelectionChanged()),
			this, SLOT(onChangeTerm()));
		QObject::disconnect(view().getUi().lvw_terms,
			SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
			SLOT(onDoubleClickTerm(QListWidgetItem*)));
	}

	void LinguisticVariableController::refreshModel() {
		view().getUi().led_name->setText(QString::fromStdString(model().name()));
		view().getUi().lvw_terms->clear();
		for (int i = 0; i < model().numberOfTerms(); ++i) {
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(QString::fromStdString(model().term(i)->name()));
			item->setToolTip(QString::fromStdString(model().term(i)->toString()));
			view().getUi().lvw_terms->addItem(item);
		}
		draw();
	}

	fl::LinguisticVariable& LinguisticVariableController::model() const {
		return *this->_model;
	}

	GuiLinguisticVariable& LinguisticVariableController::view() const {
		return *this->_view;
	}

	void LinguisticVariableController::draw() {
		view().getUi().gvw_canvas->clear();

		view().getUi().gvw_canvas->drawVariable(model());
		view().getUi().led_lvar_min->setText(QString::number(model().minimum(), 'f', 3));
		view().getUi().led_lvar_max->setText(QString::number(model().maximum(), 'f', 3));
	}

	void LinguisticVariableController::onClickAddTerm() {
		GuiLinguisticTerm lterm_view;
		lterm_view.setup();
		lterm_view.getUi().led_name->setText(QString::fromStdString(model().name()) + "."
			+ QString::number(model().numberOfTerms() + 1));
		if (lterm_view.exec() == QDialog::Accepted) {
			fl::LinguisticTerm* term = lterm_view.selectedTerm();
			model().addTerm(*term);
		}
		refreshModel();
	}

	void LinguisticVariableController::onClickEditTerm() {

		QListWidgetItem* selected = view().getUi().lvw_terms->selectedItems().at(0);
		std::string name = selected->text().toStdString();
		fl::LinguisticTerm* lterm = model().term(name);
		GuiLinguisticTerm lterm_view;
		lterm_view.setup();
		lterm_view.showTerm(*lterm);
		if (lterm_view.exec() == QDialog::Accepted) {
			model().deleteTerm(name);
			lterm = lterm_view.selectedTerm();
			model().addTerm(*lterm);
		}
		refreshModel();
	}

	void LinguisticVariableController::onClickRemoveTerm() {
		for (int i = 0; i < view().getUi().lvw_terms->count(); ++i) {
			QListWidgetItem* item = view().getUi().lvw_terms->item(i);
			if (item->isSelected()) {
				model().deleteTerm(i);
				break;
			}
		}
		refreshModel();
	}

	void LinguisticVariableController::saveModel() {
		//TODO: Implement cancellation of changes of DiscreteTerms...
		model().setName(view().getUi().led_name->text().toStdString());
	}

	void LinguisticVariableController::onClickWizard() {
		GuiLVarWizard wizard(model(), &view());
		wizard.setup();
		if (wizard.exec() == QDialog::Accepted) {

		}
		refreshModel();
	}

	void LinguisticVariableController::onChangeTerm() {
		draw();
		for (int i = 0; i < view().getUi().lvw_terms->count(); ++i) {
			if (view().getUi().lvw_terms->item(i)->isSelected()) {
				QColor color = QColor(Qt::green).darker(125);
				color.setAlpha(230);
				view().getUi().gvw_canvas->drawTerm(*model().term(i), color);
				break;
			}
		}
	}

	void LinguisticVariableController::onDoubleClickTerm(QListWidgetItem* item) {
		item->setSelected(false);
	}

}
