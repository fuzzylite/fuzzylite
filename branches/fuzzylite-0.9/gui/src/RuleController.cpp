#include "RuleController.h"

#include <QtGui/QTextBlock>
namespace fl_gui {

	RuleController::RuleController(fl::FuzzyEngine& model, GuiMainWindow& view) :
		_model(&model), _view(&view) {
		connect();
	}

	RuleController::~RuleController() {
		disconnect();
	}

	void RuleController::connect() {
		QObject::connect(view().getUi().btn_check_rules, SIGNAL(clicked()), this,
			SLOT(onClickCheckSpell()));
		QObject::connect(view().getUi().btn_generate_rules, SIGNAL(clicked()), this,
			SLOT(onClickGenerateRules()));
		refreshModel();
	}

	void RuleController::disconnect() {
		QObject::disconnect(view().getUi().btn_check_rules, SIGNAL(clicked()), this,
			SLOT(onClickCheckSpell()));
		QObject::disconnect(view().getUi().btn_generate_rules, SIGNAL(clicked()), this,
			SLOT(onClickGenerateRules()));

	}

	void RuleController::refreshModel() {
		view().getUi().ptx_rules->clear();
		for (int i = 0; i < model().numberOfRules(); ++i) {
			view().getUi().ptx_rules->appendPlainText(QString::fromStdString(
				model().rule(i)->toString()));
		}
	}

	fl::FuzzyEngine& RuleController::model() const {
		return *this->_model;
	}

	GuiMainWindow& RuleController::view() const {
		return *this->_view;
	}

	void RuleController::removeAllRules() {
		std::vector<fl::FuzzyRule*> rules = model().removeAllRules();
		for (size_t i = 0; i < rules.size(); ++i) {
			delete rules[i];
		}
	}
	void RuleController::onClickGenerateRules() {
		removeAllRules();
		view().getUi().ptx_rules->clear();
		int number_of_rules = 1;
		for (int i = 0; i < model().numberOfInputLVars(); ++i) {
			number_of_rules *= model().inputLVar(i)->numberOfTerms();
		}
		int* terms = new int[model().numberOfInputLVars()];
		for (int i = 0; i < model().numberOfInputLVars(); ++i) {
			terms[i] = 0;
		}
		for (int n_rule = 0; n_rule < number_of_rules; ++n_rule) {
			std::string rule(fl::MamdaniRule::FR_IF);
			for (int input = 0; input < model().numberOfInputLVars(); ++input) {
				fl::InputLVar& lvar = *model().inputLVar(input);
				rule += " " + lvar.name() + " " + fl::MamdaniRule::FR_IS + " "
					+ lvar.term(terms[input])->name() + " ";
				if (input < model().numberOfInputLVars() - 1) {
					rule += fl::MamdaniRule::FR_AND;
				}
			}
			rule += fl::MamdaniRule::FR_THEN;
			for (int i = 0; i < model().numberOfOutputLVars(); ++i) {
				rule += " " + model().outputLVar(i)->name() + " "
					+ fl::MamdaniRule::FR_IS + " ? ";
				if (i < model().numberOfOutputLVars() - 1) {
					rule += fl::MamdaniRule::FR_AND;
				}
			}
			view().getUi().ptx_rules->appendPlainText(QString::fromStdString(rule));
			//Increment...
			bool change = true;
			for (int i = model().numberOfInputLVars() - 1; i >= 0; --i) {
				if (change) {
					if (terms[i] + 1 >= model().inputLVar(i)->numberOfTerms()) {
						terms[i] = 0;
						change = true;
					} else {
						++terms[i];
						change = false;
					}
				}
			}
		}
		delete terms;
	}

	void RuleController::onClickCheckSpell() {
		removeAllRules();
		QStringList rules = view().getUi().ptx_rules->toPlainText().split("\n",
			QString::SkipEmptyParts);
		view().getUi().ptx_rules->clear();
		QTextCursor cursor = view().getUi().ptx_rules->textCursor();
		QTextCharFormat f;

		fl::MamdaniRule* rule = NULL;
		int good_rules = 0;
		int bad_rules = 0;

		for (int i = 0; i < rules.size(); ++i) {
			if (rules[i].trimmed().startsWith("#")) {
				continue;
			}
			rule = new fl::MamdaniRule();
			try {
				rule->parse(rules[i].toStdString(), model());
				f.setForeground(QColor(Qt::darkGreen));
				cursor.insertText(rules[i] + "\n", f);
				model().addRule(*rule);
				++good_rules;
			} catch (fl::RuleParsingException& e) {
				delete rule;
				f.setForeground(QColor(Qt::red));
				cursor.insertText(rules[i] + "\n", f);
				f.setForeground(QColor(Qt::darkRed));
				cursor.insertText(
					"    #" + QString::fromStdString(e.getMessage()) + "\n", f);
				++bad_rules;
			}
		}

		f.setForeground(QColor(Qt::darkBlue));
		std::stringstream ss;
		ss << "# Total rules: " << bad_rules + good_rules << ". Good Rules: "
			<< good_rules << ". Bad Rules: " << bad_rules << ".";
		cursor.insertText(QString::fromStdString(ss.str()), f);

	}
}
