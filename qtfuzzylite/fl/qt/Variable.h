/*
 * Variable.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FLQT_VARIABLE_H_
#define FLQT_VARIABLE_H_

#include <QtGui/QDialog>

#include "ui/ui_Variable.h"

#include <fl/Headers.h>

namespace fl {
    namespace qt {
        class Viewer;
        class Variable: public QDialog {
        Q_OBJECT

        protected slots:
            void onClickAddTerm();
            void onClickRemoveTerm();
            void onClickEditTerm();

            void onClickMoveUp();
            void onClickMoveDown();
            
            void onChangeMinRange(double);
            void onChangeMaxRange(double);
            
            void onClickTerm(QListWidgetItem* item);
            void onDoubleClickTerm(QListWidgetItem* item);

            void onSelectTerm();
            
            void redraw();
            void showSelectedTerms();

            void accept();
            void reject();

        protected:
            QListWidgetItem* _previouslySelected;
            void connect();
            void disconnect();

        public:
            Ui::Variable* ui;
            Viewer* viewer;
            fl::Variable* variable;

            enum VariableType {
                INPUT_VARIABLE, OUTPUT_VARIABLE
            };

            Variable(QWidget* parent = NULL, Qt::WindowFlags f = 0);
            virtual ~Variable();

            virtual void showEvent(QShowEvent* event);

            virtual void setup(VariableType type);

            virtual void edit(const InputVariable* variable);
            virtual void edit(const OutputVariable* variable);

            virtual void reloadModel();

        };

    } 
} 
#endif /* FLQT_VARIABLE_H_ */
