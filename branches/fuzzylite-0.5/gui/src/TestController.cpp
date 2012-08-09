#include "TestController.h"
#include "GuiGrapher.h"

using namespace fuzzy_lite;
namespace fuzzy_lite_gui {

    TestController::TestController(FuzzyEngine& model, GuiMainWindow& view)
    : _model(&model), _view(&view) {
        connect();
    }

    TestController::~TestController() {
        disconnect();
    }

    FuzzyEngine& TestController::getModel() const {
        return *this->_model;
    }

    GuiMainWindow& TestController::getView() const {
        return *this->_view;
    }

    void TestController::connect() {
        QObject::connect(getView().getUi().tlx_main, SIGNAL(currentChanged(int)),
                this, SLOT(onPageChange(int)));

    }

    void TestController::disconnect() {
        QObject::disconnect(getView().getUi().tlx_main, SIGNAL(currentChanged(int)),
                this, SLOT(onPageChange(int)));
    }

    void TestController::onPageChange(int page) {
        if (page == 0) {
            return;
        }
        //Inputs
        QVBoxLayout* layout = dynamic_cast<QVBoxLayout*> (getView().getUi().grx_test_inputs->layout());
        for (int i = 0; i < layout->count(); ++i) {
            QLayoutItem* item = layout->itemAt(i);
            GuiGrapher* g = dynamic_cast<GuiGrapher*> (item->widget());
            if (g) {
                layout->removeItem(item);
                QObject::disconnect(g, SIGNAL(onChangeInputValue()),
                        this, SLOT(onInputValueChanged()));
                delete item->widget();
                delete item;
            }
        }
        for (int i = 0; i < getModel().numberOfInputLVars(); ++i) {
            GuiGrapher* g = new GuiGrapher(getModel().getInputLVar(i));
            g->setup();
            layout->insertWidget(i, g);
            QObject::connect(g, SIGNAL(onChangeInputValue()),
                    this, SLOT(onInputValueChanged()));
        }

        //Rules
        getView().getUi().lsw_test_rules->clear();
        getView().getUi().lsw_test_rules_activation->clear();
        for (int i = 0; i < getModel().numberOfRules(); ++i) {
            QString rule = QString::fromStdString(getModel().getRule(i).toString());
            getView().getUi().lsw_test_rules->addItem(rule);
            QListWidgetItem* item = new QListWidgetItem;
            item->setText("-");
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            getView().getUi().lsw_test_rules_activation->addItem(item);
        }
        for (int i = 0; i < getModel().numberOfRules(); ++i) {
            QListWidgetItem* rule = getView().getUi().lsw_test_rules->item(i);
            QListWidgetItem* act = getView().getUi().lsw_test_rules_activation->item(i);
            QRect rect = getView().getUi().lsw_test_rules->visualItemRect(rule);
            act->setSizeHint(rect.size());
        }

        //Outputs
        layout = dynamic_cast<QVBoxLayout*> (getView().getUi().grx_test_outputs->layout());
        for (int i = 0; i < layout->count(); ++i) {
            QLayoutItem* item = layout->itemAt(i);
            GuiGrapher* g = dynamic_cast<GuiGrapher*> (item->widget());
            if (g) {
                layout->removeItem(item);
                QObject::disconnect(this, SIGNAL(forceUpdate()),
                        g, SLOT(updateUi()));
                delete item->widget();
                delete item;
            }
        }
        for (int i = 0; i < getModel().numberOfOutputLVars(); ++i) {
            GuiGrapher* g = new GuiGrapher(getModel().getOutputLVar(i));
            g->setup();
            layout->insertWidget(i, g);
            QObject::connect(this, SIGNAL(forceUpdate()),
                    g, SLOT(updateUi()));
        }
        getView().update();
    }

    void TestController::onInputValueChanged() {
//        FL_LOG("-----------------------");
        QColor from_color(Qt::white);
        QColor to_color(0, 200, 0);
        for (int i = 0; i < getModel().numberOfOutputLVars(); ++i) {
            getModel().getOutputLVar(i).clearOutput();
        }
        FuzzyOperator& op = getModel().getFuzzyOperator();
        for (int i = 0; i < getModel().numberOfRules(); ++i) {
            FuzzyRule& rule = getModel().getRule(i);
            flScalar degree = rule.evaluate(op);

            int red = FuzzyOperator::scaleParma(0, 1, degree,
                    from_color.red(), to_color.red());
            int green = FuzzyOperator::scaleParma(0, 1, degree,
                    from_color.green(), to_color.green());
            int blue = FuzzyOperator::scaleParma(0, 1, degree,
                    from_color.blue(), to_color.blue());
            QColor color = QColor(red, green, blue);

            getView().getUi().lsw_test_rules->item(i)->setBackground(QBrush(color));
            getView().getUi().lsw_test_rules_activation->item(i)->setBackground(QBrush(color));
            getView().getUi().lsw_test_rules_activation->item(i)->setText(QString::number(degree,'f',3));
            rule.fire(degree, op);
        }
        emit forceUpdate();
    }
}
