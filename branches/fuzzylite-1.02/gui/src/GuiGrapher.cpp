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
#include "gui/GuiGrapher.h"

#include <math.h>
#include <QtGui/QMenu>
#include <QtGui/QToolTip>
#include <QtGui/qevent.h>

namespace fl_gui {

    GuiGrapher::GuiGrapher(fl::LinguisticVariable& lvar, QWidget* parent,
            Qt::WindowFlags f, bool enable_lvar, bool enable_term) :
    QWidget(parent, f), _lvar(&lvar), _ui(new Ui::Grapher),
    _lvar_enabled(enable_lvar), _term_enabled(enable_term), _is_input(
    dynamic_cast<fl::InputLVar*> (&lvar)), _is_output(
    dynamic_cast<fl::OutputLVar*> (&lvar)) {

    }

    GuiGrapher::~GuiGrapher() {
        disconnect();
        delete _ui;
    }

    Ui::Grapher& GuiGrapher::getUi() const {
        return *this->_ui;
    }

    void GuiGrapher::setup() {
        getUi().setupUi(this);
        getUi().grx_term->setVisible(false);
        getUi().grx_lvar->setVisible(false);
        getUi().hsl_value->setEnabled(isInputLVar());
        getUi().hsl_value->setVisible(isInputLVar());
        getUi().led_x->setReadOnly(isOutputLVar());
        getUi().lbl_y->setVisible(isInputLVar());
        getUi().led_y->setVisible(isInputLVar());
        connect();
    }

    void GuiGrapher::connect() {
        QObject::connect(getUi().hsl_value, SIGNAL(valueChanged(int)), this,
                SLOT(onChangeSliderValue(int)));
        QObject::connect(getUi().led_x, SIGNAL(editingFinished()), this,
                SLOT(onEditInputValue()));
        QObject::connect(this, SIGNAL(onChangeInputValue()), this, SLOT(updateUi()));

    }

    void GuiGrapher::disconnect() {
        QObject::disconnect(getUi().hsl_value, SIGNAL(valueChanged(int)), this,
                SLOT(onChangeSliderValue(int)));
        QObject::disconnect(getUi().led_x, SIGNAL(editingFinished()), this,
                SLOT(onEditInputValue()));
        QObject::disconnect(this, SIGNAL(onChangeInputValue()), this, SLOT(updateUi()));
    }

    bool GuiGrapher::isInputLVar() const {
        return _is_input;
    }

    bool GuiGrapher::isOutputLVar() const {
        return _is_output;
    }

    void GuiGrapher::setLVarEnabled(bool enable) {
        this->_lvar_enabled = enable;
    }

    bool GuiGrapher::isLVarEnabled() const {
        return this->_lvar_enabled;
    }

    void GuiGrapher::setTermEnabled(bool enable) {
        this->_term_enabled = enable;
    }

    bool GuiGrapher::isTermEnabled() const {
        return this->_term_enabled;
    }

    fl::LinguisticVariable& GuiGrapher::getLVar() const {
        return *this->_lvar;
    }

    void GuiGrapher::contextMenuEvent(QContextMenuEvent* e) {
        QMenu menu;
        menu.setTitle("Information");
        if (isTermEnabled()) {
            bool shown = getUi().grx_term->isVisible();
            menu.addAction(QString((shown ? "Hide" : "Show")) + " discrete term",
                    getUi().grx_term, (shown ? SLOT(hide()) : SLOT(show())));
        }
        if (isLVarEnabled()) {
            bool shown = getUi().grx_lvar->isVisible();
            menu.addAction(QString((shown ? "Hide" : "Show")) + " linguistic variable",
                    getUi().grx_lvar, (shown ? SLOT(hide()) : SLOT(show())));
        }
        if (isTermEnabled() || isLVarEnabled()) {
            menu.exec(e->globalPos());
            updateUi();
        }
    }

    void GuiGrapher::onEditInputValue() {
        if (getLVar().isEmpty()) {
            return;
        }
        bool* ok = new bool;
        fl::flScalar x = getUi().led_x->text().toDouble(ok);
        if (ok) {
            int slider_position = fl::FuzzyOperation::Scale(_lvar->minimum(),
                    _lvar->maximum(), x, getUi().hsl_value->minimum(),
                    getUi().hsl_value->maximum());
            getUi().hsl_value->setValue(slider_position);
        }
        delete ok;
    }

    void GuiGrapher::onChangeSliderValue(int slider_position) {
        fl::flScalar x = 0;
        fl::InputLVar* input = dynamic_cast<fl::InputLVar*> (&getLVar());
        x = fl::FuzzyOperation::Scale(getUi().hsl_value->minimum(),
                getUi().hsl_value->maximum(), slider_position, input->minimum(),
                input->maximum());
        input->setInput(x);
        emit(onChangeInputValue());
    }

    void GuiGrapher::focusInEvent(QFocusEvent* e) {
        (void) e;
        getUi().hsl_value->setFocus();
    }

    void GuiGrapher::resizeEvent(QResizeEvent* e) {
        QWidget::resizeEvent(e);
        updateUi();
    }

    void GuiGrapher::updateUi() {
        if (getLVar().isEmpty()) {
            return;
        }
        getUi().gvw_canvas->setMinimumLVar(getLVar().minimum());
        getUi().gvw_canvas->setMaximumLVar(getLVar().maximum());

        getUi().lbl_name->setText(QString::fromStdString(getLVar().name()));

        fl::flScalar x = NAN;
        fl::InputLVar* input = dynamic_cast<fl::InputLVar*> (&getLVar());
        if (input) {
            x = input->input();
        }
        fl::OutputLVar* output = dynamic_cast<fl::OutputLVar*> (&getLVar());
        if (output) {
            x = output->output().defuzzify();
        }

        getUi().led_x->setText(QString::number(x, 'f', 3));

        fl::LinguisticTerm* term = getLVar().bestFuzzyApproximation(x);

        bool tsk_model =isOutputLVar() && fl::FuzzyOperator::DefaultFuzzyOperator().defuzzifier().name() ==
                fl::TakagiSugenoDefuzzifier().name();
        if (tsk_model) {
            term = getLVar().compound().clone(); //TODO: Fix Memory Leak
        }
        if (!term) {
            FL_LOG("no fuzzy approximation found");
            return;
        }

        fl::flScalar y = term->membership(x);
        getUi().led_y->setText(QString::number(y, 'f', 3));
        getUi().lbl_fuzzy->setText(QString::fromStdString(getLVar().fuzzify(x)));

        getUi().grx_term->setTitle(QString::fromStdString(term->name()));
        getUi().led_term_range_min->setText(QString::number(term->minimum()));
        getUi().led_term_range_max->setText(QString::number(term->maximum()));

        fl::flScalar area, centroid_x, centroid_y;
        area = term->areaAndCentroid(centroid_x, centroid_y);
        getUi().led_x_term_centroid->setText(QString::number(centroid_x, 'f', 3));
        getUi().led_y_term_centroid->setText(QString::number(centroid_y, 'f', 3));

        getUi().led_term_area->setText(QString::number(area));
        getUi().gvw_canvas->clear();
        getUi().gvw_canvas->drawVariable(getLVar());
        //LVAR
        if (isInputLVar()) {
            getUi().grx_lvar->setTitle(QString::fromStdString(input->name()));
            fl::CompoundTerm compound = input->compound();
            getUi().led_lvar_range_min->setText(QString::number(compound.minimum()));
            getUi().led_lvar_range_max->setText(QString::number(compound.maximum()));

            area = compound.areaAndCentroid(centroid_x, centroid_y);
            getUi().led_x_lvar_centroid->setText(QString::number(centroid_x, 'f', 3));
            getUi().led_y_lvar_centroid->setText(QString::number(centroid_y, 'f', 3));

            getUi().led_lvar_area->setText(QString::number(area, 'f', 3));
            getUi().gvw_canvas->drawGuide(x, y);

        } else if (isOutputLVar()) {
            QColor color = QColor(Qt::green).darker(125);
            color.setAlpha(230);
            getUi().gvw_canvas->drawTerm(output->output(), color);
            std::vector<fl::flScalar> samples_x, samples_y;
            output->output().samples(samples_x, samples_y,
                    output->fuzzyOperator().numberOfSamples());

            getUi().grx_lvar->setTitle(QString::fromStdString(output->output().name()));
            const fl::CompoundTerm& result = output->output();
            getUi().led_lvar_range_min->setText(QString::number(result.minimum()));
            getUi().led_lvar_range_max->setText(QString::number(result.maximum()));

            area = result.areaAndCentroid(centroid_x, centroid_y);
            getUi().led_x_lvar_centroid->setText(QString::number(centroid_x, 'f', 3));
            getUi().led_y_lvar_centroid->setText(QString::number(centroid_y, 'f', 3));

            getUi().led_lvar_area->setText(QString::number(area, 'f', 3));
        }

    }

    void GuiGrapher::mouseDoubleClickEvent(QMouseEvent* e) {
        (void) e;
        if (isOutputLVar()) {
            fl::OutputLVar* output = dynamic_cast<fl::OutputLVar*> (&getLVar());
            FL_LOG(output->output().toString());
            FL_LOG("defussi: " << output->output().defuzzify());

        }
    }

}
