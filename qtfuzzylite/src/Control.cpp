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


#include <QMessageBox>
#include <QMenu>
#include <QSignalMapper>
#include <QAction>

#include "fl/qt/Model.h"
namespace fl {
    namespace qt {

        Control::Control(QWidget* parent, Qt::WindowFlags f) :
        Viewer(parent, f), _outputIndex(0), _viewOutput(false),
        _isTakagiSugeno(false), _min(fl::nan), _max(fl::nan) { }

        Control::~Control() {
            //Exception on reset?
            //            if (dynamic_cast<InputVariable*> (variable)) {
            //                QObject::disconnect(this, SIGNAL(valueChanged(double)),
            //                        this, SLOT(updateInput(double)));
            //            }
        }

        void Control::setup(const fl::Variable* model) {
            Viewer::setup(model);
            this->variable = const_cast<fl::Variable*> (model);

            if (dynamic_cast<fl::OutputVariable*> (variable)) {
                ui->sld_x->setEnabled(false);
                ui->sbx_x->setVisible(false);
                ui->led_x->setVisible(true);
                ui->lbl_fuzzy_out->setVisible(true);
                ui->lbl_fuzzy->setVisible(false);
                ui->btn_properties->setVisible(true);

                _isTakagiSugeno = false;
                for (int i = 0; i < variable->numberOfTerms(); ++i) {
                    if (variable->getTerm(i)->className() == Constant().className() or
                            variable->getTerm(i)->className() == Linear().className()) {
                        _isTakagiSugeno = true;
                        break;
                    }
                }

                if (_isTakagiSugeno) {
                    fl::OutputVariable* outputVariable = dynamic_cast<fl::OutputVariable*> (variable);
                    _outputs = std::vector<scalar>(
                            outputVariable->getDefuzzifier()->getDivisions(),
                            (outputVariable->getMaximum() - outputVariable->getMinimum()) / 2.0);
                    _outputIndex = 0;
                    onActionGraph("view output");
                }

            } else if (dynamic_cast<InputVariable*> (variable)) {
                QObject::connect(this, SIGNAL(valueChanged(double)),
                        this, SLOT(updateInput(double)));
            }
            _min = variable->getMinimum();
            _max = variable->getMaximum();
        }

        void Control::updateInput(double value) {
            fl::InputVariable* inputVariable = dynamic_cast<fl::InputVariable*> (variable);
            if (inputVariable) {
                inputVariable->setInput(value);
            }
        }

        void Control::onChangeSliderValue(int position) {
            //            Viewer::onChangeSliderValue(position);
            //TODO: Check this crazy thing.
            if (not _viewOutput) {
                Viewer::onChangeSliderValue(position);
            } else {
                scalar value = fl::Op::scale(position,
                        ui->sld_x->minimum(), ui->sld_x->maximum(),
                        _min, _max);
                double tolerance = 1.0 / std::pow(10.0, ui->sbx_x->decimals());
                if (not fl::Op::isEq(value, ui->sbx_x->value(), tolerance)) {
                    ui->sbx_x->setValue(value);
                }
            }
        }

        void Control::updateOutput() {
            fl::OutputVariable* outputVariable = dynamic_cast<fl::OutputVariable*> (variable);
            if (not outputVariable) {
                throw fl::Exception("[cast error] trying to cast OutputVariable", FL_AT);
            }

            scalar y = outputVariable->defuzzify();

            if (_viewOutput) {
                _outputs.at(_outputIndex) = y;
                _outputIndex = (_outputIndex + 1) % _outputs.size();
                if (y > _max) _max = y;
                if (y < _min) _min = y;
            }
            ui->led_x->setText(QString::number(y, 'f', fl::fuzzylite::decimals()));
            ui->sbx_x->setValue(y);

            QString fuzzify = QString::fromStdString(outputVariable->fuzzify(y));
            ui->lbl_fuzzy_out->setText("&#956;=" + fuzzify);

            refresh();
            if (not outputVariable->output()->isEmpty())
                draw(outputVariable->output());

        }

        void Control::onClickGraph() {
            QMenu menu(this);
            std::vector<QAction*> actions;
            actions.push_back(new QAction("show/hide", this));

            if (_isTakagiSugeno) {
                actions.push_back(NULL);
                QAction* actionView = new QAction("view output", this);
                if (_viewOutput) {
                    actionView->setCheckable(true);
                    actionView->setChecked(true);
                }


                actions.push_back(actionView);

                QAction* actionClear = new QAction("clear", this);
                actionClear->setEnabled(_viewOutput);

                actions.push_back(actionClear);
            }
            QSignalMapper signalMapper(this);
            for (std::size_t i = 0; i < actions.size(); ++i) {
                if (actions.at(i)) {
                    signalMapper.setMapping(actions.at(i), actions.at(i)->text());
                    QObject::connect(actions.at(i), SIGNAL(triggered()),
                            &signalMapper, SLOT(map()));

                    menu.addAction(actions.at(i));
                } else {
                    menu.addSeparator();
                }
            }
            QObject::connect(&signalMapper, SIGNAL(mapped(const QString &)),
                    this, SLOT(onActionGraph(const QString &)));

            menu.exec(QCursor::pos() + QPoint(1, 0));
            for (std::size_t i = 0; i < actions.size(); ++i) {
                if (actions.at(i)) {
                    actions.at(i)->deleteLater();
                }
            }
            ui->btn_properties->setChecked(false);
        }

        void Control::onActionGraph(const QString& action) {
            if (action == "show/hide") {
                ui->mainWidget->setVisible(not ui->mainWidget->isVisible());
                if (ui->mainWidget->isVisible()) {
                    setSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::Minimum);
                } else {
                    setSizePolicy(QSizePolicy::Minimum,
                            QSizePolicy::Minimum);
                }
                adjustSize();
            } else if (action == "view output") {
                _viewOutput = not _viewOutput;
                if (_viewOutput) {
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
            } else if (action == "clear") {
                fl::OutputVariable* outputVariable =
                        dynamic_cast<fl::OutputVariable*> (variable);
                _outputs = std::vector<scalar>(
                        outputVariable->getDefuzzifier()->getDivisions(),
                        (outputVariable->getMaximum() - outputVariable->getMinimum()) / 2.0);
                _outputIndex = 0;
                _min = outputVariable->getMinimum();
                _max = outputVariable->getMaximum();
            }


            refresh();
        }

        void Control::refresh() {
            if (not _viewOutput) {
                Viewer::refresh();
                return;
            }
            draw();
        }

        void Control::draw() {
            if (not _viewOutput) {
                Viewer::draw();
                return;
            }

            fl::OutputVariable* outputVariable =
                    dynamic_cast<fl::OutputVariable*> (variable);

            //TODO: Handle change of resolution
            if (false and _outputs.size() != outputVariable->getDefuzzifier()->getDivisions()) {
                std::vector<scalar> replace = std::vector<scalar>(
                        outputVariable->getDefuzzifier()->getDivisions(),
                        (outputVariable->getMaximum() - outputVariable->getMinimum()) / 2.0);

                for (std::size_t i = 0; i < std::min(replace.size(), _outputs.size()); ++i) {
                    replace.at(i) =
                            _outputs.at((_outputIndex - i) % _outputs.size());
                }
                _outputs = replace;
                _outputIndex = replace.size() - 1;
            }

            //            scalar mean = (outputVariable->getMaximum() - outputVariable->getMinimum()) / 2.0;
            //            scalar dx = std::max(std::fabs(max - mean), std::fabs(min - mean));
            //            min = mean - dx;
            //            max = mean + dx;

//            scalar mean = (outputVariable->getMaximum() - outputVariable->getMinimum()) / 2.0;
//            scalar dx = std::max(std::fabs(max - mean), std::fabs(min - mean));
//            min = mean - dx;
//            max = mean + dx;
            
            scalar bound = std::max(std::fabs(_min), std::fabs(_max));

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
                y0ui = fl::Op::scale(y0, -bound, bound,
                        rect.bottom(), rect.top());

                path.moveTo(x0ui, y0ui);

                x1ui = fl::Op::scale(i + 1, 0, _outputs.size(),
                        rect.left(), rect.right());
                y1ui = fl::Op::scale(y1, -bound, bound,
                        rect.bottom(), rect.top());

                path.lineTo(x1ui, y1ui);
            }

            QPen pen;
            pen.setColor(QColor(0, 210, 0, 200));
            pen.setStyle(Qt::SolidLine);
            pen.setCapStyle(Qt::RoundCap);
            pen.setJoinStyle(Qt::MiterJoin);
            pen.setWidth(5);

            ui->canvas->scene()->addPath(path, pen);
        }

        void Control::draw(const fl::Term* term, const QColor& color) {
            if (not _viewOutput) {
                Viewer::draw(term, color);
                return;
            }
        }

    }
}
