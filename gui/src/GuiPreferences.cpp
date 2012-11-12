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
#include "gui/GuiPreferences.h"

namespace fl_gui {

GuiPreferences::GuiPreferences(fl::FuzzyOperator& model, QWidget* parent,
		Qt::WindowFlags f) :
		QDialog(parent, f), _model(&model), _ui(new Ui::Preferences) {

}

GuiPreferences::~GuiPreferences() {
	disconnect();
	delete _ui;
}

Ui::Preferences& GuiPreferences::getUi() const {
	return *this->_ui;
}

fl::FuzzyOperator& GuiPreferences::getModel() const {
	return *this->_model;
}

void GuiPreferences::setup() {
	getUi().setupUi(this);
	connect();
}

void GuiPreferences::load() {

	std::string name;
	//TNORM
	name = getModel().tnorm().name();
	if (name == fl::FuzzyAndMin().name()) {
		getUi().cbx_tnorm->setCurrentIndex(0);
	} else if (name == fl::FuzzyAndProd().name()) {
		getUi().cbx_tnorm->setCurrentIndex(1);
	} else if (name == fl::FuzzyAndBDiff().name()) {
		getUi().cbx_tnorm->setCurrentIndex(2);
	} else {
		getUi().cbx_tnorm->setCurrentIndex(-1);
		FL_LOG("Unknown TNorm <<" << name << ">>");
	}
	//SNORM
	name = getModel().snorm().name();
	if (name == fl::FuzzyOrMax().name()) {
		getUi().cbx_snorm->setCurrentIndex(0);
	} else if (name == fl::FuzzyOrSum().name()) {
		getUi().cbx_snorm->setCurrentIndex(1);
	} else if (name == fl::FuzzyOrBSum().name()) {
		getUi().cbx_snorm->setCurrentIndex(2);
	} else {
		getUi().cbx_snorm->setCurrentIndex(-1);
		FL_LOG("Unknown SNorm <<" << name << ">>");
	}
	//Modulation
	name = getModel().modulation().name();
	if (name == fl::FuzzyModClip().name()) {
		getUi().cbx_modulation->setCurrentIndex(0);
	} else if (name == fl::FuzzyModScale().name()) {
		getUi().cbx_modulation->setCurrentIndex(1);
	} else {
		getUi().cbx_modulation->setCurrentIndex(-1);
		FL_LOG("Unknown Modulation <<" << name << ">>");
	}
	//Algorithm
	name = getModel().acAlgorithm().name();
	if (name == fl::TriangulationAlgorithm().name()) {
		getUi().cbx_algorithm->setCurrentIndex(0);
//        } else if (name == fl::TrapezoidalAlgorithm().name()) {
//            getUi().cbx_algorithm->setCurrentIndex(1);
	} else if (name == fl::MidPointRectangle().name()) {
//        	getUi().cbx_algorithm->setCurrentIndex(2);
		getUi().cbx_algorithm->setCurrentIndex(1);
	} else {
		getUi().cbx_algorithm->setCurrentIndex(-1);
		FL_LOG("Unknown Algorithm <<" << name << ">>");
	}
	//Defuzzifier
	name = getModel().defuzzifier().name();
	if (name == fl::CoGDefuzzifier().name()) {
		getUi().cbx_defuzzifier->setCurrentIndex(0);
	} else if (name == fl::TakagiSugenoDefuzzifier().name()) {
		getUi().cbx_defuzzifier->setCurrentIndex(1);
	} else {
		getUi().cbx_defuzzifier->setCurrentIndex(-1);
		FL_LOG("Unknown Defuzzifier <<" << name << ">>");
	}
	//Aggregation
	name = getModel().aggregation().name();
	if (name == fl::FuzzyOrMax().name()) {
		getUi().cbx_aggregation->setCurrentIndex(0);
	} else if (name == fl::FuzzyOrSum().name()) {
		getUi().cbx_aggregation->setCurrentIndex(1);
	} else if (name == fl::FuzzyOrBSum().name()) {
		getUi().cbx_aggregation->setCurrentIndex(2);
	} else {
		getUi().cbx_aggregation->setCurrentIndex(-1);
		FL_LOG("Unknown Aggregation <<" << name << ">>");
	}

	getUi().isx_sample_size->setValue(getModel().numberOfSamples());

}

void GuiPreferences::connect() {
	load();
	QObject::connect(getUi().cbx_tnorm, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeTNorm(int)));
	QObject::connect(getUi().cbx_snorm, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeSNorm(int)));
	QObject::connect(getUi().cbx_modulation, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeModulation(int)));
	QObject::connect(getUi().cbx_defuzzifier, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeDefuzzifier(int)));
	QObject::connect(getUi().cbx_aggregation, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeAggregation(int)));
	QObject::connect(getUi().cbx_algorithm, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeAlgorithm(int)));
	QObject::connect(getUi().isx_sample_size, SIGNAL(valueChanged(int)), this,
			SLOT(onChangeSampleSize(int)));

}

void GuiPreferences::disconnect() {
	QObject::disconnect(getUi().cbx_tnorm, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeTNorm(int)));
	QObject::disconnect(getUi().cbx_snorm, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeSNorm(int)));
	QObject::disconnect(getUi().cbx_modulation,
			SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeModulation(int)));
	QObject::disconnect(getUi().cbx_defuzzifier,
			SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeDefuzzifier(int)));
	QObject::disconnect(getUi().cbx_aggregation,
			SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeAggregation(int)));
	QObject::disconnect(getUi().isx_sample_size, SIGNAL(valueChanged(int)),
			this, SLOT(onChangeSampleSize(int)));
}

void GuiPreferences::onChangeTNorm(int index) {
	switch (index) {
	case 0:
		getModel().setTnorm(new fl::FuzzyAndMin);
		break;
	case 1:
		getModel().setTnorm(new fl::FuzzyAndProd);
		break;
	case 2:
		getModel().setTnorm(new fl::FuzzyAndBDiff);
		break;
	default:
		FL_LOG("Invalid TNorm index" << index);
	}
}

void GuiPreferences::onChangeSNorm(int index) {
	switch (index) {
	case 0:
		getModel().setSnorm(new fl::FuzzyOrMax);
		break;
	case 1:
		getModel().setSnorm(new fl::FuzzyOrSum);
		break;
	case 2:
		getModel().setSnorm(new fl::FuzzyOrBSum);
		break;
	default:
		FL_LOG("Invalid SNorm index" << index);
	}
}

void GuiPreferences::onChangeModulation(int index) {
	switch (index) {
	case 0:
		getModel().setModulation(new fl::FuzzyModClip);
		break;
	case 1:
		getModel().setModulation(new fl::FuzzyModScale);
		break;
	default:
		FL_LOG("Invalid Modulation index" << index);
	}
}

void GuiPreferences::onChangeAggregation(int index) {
	switch (index) {
	case 0:
		getModel().setAggregation(new fl::FuzzyOrMax);
		break;
	case 1:
		getModel().setAggregation(new fl::FuzzyOrSum);
		break;
	case 2:
		getModel().setAggregation(new fl::FuzzyOrBSum);
		break;
	default:
		FL_LOG("Invalid Aggregation index" << index);
	}
}

void GuiPreferences::onChangeAlgorithm(int index) {
	FL_LOG("Changing algorithm to " << index);
	switch (index) {
	case 0:
		getModel().setAreaCentroidAlgorithm(new fl::TriangulationAlgorithm);
		break;
	case 1:
//		getModel().setAreaCentroidAlgorithm(new fl::TrapezoidalAlgorithm);
		getModel().setAreaCentroidAlgorithm(new fl::MidPointRectangle);
		break;
//	case 2:
//		getModel().setAreaCentroidAlgorithm(new fl::MidPointRectangle);
//		break;
	default:
		FL_LOG("Invalid Algorithm index" << index);
	} FL_LOG("Algorithm: " << getModel().acAlgorithm().name());
}

void GuiPreferences::onChangeDefuzzifier(int index) {
	switch (index) {
	case 0:
		getModel().setDefuzzifier(new fl::CoGDefuzzifier);
		break;
	case 1:
		getModel().setDefuzzifier(new fl::TakagiSugenoDefuzzifier);
		break;
	default:
		FL_LOG("Invalid Defuzzifier index" << index);
	}
}

void GuiPreferences::onChangeSampleSize(int samples) {
	getModel().setNumberOfSamples(samples);
}

}
