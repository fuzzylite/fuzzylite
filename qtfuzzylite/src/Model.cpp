/**
    This file is part of qtfuzzylite.

    qtfuzzylite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qtfuzzylite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qtfuzzylite.  If not, see <http://www.gnu.org/licenses/>.

    Juan Rada-Vilela, 01 February 2013
    jcrada@fuzzylite.com
 **/

/*
 * Model.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Model.h"

namespace fl {
    namespace qt {

        Model* Model::singleton = NULL;

        Model* Model::Default() {
            if (not singleton) {
                singleton = new Model;
                singleton->reset();
            }
            return singleton;
        }

        Model::Model() : _engine(NULL) { }

        Model::~Model() {
            if (_engine) delete _engine;
        }

        fl::Engine* Model::engine() const {
            return this->_engine;
        }

        void Model::change(Engine* engine) {
            if (_engine) delete _engine;
            _engine = engine;
            if (_engine->numberOfRuleBlocks() == 0)
                _engine->addRuleBlock(new fl::RuleBlock);
            emit engineChanged();
        }

        void Model::reset() {
            fl::Engine* e = new fl::Engine("");
            fl::RuleBlock* rb = new fl::RuleBlock;

//            rb->setConjunction(new Minimum);
//            rb->setDisjunction(new Maximum);
//            rb->setActivation(new Minimum);

            e->addRuleBlock(rb);
            change(e);
        }
    }
}
