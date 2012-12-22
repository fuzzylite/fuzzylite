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
                singleton->_engine->addRuleBlock(new RuleBlock);
            }
            return singleton;
        }

        Model::Model()
                : _engine(new fl::Engine("qtfuzzylite")),
                  _configuration(new fl::Configuration) {
        }

        Model::~Model() {
            delete _engine;
            delete _configuration;
        }

        void Model::changeEngine(Engine* engine) {
            delete _engine;
            _engine = engine;
            _engine->configure(_configuration);
        }

        fl::Engine* Model::engine() const {
            return this->_engine;
        }

        fl::Configuration* Model::configuration() const {
            return this->_configuration;
        }

        void Model::update() {
            this->_engine->configure(this->_configuration, false);
        }
    }
} 
