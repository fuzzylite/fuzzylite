#include <math.h>
#include <QtGui/QMenu>
#include <QtGui/QToolTip>
#include <QtGui/qevent.h>
#include "GuiGrapher.h"


namespace fuzzy_lite_gui {

    GuiGrapher::GuiGrapher(LinguisticVariable& lvar, QWidget* parent, Qt::WindowFlags f,
            bool enable_lvar, bool enable_term)
    : QWidget(parent, f), _lvar(&lvar), _ui(new Ui::Grapher), _scene(new QGraphicsScene),
    _lvar_enabled(enable_lvar), _term_enabled(enable_term),
    _is_input(dynamic_cast<InputLVar*> (&lvar)), _is_output(dynamic_cast<OutputLVar*> (&lvar)) {

    }

    GuiGrapher::~GuiGrapher() {
        disconnect();
        delete _ui;
        delete _scene;
    }

    Ui::Grapher& GuiGrapher::getUi() const {
        return *this->_ui;
    }

    QGraphicsScene& GuiGrapher::getScene() const {
        return *this->_scene;
    }

    void GuiGrapher::setup() {
        getUi().setupUi(this);
        getUi().gvw_canvas->setScene(&getScene());
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
        QObject::connect(getUi().hsl_value, SIGNAL(valueChanged(int)),
                this, SLOT(onChangeSliderValue(int)));
        QObject::connect(getUi().led_x, SIGNAL(editingFinished()),
                this, SLOT(onEditInputValue()));
        QObject::connect(this, SIGNAL(onChangeInputValue()),
                this, SLOT(updateUi()));

    }

    void GuiGrapher::disconnect() {
        QObject::disconnect(getUi().hsl_value, SIGNAL(valueChanged(int)),
                this, SLOT(onChangeSliderValue(int)));
        QObject::disconnect(getUi().led_x, SIGNAL(editingFinished()),
                this, SLOT(onEditInputValue()));
        QObject::disconnect(this, SIGNAL(onChangeInputValue()),
                this, SLOT(updateUi()));
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

    LinguisticVariable& GuiGrapher::getLVar() const {
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
        flScalar x = getUi().led_x->text().toDouble(ok);
        if (ok) {
            int slider_position = FuzzyOperator::scaleParma(_lvar->minimum(), _lvar->maximum(), x,
                    getUi().hsl_value->minimum(), getUi().hsl_value->maximum());
            getUi().hsl_value->setValue(slider_position);
        }
        delete ok;
    }

    void GuiGrapher::onChangeSliderValue(int slider_position) {
        flScalar x = 0;

        if (getLVar().firstTerm()->isFromInfinity() && slider_position == getUi().hsl_value->minimum()) {
            x = -INFINITY;
        }
        if (getLVar().lastTerm()->isToInfinity() && slider_position == getUi().hsl_value->maximum()) {
            x = INFINITY;
        }

        if (!isinf(x)) {
            x = FuzzyOperator::scaleParma(
                    getUi().hsl_value->minimum() + (getLVar().firstTerm()->isFromInfinity() ? 1 : 0),
                    getUi().hsl_value->maximum() - (getLVar().lastTerm()->isToInfinity() ? 1 : 0),
                    slider_position, getLVar().minimum(), getLVar().maximum());
        }
        InputLVar* input = dynamic_cast<InputLVar*> (&getLVar());
        input->setInput(x);
        emit(onChangeInputValue());
    }

    void GuiGrapher::focusInEvent(QFocusEvent* e) {
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

        getUi().lbl_name->setText(QString::fromStdString(getLVar().getName()));

        flScalar x = NAN;
        InputLVar* input = dynamic_cast<InputLVar*> (&getLVar());
        if (input) {
            x = input->getInput();
        }
        OutputLVar* output = dynamic_cast<OutputLVar*> (&getLVar());
        if (output) {
            x = output->getDefuzzifiedOutput(FuzzyOperator::DefaultFuzzyOperator());
        }

        getUi().led_x->setText(QString::number(x, 'f', 3));

        DiscreteTerm* term = _lvar->bestFuzzyApproximation(x);
        if (!term) {
            FL_LOG("no fuzzy approximation found");
            return;
        }

        flScalar y = term->membership(x);
        getUi().led_y->setText(QString::number(y, 'f', 3));
        getUi().lbl_fuzzy->setText(QString::fromStdString(_lvar->fuzzify(x)));

        getUi().grx_term->setTitle(QString::fromStdString(term->getName()));
        getUi().led_term_range_min->setText(QString::number(term->minimum()));
        getUi().led_term_range_max->setText(QString::number(term->maximum()));

        flScalar centroid_x, centroid_y;
        term->centroid(centroid_x, centroid_y);
        getUi().led_x_term_centroid->setText(QString::number(centroid_x, 'f', 3));
        getUi().led_y_term_centroid->setText(QString::number(centroid_y, 'f', 3));

        getUi().led_term_area->setText(QString::number(term->area()));

        //LVAR
        if (isInputLVar()) {
            getUi().grx_lvar->setTitle(QString::fromStdString(input->getName()));
            DiscreteTerm compound = input->compound();
            getUi().led_lvar_range_min->setText(QString::number(compound.minimum()));
            getUi().led_lvar_range_max->setText(QString::number(compound.maximum()));

            compound.centroid(centroid_x, centroid_y);
            getUi().led_x_lvar_centroid->setText(QString::number(centroid_x, 'f', 3));
            getUi().led_y_lvar_centroid->setText(QString::number(centroid_y, 'f', 3));

            getUi().led_lvar_area->setText(QString::number(compound.area(), 'f', 3));
        } else if (isOutputLVar()) {
            getUi().grx_lvar->setTitle(QString::fromStdString(output->getOutput().getName()));
            const DiscreteTerm& result = output->getOutput();
            getUi().led_lvar_range_min->setText(QString::number(result.minimum()));
            getUi().led_lvar_range_max->setText(QString::number(result.maximum()));

            result.centroid(centroid_x, centroid_y);
            getUi().led_x_lvar_centroid->setText(QString::number(centroid_x, 'f', 3));
            getUi().led_y_lvar_centroid->setText(QString::number(centroid_y, 'f', 3));

            getUi().led_lvar_area->setText(QString::number(result.area(), 'f', 3));
        }

        //Drawing

        flScalar min_lvar = getLVar().minimum();
        flScalar max_lvar = getLVar().maximum();
        if (isnan(min_lvar) || isnan(max_lvar)) {
            FL_LOG("minimum or maximum is NaN")
            return;
        }

        QRect rect = getUi().gvw_canvas->viewport()->rect();
        rect.translate(-rect.width() / 2, -rect.height() / 2);
        int min_x_canvas = rect.left() + 15 / 2, max_x_canvas = rect.right() - 15 / 2, //15 is size of slider ball in macosx
                min_y_canvas = 0 + rect.height() * 1 / 2, max_y_canvas = 0 - rect.height() * 1 / 2;
        getScene().setSceneRect(rect);
        getScene().clear();
        QPen pen;
        pen.setWidth(2);
        for (int i = 0; i < getLVar().numberOfTerms(); ++i) {
            pen.setColor(QColor(Qt::blue));
            DiscreteTerm& term = getLVar().getTerm(i);
            for (int j = 0; j < term.numberOfCoords() - 1; ++j) {
                flScalar x1 = FuzzyOperator::scaleParma(min_lvar, max_lvar,
                        term.getCoordX(j), min_x_canvas, max_x_canvas);
                flScalar y1 = FuzzyOperator::scaleParma(0, 1,
                        term.getCoordY(j), min_y_canvas, max_y_canvas);

                flScalar x2 = FuzzyOperator::scaleParma(min_lvar, max_lvar,
                        term.getCoordX(j + 1), min_x_canvas, max_x_canvas);
                flScalar y2 = FuzzyOperator::scaleParma(0, 1,
                        term.getCoordY(j + 1), min_y_canvas, max_y_canvas);
                getScene().addLine(x1, y1, x2, y2, pen);
            }
            term.centroid(centroid_x, centroid_y);
            centroid_x = FuzzyOperator::scaleParma(min_lvar, max_lvar, centroid_x,
                    min_x_canvas, max_x_canvas);
            centroid_y = FuzzyOperator::scaleParma(0, 1, centroid_y,
                    min_y_canvas, max_y_canvas);
            pen.setColor(QColor(Qt::black));
            getScene().addEllipse(centroid_x, centroid_y, 1, 1, pen);
        }

        if (isInputLVar()) {
            pen.setColor(QColor(Qt::red));
            pen.setStyle(Qt::DotLine);
            flScalar guide_x = FuzzyOperator::scaleParma(min_lvar, max_lvar,
                    x, min_x_canvas, max_x_canvas);
            flScalar guide_y = FuzzyOperator::scaleParma(0, 1,
                    y, min_y_canvas, max_y_canvas);
            getScene().addLine(guide_x, min_y_canvas, guide_x, guide_y, pen);
        }


        if (output) {
            QPolygon polygon;
            for (int i = 0; i < output->getOutput().numberOfCoords(); ++i) {
                flScalar px = FuzzyOperator::scaleParma(output->minimum(),
                        output->maximum(), output->getOutput().getCoordX(i),
                        min_x_canvas, max_x_canvas);
                flScalar py = FuzzyOperator::scaleParma(0, 1,
                        output->getOutput().getCoordY(i), min_y_canvas, max_y_canvas);
                polygon.append(QPoint(px, py));
                QPen pen(QColor(Qt::red));
                pen.setWidth(2);
                getScene().addEllipse(px, py, 1, 1, pen);

                flScalar inter_x, inter_y;
                output->getOutput().intersectionOnAggregate(inter_x, inter_y);
                if (!isnan(inter_x) && !isnan(inter_y)) {
                    inter_x = FuzzyOperator::scaleParma(output->minimum(),
                            output->maximum(), inter_x,
                            min_x_canvas, max_x_canvas);
                    inter_y = FuzzyOperator::scaleParma(0, 1, inter_y,
                            min_y_canvas, max_y_canvas);
                    pen.setColor(QColor(Qt::black));
                    pen.setWidth(4);
                    getScene().addEllipse(inter_x, inter_y, 1, 1, pen);
                }
            }
            QColor fill(QColor(Qt::blue).lighter());
            fill.setAlpha(128);
            QPen pen(fill);
            QBrush brush(fill);
            brush.setStyle(Qt::SolidPattern);
            getScene().addPolygon(polygon, pen, brush);

            output->getOutput().centroid(centroid_x, centroid_y);
            centroid_x = FuzzyOperator::scaleParma(min_lvar, max_lvar, centroid_x,
                    min_x_canvas, max_x_canvas);
            centroid_y = FuzzyOperator::scaleParma(0, 1, centroid_y,
                    min_y_canvas, max_y_canvas);
            pen.setColor(QColor(Qt::black));
            getScene().addEllipse(centroid_x, centroid_y, 1, 1, pen);
            //            FL_LOG(output->getOutput().toString());
        }
    }

    void GuiGrapher::mouseDoubleClickEvent(QMouseEvent* e) {
        if (isOutputLVar()) {
            OutputLVar* output = dynamic_cast<OutputLVar*> (&getLVar());
            FL_LOG(output->getOutput().toString());
            FL_LOG("defussi: " << output->getDefuzzifiedOutput(FuzzyOperator::DefaultFuzzyOperator()));
        }
    }



}
