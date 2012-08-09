#include "GuiPreferences.h"

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

	void GuiPreferences::connect() {
		getUi().cbx_tnorm->setCurrentIndex(getModel().getTnorm());
		getUi().cbx_snorm->setCurrentIndex(getModel().getSnorm());
		getUi().cbx_modulation->setCurrentIndex(getModel().getConsequentModulation());
		getUi().cbx_defuzzification->setCurrentIndex(getModel().getDefuzzification());
		getUi().cbx_aggregation->setCurrentIndex(getModel().getAggregation());
		getUi().isx_sample_size->setValue(getModel().samples());

		QObject::connect(getUi().cbx_tnorm, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeTNorm(int)));
		QObject::connect(getUi().cbx_snorm, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeSNorm(int)));
		QObject::connect(getUi().cbx_modulation, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeModulation(int)));
		QObject::connect(getUi().cbx_defuzzification, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeDefuzzification(int)));
		QObject::connect(getUi().cbx_aggregation, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeAggregation(int)));
		QObject::connect(getUi().isx_sample_size, SIGNAL(valueChanged(int)), this,
			SLOT(onChangeSampleSize(int)));

	}

	void GuiPreferences::disconnect() {
		QObject::disconnect(getUi().cbx_tnorm, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeTNorm(int)));
		QObject::disconnect(getUi().cbx_snorm, SIGNAL(currentIndexChanged(int)), this,
			SLOT(onChangeSNorm(int)));
		QObject::disconnect(getUi().cbx_modulation, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeModulation(int)));
		QObject::disconnect(getUi().cbx_defuzzification,
			SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeDefuzzification(int)));
		QObject::disconnect(getUi().cbx_aggregation, SIGNAL(currentIndexChanged(int)),
			this, SLOT(onChangeAggregation(int)));
		QObject::disconnect(getUi().isx_sample_size, SIGNAL(valueChanged(int)), this,
					SLOT(onChangeSampleSize(int)));
	}

	void GuiPreferences::onChangeTNorm(int index) {
		getModel().setTnorm((fl::FuzzyOperator::TNORM) index);
	}

	void GuiPreferences::onChangeSNorm(int index) {
		getModel().setSnorm((fl::FuzzyOperator::SNORM) index);
	}

	void GuiPreferences::onChangeModulation(int index) {
		getModel().setConsequentModulation((fl::FuzzyOperator::CMODULATION) index);
	}

	void GuiPreferences::onChangeDefuzzification(int index) {
		getModel().setDefuzzify((fl::FuzzyOperator::DEFUZZIFY) index);
	}

	void GuiPreferences::onChangeAggregation(int index) {
		getModel().setAggregate((fl::FuzzyOperator::SNORM) index);
	}

	void GuiPreferences::onChangeSampleSize(int samples){
		getModel().setSamples(samples);
	}

}
