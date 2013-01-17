/*
 * Configuration.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Configuration.h"

#include "fl/qt/Window.h"

#include "fl/qt/Model.h" 
 
#include <QtGui/QMessageBox>
#include <sstream>

namespace fl {
    namespace qt {

        Configuration::Configuration(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f), ui(new Ui::Configuration) {
            setWindowFlags(Qt::Tool);

        }

        Configuration::~Configuration() {
            disconnect();
            delete ui;
        }

        void Configuration::setup() {
            ui->setupUi(this);
            layout()->setSizeConstraint(QLayout::SetFixedSize);
            this->adjustSize();
            QStringList andOperators;
            std::vector<std::string> tnorms = fl::Factory::instance()->tnorm()->available();
            for (std::size_t i = 0; i < tnorms.size(); ++i) {
                andOperators.append(QString::fromStdString(tnorms.at(i)));
            }
            ui->cbx_tnorm->addItems(andOperators);
            ui->cbx_activation->addItems(andOperators);

            QStringList orOperators;
            std::vector<std::string> snorms = fl::Factory::instance()->snorm()->available();
            for (std::size_t i = 0; i < snorms.size(); ++i) {
                orOperators.append(QString::fromStdString(snorms.at(i)));
            }
            ui->cbx_snorm->addItems(orOperators);
            ui->cbx_accumulation->addItems(orOperators);

            std::vector<std::string> defuzzifiers = fl::Factory::instance()->defuzzifier()->available();
            for (std::size_t i = 0; i < defuzzifiers.size(); ++i) {
                ui->cbx_defuzzifier->addItem(QString::fromStdString(defuzzifiers[i]));
            }

            ui->sbx_divisions->setValue(FL_DIVISIONS);
            
            connect();
        }

        void Configuration::connect() {
            QObject::connect(ui->cbx_tnorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeTNorm(int)));
            QObject::connect(ui->cbx_snorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeSNorm(int)));
            QObject::connect(ui->cbx_activation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeActivation(int)));
            QObject::connect(ui->cbx_accumulation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeAccumulation(int)));
            QObject::connect(ui->cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeDefuzzifier(int)));
            QObject::connect(ui->sbx_divisions, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeDivisions(int)));

            QObject::connect(ui->ckx_any, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::connect(ui->ckx_extremely, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::connect(ui->ckx_not, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::connect(ui->ckx_seldom, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::connect(ui->ckx_somewhat, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::connect(ui->ckx_very, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));


        }

        void Configuration::disconnect() {
            QObject::disconnect(ui->cbx_tnorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeTNorm(int)));
            QObject::disconnect(ui->cbx_snorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeSNorm(int)));
            QObject::disconnect(ui->cbx_activation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeActivation(int)));
            QObject::disconnect(ui->cbx_accumulation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeAccumulation(int)));
            QObject::disconnect(ui->cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeDefuzzifier(int)));
            QObject::disconnect(ui->sbx_divisions, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeDivisions(int)));

            QObject::disconnect(ui->ckx_any, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::disconnect(ui->ckx_extremely, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::disconnect(ui->ckx_not, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::disconnect(ui->ckx_seldom, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::disconnect(ui->ckx_somewhat, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
            QObject::disconnect(ui->ckx_very, SIGNAL(stateChanged(int)),
                    this, SLOT(onChangeHedgeSelection(int)));
        }

        void Configuration::showEvent(QShowEvent*) {
            this->adjustSize();
            QRect scr = Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());

            applyDefaults();
            loadFromModel();
        }

        void Configuration::applyDefaults() {
            fl::Engine* engine = Model::Default()->engine();

            if (engine->numberOfRuleBlocks() == 0) {
                QMessageBox::critical(this, "Error",
                        "Current engine has no rule blocks. "
                        "At least one ruleblock was expected.",
                        QMessageBox::Ok);
                return;
            }
            std::string tnorm = Minimum().className();
            std::string snorm = Maximum().className();
            std::string activation = Minimum().className();

            RuleBlock* ruleblock = engine->getRuleBlock(0);
            if (ruleblock->getTnorm()) tnorm = ruleblock->getTnorm()->className();
            if (ruleblock->getSnorm()) snorm = ruleblock->getSnorm()->className();
            if (ruleblock->getActivation()) activation = ruleblock->getActivation()->className();

            std::string defuzzifier = Centroid().className();
            int divisions = FL_DIVISIONS;
            std::string accumulation = Maximum().className();

            if (engine->numberOfOutputVariables() > 0) {
                OutputVariable* variable = engine->getOutputVariable(0);
                if (variable->getDefuzzifier()) {
                    defuzzifier = variable->getDefuzzifier()->className();
                    divisions = variable->getDefuzzifier()->getDivisions();
                }
                if (variable->output()->getAccumulation())
                    accumulation = variable->output()->getAccumulation()->className();
            }
            engine->configure(tnorm, snorm, activation, accumulation, defuzzifier, divisions);
        }

        void Configuration::loadFromModel() {
            disconnect();
            fl::Engine* engine = Model::Default()->engine();
            if (engine->numberOfRuleBlocks() == 0) {
                QMessageBox::critical(this, "Error",
                        "Current engine has no rule blocks. "
                        "At least one ruleblock was expected.",
                        QMessageBox::Ok);
                return;
            }
            RuleBlock* ruleblock = engine->getRuleBlock(0);
            ui->cbx_tnorm->setCurrentIndex(indexOfTnorm(ruleblock->getTnorm()->className()));
            ui->cbx_snorm->setCurrentIndex(indexOfSnorm(ruleblock->getSnorm()->className()));
            ui->cbx_activation->setCurrentIndex(indexOfTnorm(ruleblock->getActivation()->className()));
            if (engine->numberOfOutputVariables() > 0) {
                OutputVariable* variable = engine->getOutputVariable(0);
                ui->cbx_defuzzifier->setCurrentIndex(
                        indexOfDefuzzifier(variable->getDefuzzifier()->className()));
                ui->sbx_divisions->setValue(variable->getDefuzzifier()->getDivisions());

                ui->cbx_accumulation->setCurrentIndex(
                        indexOfSnorm(variable->output()->getAccumulation()->className()));
            } else {
                ui->cbx_defuzzifier->setCurrentIndex(0);
                ui->sbx_divisions->setValue(FL_DIVISIONS);
                ui->cbx_accumulation->setCurrentIndex(0);
            }

            ui->ckx_any->setChecked(engine->hasHedge(Any().name()));

            ui->ckx_extremely->setChecked(engine->hasHedge(Extremely().name()));
            ui->ckx_not->setChecked(engine->hasHedge(Not().name()));
            ui->ckx_seldom->setChecked(engine->hasHedge(Seldom().name()));
            ui->ckx_somewhat->setChecked(engine->hasHedge(Somewhat().name()));
            ui->ckx_very->setChecked(engine->hasHedge(Very().name()));

            connect();
        }

        int Configuration::indexOfTnorm(const std::string& tnorm) {
            int result = ui->cbx_tnorm->findText(QString::fromStdString(tnorm));
            if (result == -1) throw fl::Exception("[internal error] T-Norm <"
                    + tnorm + "> not registered", FL_AT);
            return result;
        }

        int Configuration::indexOfSnorm(const std::string& snorm) {
            int result = ui->cbx_snorm->findText(QString::fromStdString(snorm));
            if (result == -1) throw fl::Exception("[internal error] S-Norm <"
                    + snorm + "> not registered", FL_AT);
            return result;
        }

        int Configuration::indexOfDefuzzifier(const std::string& defuzzifier) {
            int result = ui->cbx_defuzzifier->findText(QString::fromStdString(defuzzifier));
            if (result == -1) throw fl::Exception("[internal error] Defuzzifier <"
                    + defuzzifier + "> not registered", FL_AT);
            return result;
        }

        void Configuration::onChangeTNorm(int) {
            std::string tnorm = ui->cbx_tnorm->currentText().toStdString();
            fl::Engine* engine = Model::Default()->engine();
            for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
                RuleBlock* ruleblock = engine->getRuleBlock(i);
                ruleblock->setTnorm(Factory::instance()->tnorm()->create(tnorm));
            }
        }

        void Configuration::onChangeSNorm(int) {
            std::string snorm = ui->cbx_snorm->currentText().toStdString();
            fl::Engine* engine = Model::Default()->engine();
            for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
                RuleBlock* ruleblock = engine->getRuleBlock(i);
                ruleblock->setSnorm(Factory::instance()->snorm()->create(snorm));
            }
        }

        void Configuration::onChangeActivation(int) {
            std::string activation = ui->cbx_activation->currentText().toStdString();
            fl::Engine* engine = Model::Default()->engine();
            for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
                RuleBlock* ruleblock = engine->getRuleBlock(i);
                ruleblock->setActivation(Factory::instance()->tnorm()->create(activation));
            }
        }

        void Configuration::onChangeAccumulation(int) {
            std::string accumulation = ui->cbx_accumulation->currentText().toStdString();
            fl::Engine* engine = Model::Default()->engine();
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* outputVariable = engine->getOutputVariable(i);
                outputVariable->output()->setAccumulation(
                        Factory::instance()->snorm()->create(accumulation));
            }
        }

        void Configuration::onChangeDefuzzifier(int) {
            std::string defuzzifier = ui->cbx_defuzzifier->currentText().toStdString();
            fl::Engine* engine = Model::Default()->engine();
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* outputVariable = engine->getOutputVariable(i);
                outputVariable->setDefuzzifier(
                        Factory::instance()->defuzzifier()->create(defuzzifier));
            }
        }

        void Configuration::onChangeDivisions(int value) {
            fl::Engine* engine = Model::Default()->engine();
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* outputVariable = engine->getOutputVariable(i);
                outputVariable->getDefuzzifier()->setDivisions(value);
            }
        }

        void Configuration::onChangeHedgeSelection(int) {
            fl::Engine* engine = Model::Default()->engine();
            if (ui->ckx_any->isChecked() and not engine->hasHedge(Any().name()))
                engine->addHedge(new Any);
            else if (not ui->ckx_any->isChecked() and engine->hasHedge(Any().name()))
                delete engine->removeHedge(Any().name());

            if (ui->ckx_extremely->isChecked() and not engine->hasHedge(Extremely().name()))
                engine->addHedge(new Extremely);
            else if (not ui->ckx_extremely->isChecked() and engine->hasHedge(Extremely().name()))
                delete engine->removeHedge(Extremely().name());

            if (ui->ckx_not->isChecked() and not engine->hasHedge(Not().name()))
                engine->addHedge(new Not);
            else if (not ui->ckx_not->isChecked() and engine->hasHedge(Not().name()))
                delete engine->removeHedge(Not().name());

            if (ui->ckx_seldom->isChecked() and not engine->hasHedge(Seldom().name()))
                engine->addHedge(new Seldom);
            else if (not ui->ckx_seldom->isChecked() and engine->hasHedge(Seldom().name()))
                delete engine->removeHedge(Seldom().name());

            if (ui->ckx_somewhat->isChecked() and not engine->hasHedge(Somewhat().name()))
                engine->addHedge(new Somewhat);
            else if (not ui->ckx_somewhat->isChecked() and engine->hasHedge(Somewhat().name()))
                delete engine->removeHedge(Somewhat().name());

            if (ui->ckx_very->isChecked() and not engine->hasHedge(Very().name()))
                engine->addHedge(new Very);
            else if (not ui->ckx_very->isChecked() and engine->hasHedge(Very().name()))
                delete engine->removeHedge(Very().name());

            Window::mainWindow()->fixDependencies();
        }

        void Configuration::main() {
            Configuration* c = new Configuration;
            c->setup();
            c->show();
        }

    }
}
