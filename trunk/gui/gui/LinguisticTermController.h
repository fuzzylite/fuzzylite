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
/*
 * LinguisticTermController.h
 *
 *  Created on: Dec 13, 2009
 *      Author: jcrada
 */

#ifndef _LINGUISTICTERMCONTROLLER_H_
#define _LINGUISTICTERMCONTROLLER_H_

#include <QtCore/QObject>

#include "gui/GuiLinguisticTerm.h"
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
