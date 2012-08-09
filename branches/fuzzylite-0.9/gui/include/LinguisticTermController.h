/*
 * LinguisticTermController.h
 *
 *  Created on: Dec 13, 2009
 *      Author: jcrada
 */

#ifndef _LINGUISTICTERMCONTROLLER_H_
#define _LINGUISTICTERMCONTROLLER_H_

#include <QtCore/QObject>

#include "GuiLinguisticTerm.h"
#include <fuzzylite/FuzzyLite.h>

namespace fl_gui {

	class LinguisticTermController : public QObject{
	private:
		fl::LinguisticTerm* _model;
		GuiLinguisticTerm* _view;

	protected:
		fl::LinguisticTerm& model() const;
		GuiLinguisticTerm& view() const;
	public:
		LinguisticTermController(fl::LinguisticTerm& model, GuiLinguisticTerm& view);
		virtual ~LinguisticTermController();

	};


}  // namespace fl_gui
#endif /* _LINGUISTICTERMCONTROLLER_H_ */
