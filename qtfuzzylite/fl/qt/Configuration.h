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

        class Configuration : public QDialog {
            Q_OBJECT

        protected slots:
            virtual void onChangeTNorm(int);
            virtual void onChangeSNorm(int);
            virtual void onChangeActivation(int);
            virtual void onChangeAccumulation(int);
            virtual void onChangeDefuzzifier(int);
            virtual void onChangeDivisions(int);
            
            virtual void onChangeHedgeSelection(int);

        protected:

            virtual void connect();
            virtual void disconnect();
            
            virtual int indexOfTnorm(const std::string& tnorm);
            virtual int indexOfSnorm(const std::string& snorm);
            virtual int indexOfDefuzzifier(const std::string& defuzzifier);
            
            virtual void showEvent(QShowEvent*);
            
        public:
            Ui::Preferences* ui;

            Configuration(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Configuration();

            virtual void setup();

            virtual void applyDefaults();
            virtual void loadFromModel();

            static void main();
        };
    }
}
#endif /* FLQT_CONFIGURATION_H_ */
