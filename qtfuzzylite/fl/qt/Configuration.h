/*
 * Configuration.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_CONFIGURATION_H_
#define FLQT_CONFIGURATION_H_

#include <QtGui/QDialog>
#include <QtGui/QWidget>

#include "ui/ui_Configuration.h"

#include <fl/Headers.h>

#include <vector>


namespace fl {
    namespace qt {

        class Configuration: public QDialog {
        Q_OBJECT

        protected slots:
            virtual void onChangeTNorm(int index);
            virtual void onChangeSNorm(int index);
            virtual void onChangeActivation(int index);
            virtual void onChangeAccumulation(int index);
            virtual void onChangeDefuzzifier(int index);
            virtual void onChangeDivisions(int value);


        protected:
            std::vector<std::pair<std::string, TNorm*> > _andOperators;
            std::vector<std::pair<std::string, SNorm*> > _orOperators;
            std::vector<std::pair<std::string, Defuzzifier*> > _defuzzifiers;



            virtual void connect();
            virtual void disconnect();

        public:
            Ui::Configuration* ui;

            Configuration(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Configuration();

            virtual void setup();

            virtual void refresh();

            static void main();
        };
    } 
} 
#endif /* FLQT_CONFIGURATION_H_ */
