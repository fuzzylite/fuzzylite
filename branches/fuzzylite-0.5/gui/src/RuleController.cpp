#include "RuleController.h"

#include <MamdaniRule.h>
#include <QtGui/QTextBlock>
namespace fuzzy_lite_gui {

    RuleController::RuleController(FuzzyEngine& model, GuiMainWindow& view)
    : _model(&model), _view(&view) {
        connect();
    }

    RuleController::~RuleController() {
        disconnect();
    }

    void RuleController::connect() {
        QObject::connect(getView().getUi().btn_check_rules, SIGNAL(clicked()),
                this, SLOT(onClickCheckSpell()));
        QObject::connect(getView().getUi().btn_generate_rules, SIGNAL(clicked()),
                this, SLOT(onClickGenerateRules()));
        for (int i = 0 ; i < getModel().numberOfRules() ; ++i){
            getView().getUi().ptx_rules->appendPlainText(
                    QString::fromStdString(getModel().getRule(i).toString()));
        }
    }

    void RuleController::disconnect() {
        QObject::disconnect(getView().getUi().btn_check_rules, SIGNAL(clicked()),
                this, SLOT(onClickCheckSpell()));
        QObject::disconnect(getView().getUi().btn_generate_rules, SIGNAL(clicked()),
                this, SLOT(onClickGenerateRules()));

    }

    FuzzyEngine& RuleController::getModel() const {
        return *this->_model;
    }

    GuiMainWindow& RuleController::getView() const {
        return *this->_view;
    }

    void RuleController::removeAllRules(){
        std::vector<FuzzyRule*> rules = getModel().removeAllRules();
        for (int i = 0 ; i < rules.size() ; ++i){
            delete rules[i];
        }
    }
    void RuleController::onClickGenerateRules() {
        removeAllRules();
        getView().getUi().ptx_rules->clear();
        int number_of_rules = 1;
        for (int i = 0; i < getModel().numberOfInputLVars(); ++i) {
            number_of_rules *= getModel().getInputLVar(i).numberOfTerms();
        }
        int* terms = new int[getModel().numberOfInputLVars()];
        for (int i = 0; i < getModel().numberOfInputLVars(); ++i) {
            terms[i] = 0;
        }
        for (int n_rule = 0; n_rule < number_of_rules; ++n_rule) {
            std::string rule(MamdaniRule::FR_IF);
            for (int input = 0; input < getModel().numberOfInputLVars(); ++input) {
                InputLVar& lvar = getModel().getInputLVar(input);
                rule += " " + lvar.getName() + " " + MamdaniRule::FR_IS + " " +
                        lvar.getTerm(terms[input]).getName() + " ";
                if (input < getModel().numberOfInputLVars() - 1) {
                    rule += MamdaniRule::FR_AND;
                }
            }
            rule += MamdaniRule::FR_THEN;
            for (int i = 0; i < getModel().numberOfOutputLVars(); ++i) {
                rule += " " + getModel().getOutputLVar(i).getName() + " " +
                        MamdaniRule::FR_IS + " ? ";
                if (i < getModel().numberOfOutputLVars() - 1) {
                    rule += MamdaniRule::FR_AND;
                }
            }
            getView().getUi().ptx_rules->appendPlainText(QString::fromStdString(rule));
            //Increment...
            bool change = true;
            for (int i = getModel().numberOfInputLVars() - 1; i >= 0; --i) {
                if (change) {
                    if (terms[i] + 1 >= getModel().getInputLVar(i).numberOfTerms()) {
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
        QStringList rules = getView().getUi().ptx_rules->toPlainText().split("\n", QString::SkipEmptyParts);
        getView().getUi().ptx_rules->clear();
        QTextCursor cursor = getView().getUi().ptx_rules->textCursor();
        QTextCharFormat f;

        MamdaniRule* rule = NULL;
        int good_rules = 0;
        int bad_rules = 0;

        for (int i = 0; i < rules.size(); ++i) {
            if (rules[i].trimmed().startsWith("#")) {
                continue;
            }
            rule = new MamdaniRule();
            try {
                rule->parse(rules[i].toStdString(), getModel());
                f.setForeground(QColor(Qt::darkGreen));
                cursor.insertText(rules[i] + "\n", f);
                getModel().addRule(*rule);
                ++good_rules;
            } catch (RuleParsingException& e) {
                delete rule;
                f.setForeground(QColor(Qt::red));
                cursor.insertText(rules[i] + "\n", f);
                f.setForeground(QColor(Qt::darkRed));
                cursor.insertText("    #" + QString::fromStdString(e.getMessage()) + "\n", f);
                ++bad_rules;
            }
        }

        f.setForeground(QColor(Qt::darkBlue));
        std::stringstream ss;
        ss << "# Total rules: " << bad_rules + good_rules << ". Good Rules: " << good_rules << ". Bad Rules: " << bad_rules << ".";
        cursor.insertText(QString::fromStdString(ss.str()), f);

    }
}
