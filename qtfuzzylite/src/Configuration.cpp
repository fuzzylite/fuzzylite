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
            _andOperators.push_back(Minimum().className());
            _andOperators.push_back(AlgebraicProduct().className());
            _andOperators.push_back(BoundedDifference().className());
            _andOperators.push_back(DrasticProduct().className());
            _andOperators.push_back(EinsteinProduct().className());
            _andOperators.push_back(HamacherProduct().className());

            _orOperators.push_back(Maximum().className());
            _orOperators.push_back(AlgebraicSum().className());
            _orOperators.push_back(BoundedSum().className());
            _orOperators.push_back(NormalizedSum().className());
            _orOperators.push_back(DrasticSum().className());
            _orOperators.push_back(EinsteinSum().className());
            _orOperators.push_back(HamacherSum().className());

            _defuzzifiers.push_back(Centroid().className());
            _defuzzifiers.push_back(SmallestOfMaximum().className());
            _defuzzifiers.push_back(LargestOfMaximum().className());
            _defuzzifiers.push_back(MeanOfMaximum().className());

        }

        Configuration::~Configuration() {
            disconnect();
            delete ui;
        }

        void Configuration::setup() {
            ui->setupUi(this);

            QStringList andOperators;

            for (std::size_t i = 0; i < _andOperators.size(); ++i) {
                andOperators.append(QString::fromStdString(_andOperators[i]));
            }
            ui->cbx_tnorm->addItems(andOperators);
            ui->cbx_activation->addItems(andOperators);

            QStringList orOperators;
            for (std::size_t i = 0; i < _orOperators.size(); ++i) {
                orOperators.append(QString::fromStdString(_orOperators[i]));
            }
            ui->cbx_snorm->addItems(orOperators);
            ui->cbx_accumulation->addItems(orOperators);

            QStringList defuzzifiers;
            for (std::size_t i = 0; i < _defuzzifiers.size(); ++i) {
                defuzzifiers.append(QString::fromStdString(_defuzzifiers[i]));
            }
            ui->cbx_defuzzifier->addItems(defuzzifiers);

            ui->sbx_divisions->setValue(FL_DEFAULT_DIVISIONS);

            connect();
        }

        void Configuration::connect() {
            QObject::connect(ui->cbx_tnorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::connect(ui->cbx_snorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::connect(ui->cbx_activation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::connect(ui->cbx_accumulation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::connect(ui->cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::connect(ui->sbx_divisions, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));

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
                    this, SLOT(onChangeConfiguration(int)));
            QObject::disconnect(ui->cbx_snorm, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::disconnect(ui->cbx_activation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::disconnect(ui->cbx_accumulation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::disconnect(ui->cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));
            QObject::disconnect(ui->sbx_divisions, SIGNAL(valueChanged(int)),
                    this, SLOT(onChangeConfiguration(int)));

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
            return;
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
            int divisions = FL_DEFAULT_DIVISIONS;
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
            }
            for (int i = 0; i < engine->numberOfHedges(); ++i) {
                ui->ckx_any->setChecked(engine->getHedge(i)->name() == Any().name());
                ui->ckx_extremely->setChecked(engine->getHedge(i)->name() == Extremely().name());
                ui->ckx_not->setChecked(engine->getHedge(i)->name() == Not().name());
                ui->ckx_seldom->setChecked(engine->getHedge(i)->name() == Seldom().name());
                ui->ckx_somewhat->setChecked(engine->getHedge(i)->name() == Somewhat().name());
                ui->ckx_very->setChecked(engine->getHedge(i)->name() == Very().name());
            }
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

        void Configuration::onChangeTNorm(int){
            
        }
        
        void Configuration::onChangeConfiguration(int) {
            std::string tnorm = ui->cbx_tnorm->currentText().toStdString();
            std::string snorm = ui->cbx_snorm->currentText().toStdString();
            std::string activation = ui->cbx_activation->currentText().toStdString();

            std::string accumulation = ui->cbx_accumulation->currentText().toStdString();
            std::string defuzzifier = ui->cbx_defuzzifier->currentText().toStdString();
            int divisions = ui->sbx_divisions->value();
            fl::Engine* engine = Model::Default()->engine();
            engine->configure(tnorm, snorm, activation, accumulation, defuzzifier, divisions);
        }

        void Configuration::onChangeHedgeSelection(int) {
            fl::Engine* engine = Model::Default()->engine();
            for (int i = engine->numberOfHedges() - 1; i >= 0; --i) {
                delete engine->removeHedge(i);
            }

            if (ui->ckx_any->isChecked() and not engine->hasHedge(Any().name()))
                engine->addHedge(new Any);

            if (ui->ckx_extremely->isChecked() and not engine->hasHedge(Extremely().name()))
                engine->addHedge(new Extremely);

            if (ui->ckx_not->isChecked() and not engine->hasHedge(Not().name()))
                engine->addHedge(new Not);

            if (ui->ckx_seldom->isChecked() and not engine->hasHedge(Seldom().name()))
                engine->addHedge(new Seldom);

            if (ui->ckx_somewhat->isChecked() and not engine->hasHedge(Somewhat().name()))
                engine->addHedge(new Somewhat);

            if (ui->ckx_very->isChecked() and not engine->hasHedge(Very().name()))
                engine->addHedge(new Very);

            Window::mainWindow()->fixDependencies();
        }

        void Configuration::main() {
            Configuration* c = new Configuration;
            c->setup();
            c->show();

        }

    }
}
