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

namespace fl {
    namespace qt {

        class Variable: public QDialog {
        Q_OBJECT

        protected slots:
            void onClickAddTerm();
            void onClickRemoveTerm();
            void onClickEditTerm();

            void onClickMoveUp();
            void onClickMoveDown();
            
            void onDoubleClickTerm(QListWidgetItem* item);

            void onSelectTerm();
            void onClickWizard();

            void accept();
            void reject();

        protected:
            void connect();
            void disconnect();

        public:
            Ui::Variable* ui;
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

    } /* namespace qt */
} /* namespace fl */
#endif /* FLQT_VARIABLE_H_ */
