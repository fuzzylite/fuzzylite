/*   Copyright 2010 Juan Rada-Vilela

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
#include "gui/RuleController.h"

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
        QObject::connect(view().getUi().btn_generate_rules, SIGNAL(clicked()), this,
                SLOT(onClickGenerateRules()));
        QObject::connect(view().getUi().btn_check_rules, SIGNAL(clicked()), this,
                SLOT(onClickCheckSpell()));

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
        for (int i = 0; i < model().ruleBlock(0)->numberOfRules(); ++i) {
            view().getUi().ptx_rules->appendPlainText(QString::fromStdString(
                    model().ruleBlock(0)->rule(i)->toString()));
        }
    }

    fl::FuzzyEngine& RuleController::model() const {
        return *this->_model;
    }

    GuiMainWindow& RuleController::view() const {
        return *this->_view;
    }

    void RuleController::removeAllRules() {
        model().ruleBlock(0)->reset();
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
            std::string rule(fl::FuzzyRule::FR_IF);
            for (int input = 0; input < model().numberOfInputLVars(); ++input) {
                fl::InputLVar& lvar = *model().inputLVar(input);
                rule += " " + lvar.name() + " " + fl::FuzzyRule::FR_IS + " "
                        + lvar.term(terms[input])->name() + " ";
                if (input < model().numberOfInputLVars() - 1) {
                    rule += fl::FuzzyRule::FR_AND;
                }
            }
            rule += fl::FuzzyRule::FR_THEN;
            for (int i = 0; i < model().numberOfOutputLVars(); ++i) {
                rule += " " + model().outputLVar(i)->name() ;
                if (fl::FuzzyOperator::DefaultFuzzyOperator().defuzzifier().name() ==
                        fl::TakagiSugenoDefuzzifier().name()) {
                    rule += " = ";
                } else {
                    rule += " " + fl::FuzzyRule::FR_IS + " ";
                }
                rule += "?";
                if (i < model().numberOfOutputLVars() - 1) {
                    rule += " " +  fl::FuzzyRule::FR_AND;
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

        fl::FuzzyRule* rule = NULL;
        int good_rules = 0;
        int bad_rules = 0;

        for (int i = 0; i < rules.size(); ++i) {
            if (rules[i].trimmed().startsWith("#")) {
                continue;
            }
            if (model().fuzzyOperator().defuzzifier().name() == fl::TakagiSugenoDefuzzifier().name()) {
                rule = new fl::TakagiSugenoRule();
            } else {
                rule = new fl::MamdaniRule();
            }

            try {
                rule->parse(rules[i].toStdString(), model());
                f.setForeground(QColor(Qt::darkGreen));
                cursor.insertText(rules[i] + "\n", f);
                model().ruleBlock(0)->addRule(rule);
                ++good_rules;
            } catch (fl::ParsingException& e) {
                delete rule;

                f.setForeground(QColor(Qt::red));
                cursor.insertText(rules[i] + "\n", f);
                f.setForeground(QColor(Qt::darkRed));
                cursor.insertText(
                        "    #" + QString::fromStdString(e.message()) + "\n", f);
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
