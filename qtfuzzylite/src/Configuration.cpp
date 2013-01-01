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
            _andOperators.push_back(std::pair<std::string, TNorm*>
                    (Minimum().className(), new Minimum));
            _andOperators.push_back(std::pair<std::string, TNorm*>
                    (AlgebraicProduct().className(), new AlgebraicProduct));
            _andOperators.push_back(std::pair<std::string, TNorm*>
                    (BoundedDifference().className(), new BoundedDifference));
            _andOperators.push_back(std::pair<std::string, TNorm*>
                    (DrasticProduct().className(), new DrasticProduct));
            _andOperators.push_back(std::pair<std::string, TNorm*>
                    (EinsteinProduct().className(), new EinsteinProduct));
            _andOperators.push_back(std::pair<std::string, TNorm*>
                    (HamacherProduct().className(), new HamacherProduct));

            _orOperators.push_back(std::pair<std::string, SNorm*>
                    (Maximum().className(), new Maximum));
            _orOperators.push_back(std::pair<std::string, SNorm*>
                    (AlgebraicSum().className(), new AlgebraicSum));
            _orOperators.push_back(std::pair<std::string, SNorm*>
                    (BoundedSum().className(), new BoundedSum));
            _orOperators.push_back(std::pair<std::string, SNorm*>
                    (NormalizedSum().className(), new NormalizedSum));
            _orOperators.push_back(std::pair<std::string, SNorm*>
                    (DrasticSum().className(), new DrasticSum));
            _orOperators.push_back(std::pair<std::string, SNorm*>
                    (EinsteinSum().className(), new EinsteinSum));
            _orOperators.push_back(std::pair<std::string, SNorm*>
                    (HamacherSum().className(), new HamacherSum));

            _defuzzifiers.push_back(std::pair<std::string, Defuzzifier*>
                    (CenterOfGravity().name(), new CenterOfGravity));
            _defuzzifiers.push_back(std::pair<std::string, Defuzzifier*>
                    (SmallestOfMaximum().name(), new SmallestOfMaximum));
            _defuzzifiers.push_back(std::pair<std::string, Defuzzifier*>
                    (LargestOfMaximum().name(), new LargestOfMaximum));
            _defuzzifiers.push_back(std::pair<std::string, Defuzzifier*>
                    (MeanOfMaximum().name(), new MeanOfMaximum));

        }

        Configuration::~Configuration() {
            disconnect();
            delete ui;
        }

        void Configuration::setup() {
            ui->setupUi(this);

            QStringList andOperators;

            for (std::size_t i = 0; i < _andOperators.size(); ++i) {
                andOperators.append(QString::fromStdString(_andOperators[i].first));
            }
            ui->cbx_tnorm->addItems(andOperators);
            ui->cbx_activation->addItems(andOperators);

            QStringList orOperators;
            for (std::size_t i = 0; i < _orOperators.size(); ++i) {
                orOperators.append(QString::fromStdString(_orOperators[i].first));
            }
            ui->cbx_snorm->addItems(orOperators);
            ui->cbx_accumulation->addItems(orOperators);

            QStringList defuzzifiers;
            for (std::size_t i = 0; i < _defuzzifiers.size(); ++i) {
                defuzzifiers.append(QString::fromStdString(_defuzzifiers[i].first));
            }
            ui->cbx_defuzzifier->addItems(defuzzifiers);

            ui->sbx_divisions->setValue(FL_DEFAULT_DIVISIONS);

            this->adjustSize();
            if (parentWidget()) {
                QRect scr = parentWidget()->geometry();
                move(scr.center() - rect().center());
            }
            refresh();
            connect();
        }

        void Configuration::connect() {
            QObject::connect(ui->cbx_tnorm, SIGNAL(currentIndexChanged(int)), this,
                    SLOT(onChangeTNorm(int)));
            QObject::connect(ui->cbx_snorm, SIGNAL(currentIndexChanged(int)), this,
                    SLOT(onChangeSNorm(int)));
            QObject::connect(ui->cbx_activation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeActivation(int)));
            QObject::connect(ui->cbx_accumulation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeAccumulation(int)));
            QObject::connect(ui->cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeDefuzzifier(int)));
            QObject::connect(ui->sbx_divisions, SIGNAL(valueChanged(int)), this,
                    SLOT(onChangeDivisions(int)));
        }

        void Configuration::disconnect() {
            QObject::disconnect(ui->cbx_tnorm, SIGNAL(currentIndexChanged(int)), this,
                    SLOT(onChangeTNorm(int)));
            QObject::disconnect(ui->cbx_snorm, SIGNAL(currentIndexChanged(int)), this,
                    SLOT(onChangeSNorm(int)));
            QObject::disconnect(ui->cbx_activation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeActivation(int)));
            QObject::disconnect(ui->cbx_accumulation, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeAccumulation(int)));
            QObject::disconnect(ui->cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
                    this, SLOT(onChangeDefuzzifier(int)));
            QObject::disconnect(ui->sbx_divisions, SIGNAL(valueChanged(int)), this,
                    SLOT(onChangeDivisions(int)));
        }

        void Configuration::refresh() {
            fl::Configuration* configuration = Model::Default()->configuration();
            std::string tnorm = configuration->getTnorm()->className();
            for (std::size_t i = 0; i < _andOperators.size(); ++i) {
                if (_andOperators[i].first == tnorm) {
                    ui->cbx_tnorm->setCurrentIndex(i);
                }
            }

            std::string activation = configuration->getActivation()->className();
            for (std::size_t i = 0; i < _andOperators.size(); ++i) {
                if (_andOperators[i].first == activation) {
                    ui->cbx_activation->setCurrentIndex(i);
                }
            }

            std::string snorm = configuration->getSnorm()->className();
            for (std::size_t i = 0; i < _orOperators.size(); ++i) {
                if (_orOperators[i].first == snorm) {
                    ui->cbx_snorm->setCurrentIndex(i);
                }
            }

            std::string accumulation = configuration->getAccumulation()->className();
            for (std::size_t i = 0; i < _orOperators.size(); ++i) {
                if (_orOperators[i].first == accumulation) {
                    ui->cbx_accumulation->setCurrentIndex(i);
                }
            }

            std::string defuzzifier = configuration->getDefuzzifier()->name();
            for (std::size_t i = 0; i < _defuzzifiers.size(); ++i) {
                if (_defuzzifiers[i].first == defuzzifier) {
                    ui->cbx_defuzzifier->setCurrentIndex(i);
                }
            }
            ui->sbx_divisions->setValue(configuration->getDefuzzifier()->getDivisions());

            std::ostringstream ossConfig;
            ossConfig << configuration->toString()
                    << " with divisions="
                    << configuration->getDefuzzifier()->getDivisions();
            ui->ted_configuration->document()->setPlainText(
                    QString::fromStdString(ossConfig.str()));
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
