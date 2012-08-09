/*   Copyright 2010 Juan Rada-Vilela

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
#include "gui/TestController.h"
#include "gui/GuiGrapher.h"

namespace fl_gui {

    TestController::TestController(fl::FuzzyEngine& model, GuiMainWindow& view) :
    _model(&model), _view(&view) {
        connect();
    }

    TestController::~TestController() {
        disconnect();
    }

    fl::FuzzyEngine& TestController::getModel() const {
        return *this->_model;
    }

    GuiMainWindow& TestController::getView() const {
        return *this->_view;
    }

    void TestController::connect() {
        QObject::connect(getView().getUi().btn_ok, SIGNAL(clicked()), this,
                SLOT(load()));


    }

    void TestController::disconnect() {
        QObject::disconnect(getView().getUi().btn_ok, SIGNAL(clicked()),
                this, SLOT(load()));
    }

    void TestController::refreshModel() {
        unload();
        load();
    }

    void TestController::load() {
        unload();
        //Inputs
        QLayout* layout = getView().getUi().grx_test_inputs->layout();
        for (int i = 0; i < getModel().numberOfInputLVars(); ++i) {
            GuiGrapher* g = new GuiGrapher(*getModel().inputLVar(i));
            g->setup();
            //            layout->insertWidget(i, g);
            layout->addWidget(g);
            QObject::connect(g, SIGNAL(onChangeInputValue()), this,
                    SLOT(onInputValueChanged()));
        }
        layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Ignored, QSizePolicy::Expanding));

        //Rules
        for (int i = 0; i < getModel().ruleBlock(0)->numberOfRules(); ++i) {
            QString rule = QString::fromStdString(getModel().ruleBlock(0)->rule(i)->toString());
            getView().getUi().lsw_test_rules->addItem(rule);
            QListWidgetItem* item = new QListWidgetItem;
            item->setText("-");
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            getView().getUi().lsw_test_rules_activation->addItem(item);
        }
        for (int i = 0; i < getModel().ruleBlock(0)->numberOfRules(); ++i) {
            QListWidgetItem* rule = getView().getUi().lsw_test_rules->item(i);
            QListWidgetItem* act = getView().getUi().lsw_test_rules_activation->item(i);
            QRect rect = getView().getUi().lsw_test_rules->visualItemRect(rule);
            act->setSizeHint(rect.size());
        }

        layout = getView().getUi().grx_test_outputs->layout();
        //Outputs
        for (int i = 0; i < getModel().numberOfOutputLVars(); ++i) {
            GuiGrapher* g = new GuiGrapher(*getModel().outputLVar(i));
            g->setup();
            layout->addWidget(g);
            QObject::connect(this, SIGNAL(forceUpdate()), g, SLOT(updateUi()));
        }
        layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Ignored, QSizePolicy::Expanding));
        
        getView().getUi().tab_container->setCurrentIndex(1);
    }

    void TestController::unload() {
        //Inputs
        QLayout* layout = getView().getUi().grx_test_inputs->layout();

        for (int i = layout->count() - 1; i >= 0; --i) {
            QLayoutItem* item = layout->itemAt(i);
            GuiGrapher* g = dynamic_cast<GuiGrapher*> (item->widget());
            if (g) {
                QObject::disconnect(g, SIGNAL(onChangeInputValue()), this,
                        SLOT(onInputValueChanged()));
            }
            layout->removeItem(item);
            delete item->widget();
            delete item;
        }

        //Rules
        getView().getUi().lsw_test_rules->clear();
        getView().getUi().lsw_test_rules_activation->clear();

        //Outputs
        layout = getView().getUi().grx_test_outputs->layout();
        for (int i = layout->count() - 1; i >= 0; --i) {
            QLayoutItem* item = layout->itemAt(i);
            GuiGrapher* g = dynamic_cast<GuiGrapher*> (item->widget());
            if (g) {
                QObject::disconnect(this, SIGNAL(forceUpdate()), g, SLOT(updateUi()));
            }
            layout->removeItem(item);
            delete item->widget();
            delete item;
        }
    }

    void TestController::onInputValueChanged() {
        //        FL_LOG("-----------------------");
        QColor from_color(Qt::white);
        QColor to_color(0, 200, 0);
        for (int i = 0; i < getModel().numberOfOutputLVars(); ++i) {
            getModel().outputLVar(i)->output().clear();
        }
        for (int i = 0; i < getModel().ruleBlock(0)->numberOfRules(); ++i) {
            fl::FuzzyRule& rule = *getModel().ruleBlock(0)->rule(i);
            fl::flScalar degree = rule.firingStrength();

            int red, green, blue, alpha;
            GuiCanvas::ColorGradient(from_color.red(), from_color.green(),
                    from_color.blue(), from_color.alpha(), to_color.red(), to_color.green(),
                    to_color.blue(), to_color.alpha(), (int) (degree * 255), red, green, blue, alpha);

            QColor color = QColor(red, green, blue, alpha);

            getView().getUi().lsw_test_rules->item(i)->setBackground(QBrush(color));
            getView().getUi().lsw_test_rules_activation->item(i)->setBackground(QBrush(
                    color));
            getView().getUi().lsw_test_rules_activation->item(i)->setText(
                    QString::number(degree, 'f', 3));
            rule.fire(degree);
        }
        emit forceUpdate();
    }
}
