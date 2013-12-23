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
 * Control.cpp
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Control.h"

#include "fl/qt/qtfuzzylite.h"

#include <QMessageBox>
#include <QMenu>
#include <QSignalMapper>
#include <QAction>
#include <QInputDialog>
#include <QSettings>

#include "fl/qt/Model.h"
namespace fl {
    namespace qt {

        Control::Control(QWidget* parent, Qt::WindowFlags f) :
        Viewer(parent, f), _outputIndex(0), _outputView(false),
        _isTakagiSugeno(false), _minOutput(fl::nan), _maxOutput(fl::nan),
        _allowsOutputView(false) {
        }

        Control::~Control() {
        }

        void Control::setup(const fl::Variable* model) {
            Viewer::setup(model);
            this->variable = const_cast<fl::Variable*> (model);

            _isTakagiSugeno = false;
            //            for (int i = 0; i < variable->numberOfTerms(); ++i) {
            //                if (variable->getTerm(i)->className() == Constant().className() or
            //                        variable->getTerm(i)->className() == Linear().className()) {
            //                    _isTakagiSugeno = true;
            //                    break;
            //                }
            //            }
            ui->btn_name->setEnabled(true);
            QObject::connect(ui->btn_name, SIGNAL(clicked()),
                    this, SLOT(onClickVariableName()));
            ui->sld_x->setEnabled(variable->isEnabled());
            ui->sbx_x->setEnabled(variable->isEnabled());
            ui->led_x->setEnabled(variable->isEnabled());
            if (dynamic_cast<fl::OutputVariable*> (variable)) {
                ui->sld_x->setEnabled(false);
                ui->sbx_x->setVisible(false);
                ui->led_x->setVisible(true);
                ui->lbl_fuzzy_out->setVisible(true);
                ui->lbl_fuzzy->setVisible(false);
                ui->btn_name->setVisible(true);
                ui->btn_name->setIcon(QIcon(":/output.png"));

                _minOutput = variable->getMinimum();
                _maxOutput = variable->getMaximum();

                OutputVariable* outputVariable = dynamic_cast<fl::OutputVariable*> (variable);
                fl::Defuzzifier* defuzzifier = outputVariable->getDefuzzifier();
                _isTakagiSugeno = defuzzifier and
                        (defuzzifier->className() == WeightedAverage().className() or
                        defuzzifier->className() == WeightedSum().className());

            } else if (dynamic_cast<InputVariable*> (variable)) {
                QObject::connect(this, SIGNAL(valueChanged(double)),
                        this, SLOT(updateInput(double)));
                ui->btn_name->setIcon(QIcon(":/input.png"));
                if (_isTakagiSugeno or variable->terms().empty()) {
                    minimizeViewer();
                }
            }
        }

        void Control::setAllowOutputView(bool allow) {
            _allowsOutputView = allow;
            if (_allowsOutputView) {
                fl::OutputVariable* outputVariable = dynamic_cast<fl::OutputVariable*> (variable);
                if (outputVariable) {
                    QSettings settings;
                    int defaultOutputViewResolution =
                            settings.value("view/defaultOutputViewResolution", 250).toInt();
                    _outputs = std::vector<scalar>(
                            defaultOutputViewResolution,
                            (outputVariable->getMaximum() + outputVariable->getMinimum()) / 2.0);
                    _outputIndex = 0;
                    _minOutput = variable->getMinimum();
                    _maxOutput = variable->getMaximum();
                    if (_isTakagiSugeno) swapOutputView();
                }
            }
        }

        bool Control::allowsOutputView() const {
            return this->_allowsOutputView;
        }

        void Control::updateInput(double value) {
            fl::InputVariable* inputVariable = dynamic_cast<fl::InputVariable*> (variable);
            if (inputVariable) {
                inputVariable->setInputValue(value);
            }
        }

        void Control::onChangeSliderValue(int position) {
            if (not _outputView) {
                Viewer::onChangeSliderValue(position);
            } else {
            }
        }

        void Control::onEditInputValue() {
            if (not _outputView) {
                Viewer::onEditInputValue();
            } else {

                scalar min = fl::Op::min(_minOutput, variable->getMinimum());
                scalar max = fl::Op::max(_maxOutput, variable->getMaximum());

                scalar value = ui->sbx_x->value();
                scalar sliderValue = fl::Op::scale(ui->sld_x->value(),
                        ui->sld_x->minimum(), ui->sld_x->maximum(),
                        min, max);

                double tolerance = 1.0 / std::pow(10.0, ui->sbx_x->decimals());
                if (not fl::Op::isEq(value, sliderValue, tolerance)) {
                    int position = (int) fl::Op::scale(value,
                            min, max,
                            ui->sld_x->minimum(), ui->sld_x->maximum());
                    if (position != ui->sld_x->value()) {
                        ui->sld_x->setValue(position);
                    }
                }

                emit valueChanged(value);
            }
        }

        void Control::updateOutput() {
            fl::OutputVariable* outputVariable = dynamic_cast<fl::OutputVariable*> (variable);
            if (not outputVariable) {
                throw fl::Exception("[cast error] trying to cast OutputVariable", FL_AT);
            }

            scalar y = outputVariable->defuzzify();

            if (_outputView) {
                _outputs.at(_outputIndex) = y;
                _outputIndex = (_outputIndex + 1) % _outputs.size();
                if (not (fl::Op::isNan(y) or fl::Op::isInf(y))) {
                    if (y > _maxOutput) _maxOutput = y;
                    if (y < _minOutput) _minOutput = y;
                }
            }
            ui->led_x->setText(QString::fromStdString(fl::Op::str(y)));
            ui->sbx_x->setValue(y);

            QString fuzzify = QString::fromStdString(outputVariable->fuzzify(y));
            ui->lbl_fuzzy_out->setText("&#956;=" + fuzzify);

            refresh();
            if (not outputVariable->fuzzyOutput()->isEmpty())
                draw(outputVariable->fuzzyOutput());

        }

        void Control::onClickVariableName() {
            //TODO: Add option to edit variable.
            QMenu menu(this);
            if (not ui->wdg_canvas->isVisible()) {
                menu.addAction("maximize", this, SLOT(onActionVariableName()));
            } else {
                menu.addAction("minimize", this, SLOT(onActionVariableName()));

                if (allowsOutputView()) {
                    menu.addSeparator();

                    if (not _isTakagiSugeno) {
                        QAction* actionView = new QAction("output view", &menu);
                        QObject::connect(actionView, SIGNAL(triggered()),
                                this, SLOT(onActionVariableName()));
                        if (_outputView) {
                            actionView->setCheckable(true);
                            actionView->setChecked(true);
                        }
                        menu.addAction(actionView);
                    }

                    if (_outputView) {
                        menu.addAction("resolution...", this, SLOT(onActionVariableName()));
                        menu.addSeparator();
                        menu.addAction("clear", this, SLOT(onActionVariableName()));
                    }
                }
            }

            menu.exec(QCursor::pos() + QPoint(1, 0));

            ui->btn_name->setChecked(false);
        }

        void Control::onActionVariableName() {
            QAction* action = qobject_cast<QAction*>(sender());
            if (not action) return;

            if (action->text() == "maximize") {
                maximizeViewer();
            } else if (action->text() == "minimize") {
                minimizeViewer();
            } else if (action->text() == "output view") {
                swapOutputView();
            } else if (action->text() == "resolution...") {
                QSettings settings;
                int minOutputViewResolution =
                        settings.value("view/minOutputViewResolution", 50).toInt();
                int maxOutputViewResolution =
                        settings.value("view/maxOutputViewResolution", 1000).toInt();
                bool ok;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
                int resolution = QInputDialog::getInteger(this,
                        "Resolution of Output View",
                        "How many defuzzified values do you want to show?",
                        _outputs.size(), minOutputViewResolution, maxOutputViewResolution,
                        10, &ok);
#else
                int resolution = QInputDialog::getInt(this,
                        "Resolution of Output View",
                        "How many defuzzified values do you want to show?",
                        _outputs.size(), minOutputViewResolution, maxOutputViewResolution,
                        10, &ok);
#endif


                if (ok) { //clear
                    fl::OutputVariable* outputVariable =
                            dynamic_cast<fl::OutputVariable*> (variable);
                    _outputs = std::vector<scalar>(
                            resolution,
                            (outputVariable->getMaximum() + outputVariable->getMinimum()) / 2.0);
                    _outputIndex = 0;
                    _minOutput = outputVariable->getMinimum();
                    _maxOutput = outputVariable->getMaximum();
                    ui->sbx_x->setValue((_maxOutput + _minOutput) / 2.0);
                }
            } else if (action->text() == "clear") {
                fl::OutputVariable* outputVariable =
                        dynamic_cast<fl::OutputVariable*> (variable);
                _outputs = std::vector<scalar>(
                        _outputs.size(),
                        (outputVariable->getMaximum() + outputVariable->getMinimum()) / 2.0);
                _outputIndex = 0;
                _minOutput = outputVariable->getMinimum();
                _maxOutput = outputVariable->getMaximum();
                ui->sbx_x->setValue((_maxOutput + _minOutput) / 2.0);
            }
            if (not (action->text() == "clear" or action->text() == "resolution...")) {
                if (parentWidget()) parentWidget()->adjustSize();
                adjustSize();
            }

            if (action->text() == "maximize") emit signalRefresh();
            else refresh();
        }

        void Control::minimizeViewer() {
            //            if (isMinimizedViewer()) return;
            ui->wdg_canvas->setVisible(false);
            ui->wdg_out->setVisible(false);

            if (_outputView) {
                ui->canvas->setBackgroundBrush(QBrush(Qt::white));
                ui->lyt_canvas->removeWidget(ui->sld_x);
                ui->sld_x->setOrientation(Qt::Horizontal);
                ui->mainLayout->addWidget(ui->sld_x);
                setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                setFixedHeight(minimumSizeHint().height() + ui->sld_x->sizeHint().height());
            } else {
                setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
                setFixedHeight(minimumSizeHint().height());
            }

        }

        void Control::maximizeViewer() {
            //            if (isMaximizedViewer()) return;
            ui->wdg_canvas->setVisible(true);
            ui->wdg_out->setVisible(true);

            if (_outputView) {
                ui->mainLayout->removeWidget(ui->sld_x);
                ui->sld_x->setOrientation(Qt::Vertical);
                ui->lyt_canvas->addWidget(ui->sld_x);
                ui->canvas->setBackgroundBrush(QBrush(Qt::black));
            }
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            setMinimumSize(0, 0);
            setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        }

        void Control::swapOutputView() {
            _outputView = not _outputView;
            if (_outputView) {
                ui->mainLayout->removeWidget(ui->sld_x);
                ui->sld_x->setOrientation(Qt::Vertical);
                ui->lyt_canvas->addWidget(ui->sld_x);
                ui->canvas->setBackgroundBrush(QBrush(Qt::black));
            } else {
                ui->canvas->setBackgroundBrush(QBrush(Qt::white));
                ui->lyt_canvas->removeWidget(ui->sld_x);
                ui->sld_x->setOrientation(Qt::Horizontal);
                ui->mainLayout->addWidget(ui->sld_x);
            }
        }

        void Control::refresh() {
            if (not _outputView) {
                Viewer::refresh();
                return;
            }
            draw();
        }

        void Control::draw() {
            if (not _outputView) {
                Viewer::draw();
                return;
            }

            fl::OutputVariable* outputVariable =
                    dynamic_cast<fl::OutputVariable*> (variable);

            scalar min = fl::Op::min(_minOutput, outputVariable->getMinimum());
            scalar max = fl::Op::max(_maxOutput, outputVariable->getMaximum());

            ui->lbl_min->setText(QString::fromStdString(fl::Op::str(min)));
            ui->lbl_max->setText(QString::fromStdString(fl::Op::str(max)));

            ui->canvas->scene()->clear();
            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());
            QRect rect = ui->canvas->viewport()->rect();

            QPainterPath path;
            scalar x0ui, y0ui, x1ui, y1ui;
            for (std::size_t i = 0; i + 1 < _outputs.size(); ++i) {
                int ix0 = (_outputIndex + i) % _outputs.size();
                scalar y0 = _outputs.at(ix0);

                int ix1 = (_outputIndex + i + 1) % _outputs.size();
                scalar y1 = _outputs.at(ix1);

                x0ui = fl::Op::scale(i, 0, _outputs.size(),
                        rect.left(), rect.right());
                y0ui = fl::Op::scale(y0, min, max,
                        rect.bottom(), rect.top());

                x1ui = fl::Op::scale(i + 1, 0, _outputs.size(),
                        rect.left(), rect.right());
                y1ui = fl::Op::scale(y1, min, max,
                        rect.bottom(), rect.top());

                if (fl::Op::isNan(y0ui) or fl::Op::isInf(y0ui)) {
                    path.moveTo(x1ui, y1ui);
                } else {
                    path.moveTo(x0ui, y0ui);
                }

                path.lineTo(x1ui, y1ui);
            }

            QPen pen;
            pen.setColor(QColor(0, 210, 0, 200));
            pen.setStyle(Qt::SolidLine);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::MiterJoin);
            QSettings settings;
            int penWidthOutputView = settings.value("view/penWidthOutputView", 3).toInt();
            pen.setWidth(penWidthOutputView);

            ui->canvas->scene()->addPath(path, pen);
        }

        void Control::draw(const fl::Term* term, const QColor& color) {
            if (not _outputView) {
                Viewer::draw(term, color);
                return;
            }
        }

    }
}
