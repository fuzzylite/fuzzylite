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
 * Model.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_MODEL_H_
#define FLQT_MODEL_H_

#include <fl/Headers.h>

#include <QObject>

namespace fl {
    namespace qt {

        class Model : public QObject {
            Q_OBJECT
        public:
            static Model* Default();

        signals:
            void engineChanged();

        protected:
            Model();
            virtual ~Model();
            static Model* singleton;
            fl::Engine* _engine;

        public:
            virtual fl::Engine* engine() const;
            virtual void change(Engine* engine);
            virtual void reset();
        };


    }
}
#endif /* FLQT_MODEL_H_ */
