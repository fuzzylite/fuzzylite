#include "TestController.h"
#include "GuiGrapher.h"

namespace fl_gui {

	TestController::TestController(fl::FuzzyEngine& model, GuiMainWindow& view) :
		_model(&model), _view(&view) {
		connect();
	}

	TestController::~TestController() {
		disconnect();
	}

	fl::FuzzyEngine& TestController::getModel() const {
		return *this->_model;
	}

	GuiMainWindow& TestController::getView() const {
		return *this->_view;
	}

	void TestController::connect() {
		QObject::connect(getView().getUi().tlx_main, SIGNAL(currentChanged(int)), this,
			SLOT(onPageChange(int)));

	}

	void TestController::disconnect() {
		QObject::disconnect(getView().getUi().tlx_main, SIGNAL(currentChanged(int)),
			this, SLOT(onPageChange(int)));
	}

	void TestController::onPageChange(int page) {
		if (page == 0) {
			return;
		}
		//Inputs
		QVBoxLayout* layout =
			dynamic_cast<QVBoxLayout*> (getView().getUi().grx_test_inputs->layout());
		for (int i = layout->count() - 1; i >= 0; --i) {
			QLayoutItem* item = layout->itemAt(i);
			GuiGrapher* g = dynamic_cast<GuiGrapher*> (item->widget());
			if (g) {
				layout->removeItem(item);
				QObject::disconnect(g, SIGNAL(onChangeInputValue()), this,
					SLOT(onInputValueChanged()));
				delete item->widget();
				delete item;
			}
		}
		for (int i = 0; i < getModel().numberOfInputLVars(); ++i) {
			GuiGrapher* g = new GuiGrapher(*getModel().inputLVar(i));
			g->setup();
			layout->insertWidget(i, g);
			QObject::connect(g, SIGNAL(onChangeInputValue()), this,
				SLOT(onInputValueChanged()));
		}

		//Rules
		getView().getUi().lsw_test_rules->clear();
		getView().getUi().lsw_test_rules_activation->clear();
		for (int i = 0; i < getModel().numberOfRules(); ++i) {
			QString rule = QString::fromStdString(getModel().rule(i)->toString());
			getView().getUi().lsw_test_rules->addItem(rule);
			QListWidgetItem* item = new QListWidgetItem;
			item->setText("-");
			item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			getView().getUi().lsw_test_rules_activation->addItem(item);
		}
		for (int i = 0; i < getModel().numberOfRules(); ++i) {
			QListWidgetItem* rule = getView().getUi().lsw_test_rules->item(i);
			QListWidgetItem* act = getView().getUi().lsw_test_rules_activation->item(i);
			QRect rect = getView().getUi().lsw_test_rules->visualItemRect(rule);
			act->setSizeHint(rect.size());
		}

		//Outputs
		layout
			= dynamic_cast<QVBoxLayout*> (getView().getUi().grx_test_outputs->layout());
		for (int i = layout->count() - 1; i >= 0 ; --i) {
			QLayoutItem* item = layout->itemAt(i);
			GuiGrapher* g = dynamic_cast<GuiGrapher*> (item->widget());
			if (g) {
				layout->removeItem(item);
				QObject::disconnect(this, SIGNAL(forceUpdate()), g, SLOT(updateUi()));
				delete item->widget();
				delete item;
			}
		}
		for (int i = 0; i < getModel().numberOfOutputLVars(); ++i) {
			GuiGrapher* g = new GuiGrapher(*getModel().outputLVar(i));
			g->setup();
			layout->insertWidget(i, g);
			QObject::connect(this, SIGNAL(forceUpdate()), g, SLOT(updateUi()));
		}

	}

	void TestController::onInputValueChanged() {
		//        FL_LOG("-----------------------");
		QColor from_color(Qt::white);
		QColor to_color(0, 200, 0);
		for (int i = 0; i < getModel().numberOfOutputLVars(); ++i) {
			getModel().outputLVar(i)->output().clear();
		}
		for (int i = 0; i < getModel().numberOfRules(); ++i) {
			fl::FuzzyRule& rule = *getModel().rule(i);
			fl::flScalar degree = rule.evaluate();

			int red, green, blue, alpha;
			fl::FuzzyOperator::ColorGradient(from_color.red(), from_color.green(),
				from_color.blue(), from_color.alpha(), to_color.red(), to_color.green(),
				to_color.blue(), to_color.alpha(), degree, red, green, blue, alpha);

			QColor color = QColor(red, green, blue, alpha);

			getView().getUi().lsw_test_rules->item(i)->setBackground(QBrush(color));
			getView().getUi().lsw_test_rules_activation->item(i)->setBackground(QBrush(
				color));
			getView().getUi().lsw_test_rules_activation->item(i)->setText(
				QString::number(degree, 'f', 3));
			rule.fire(degree);
		}
		emit forceUpdate();
	}
}
