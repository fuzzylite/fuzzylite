/*
 * Model.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_MODEL_H_
#define FLQT_MODEL_H_

#include <fl/Headers.h>

namespace fl {
    namespace qt {
        class Model {
        public:
            static Model* Default();

        protected:
            static Model* singleton;
            fl::Engine* _engine;

        public:
            Model();
            virtual ~Model();

            virtual fl::Engine* engine() const;
            virtual void change(Engine* engine);
            virtual void reset();

        };
    }
} 
#endif /* FLQT_MODEL_H_ */
