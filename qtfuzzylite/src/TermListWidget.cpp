/* 
 * File:   TermListWidget.cpp
 * Author: jcrada
 *
 * Created on 20 April 2013, 11:33 AM
 */

#include "fl/qt/TermListWidget.h"
#include "fl/qt/Model.h"
#include "fl/qt/Viewer.h"

#include <fl/Headers.h>

#include <QWidgetAction>
#include <QMenu>

#include <vector>

namespace fl {
    namespace qt {

        TermListWidget::TermListWidget(QWidget* parent)
        : QListWidget(parent) { }

        void TermListWidget::contextMenuEvent(QContextMenuEvent* event) {
            std::vector<fl::Variable*> selectedVariables;
            for (int i = 0; i < selectedIndexes().size(); ++i) {
                if (_variableType == "input") {
                    selectedVariables.push_back(
                            Model::Default()->engine()->getInputVariable(
                            selectedIndexes().at(i).row()));
                } else if (_variableType == "output") {
                    selectedVariables.push_back(
                            Model::Default()->engine()->getOutputVariable(
                            selectedIndexes().at(i).row()));
                } else {
                    throw fl::Exception("[internal error] unrecognized variable type", FL_AT);
                }
            }

            if (selectedVariables.size() == 0) return;

            QMenu menu(this);
            for (std::size_t i = 0; i < selectedVariables.size(); ++i) {
                Viewer* viewer = new Viewer;
                viewer->setup(selectedVariables.at(i));
                viewer->ui->mainLayout->setContentsMargins(3,0,3,0);
                QWidgetAction* action = new QWidgetAction(&menu);
                action->setDefaultWidget(viewer);
                
                menu.addAction(action);
            }
            menu.exec(QCursor::pos() + QPoint(1, 0));
        }

        void TermListWidget::setVariableType(const std::string& variableType) {
            this->_variableType = variableType;
        }

        std::string TermListWidget::getVariableType() const {
            return this->_variableType;
        }
    }
}
