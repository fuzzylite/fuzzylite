/*
 * Configuration.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Configuration.h"

#include "fl/qt/Model.h"

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

            this->adjustSize();
            QRect scr = Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());

            loadFromModel();
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

        void Configuration::applyDefaults() {
            fl::Engine* engine = Model::Default()->engine();
            for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
                RuleBlock rb = engine->getRuleBlock(i);
                if (not rb->getTnorm()) rb->setTnorm(new Minimum);
                if (not rb->getSnorm()) rb->setSnorm(new Maximum);
                if (not rb->getActivation()) rb->setActivation(new Minimum);
            }

            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* variable = engine->getOutputVariable(i);
                if (not variable->getDefuzzifier()) variable->setDefuzzifier(new Centroid);
                if (not variable->output()->getAccumulation())
                    variable->output()->setAccumulation(new Maximum);
            }
        }

        void Configuration::loadFromModel() {
            fl::Engine* engine = Model::Default()->engine();
            if (engine->numberOfRuleBlocks() != 0) {
                QMessageBox::critical(this, "Error",
                        "<qt>"
                        "<b>qtfuzzylite</b> only supports engines with a single RuleBlock, "
                        "but found <" + fl::Op::str((scalar) engine->numberOfRuleBlocks(), 0) + ">"
                        "</qt>",
                        QMessageBox::Ok);
                return;
            }

            RuleBlock* rb = engine->getRuleBlock(0);

            ui->cbx_tnorm->setCurrentIndex(indexOfTnorm(rb->getTnorm()->className()));
            ui->cbx_snorm->setCurrentIndex(indexOfSnorm(rb->getSnorm()->className()));
            ui->cbx_activation->setCurrentIndex(indexOfTnorm(rb->getActivation()->className()));

            std::string accumulation, error;
            Defuzzifier* defuzzifier = NULL;
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                OutputVariable* variable = engine->getOutputVariable(i);
                if (not defuzzifier) defuzzifier = variable->getDefuzzifier();
                else if (defuzzifier->className() != variable->getDefuzzifier()->className()) {
                    error = "defuzzifiers";
                    break;
                }

                if (accumulation.empty()) accumulation = variable->output()->getAccumulation()->className();
                else if (accumulation != variable->output()->getAccumulation()->className()) {
                    error = "accumulation S-Norms";
                    break;
                }
            }
            if (not error.empty()) {
                QMessageBox::critical(this, "Error",
                        "<qt><b>qtfuzzylite</b> does not support engines whose "
                        "output variables have different " + error + "</qt>",
                        QMessageBox::Ok);
                return;
            }
            
            ui->cbx_accumulation->setCurrentIndex(indexOfSnorm(accumulation));
            
            ui->cbx_defuzzifier->setCurrentIndex(indexOfDefuzzifier(defuzzifier->className()));
            ui->sbx_divisions->setValue(defuzzifier->getDivisions());
        }

        void Configuration::onChangeTNorm(int index) {
            Model::Default()->configuration()->setTnorm(
                    _andOperators[index].second);
            Model::Default()->update();
            refresh();
        }

        void Configuration::onChangeActivation(int index) {
            Model::Default()->configuration()->setActivation(
                    _andOperators[index].second);
            Model::Default()->update();
            refresh();
        }

        void Configuration::onChangeSNorm(int index) {
            Model::Default()->configuration()->setSnorm(
                    _orOperators[index].second);
            Model::Default()->update();
            refresh();
        }

        void Configuration::onChangeAccumulation(int index) {
            Model::Default()->configuration()->setAccumulation(
                    _orOperators[index].second);
            Model::Default()->update();
            refresh();
        }

        void Configuration::onChangeDefuzzifier(int index) {
            Model::Default()->configuration()->setDefuzzifier(
                    _defuzzifiers[index].second);
            Model::Default()->update();
            refresh();
        }

        void Configuration::onChangeDivisions(int value) {
            Model::Default()->configuration()->getDefuzzifier()->setDivisions(value);
            Model::Default()->update();
            refresh();
        }

        void Configuration::main() {
            Configuration* c = new Configuration;
            c->setup();
            c->show();

        }

    }
}
