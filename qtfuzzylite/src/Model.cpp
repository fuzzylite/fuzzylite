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
        }

        void Model::reset() {
            fl::Engine e = new fl::Engine("qtfuzzylite");
            e->addRuleBlock(new fl::RuleBlock);
            change(e);
        }
    }
}
