/*
 * LinguisticTermController.cpp
 *
 *  Created on: Dec 14, 2009
 *      Author: jcrada
 */

#include "LinguisticTermController.h"
namespace fl_gui {

	LinguisticTermController::LinguisticTermController(fl::LinguisticTerm& model, GuiLinguisticTerm& view) :
	_model(&model), _view(&view){

	}

	LinguisticTermController::~LinguisticTermController(){

	}

	fl::LinguisticTerm& LinguisticTermController::model() const{
		return *this->_model;
	}

	GuiLinguisticTerm& LinguisticTermController::view() const{
		return *this->_view;
	}


}  // namespace fl_gui
