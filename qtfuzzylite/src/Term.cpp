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
 * Term.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Term.h"

#include "fl/qt/Viewer.h"
#include "fl/qt/Window.h"
#include "fl/qt/Wizard.h"
#include "fl/qt/Model.h"

#include "fl/qt/qtfuzzylite.h"

#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSignalMapper>
#include <QCoreApplication>

namespace fl {
    namespace qt {

        Term::Term(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f),
        ui(new Ui::Term), viewer(NULL), dummyVariable(new Variable),
        indexOfEditingTerm(-1) {
            setWindowFlags(Qt::Dialog
                    | Qt::WindowSystemMenuHint
                    | Qt::WindowCloseButtonHint
                    | Qt::WindowStaysOnTopHint);
        }

        Term::~Term() {
            delete ui;
            for (std::size_t i = 0; i < _basicTerms.size(); ++i) {
                delete _basicTerms.at(i);
            }
            for (std::size_t i = 0; i < _extendedTerms.size(); ++i) {
                delete _extendedTerms.at(i);
            }
            for (std::size_t i = 0; i < _edgeTerms.size(); ++i) {
                delete _edgeTerms.at(i);
            }
            for (std::size_t i = 0; i < _fxTerms.size(); ++i) {
                delete _fxTerms.at(i);
            }
            for (int i = dummyVariable->numberOfTerms() - 1; i >= 0; --i) {
                delete dummyVariable->removeTerm(i);
            }
            delete dummyVariable;

        }

        void Term::onEngineChanged() {
            close();
            //
            //            fl::Term* current = selectedTerm();
            //            Linear* linear;
            //            for (std::size_t i = 0; i < _fxTerms.size(); ++i) {
            //                linear = dynamic_cast<Linear*> (_fxTerms.at(i));
            //                if (linear) {
            //                    Engine* engine = Model::Default()->engine();
            //                    linear->set(std::vector<scalar>(engine->numberOfInputVariables() + 1),
            //                            engine->inputVariables());
            //                    loadFrom(linear);
            //                    break;
            //                }
            //            }
            //
            //            if (current != linear) loadFrom(current);
        }

        void Term::loadTerms(scalar min, scalar max) {
            scalar average = (min + max) / 2;
            scalar diff = (max - min);
            //It MUST follow the order of the toolbox.
            _basicTerms.push_back(new Triangle("", min, average, max));
            _basicTerms.push_back(new Trapezoid("", min, min + .25 * diff,
                    min + .75 * diff, max));
            _basicTerms.push_back(new Rectangle("", min + .25 * diff, min + .75 * diff));

            std::vector<std::pair<scalar, scalar> > xy;
            xy.push_back(std::make_pair(min, 0));
            xy.push_back(std::make_pair(min + .25 * diff, 1));
            xy.push_back(std::make_pair(min + .5 * diff, .5));
            xy.push_back(std::make_pair(min + .75 * diff, 1));
            xy.push_back(std::make_pair(max, 0));
            _basicTerms.push_back(new Discrete("", xy));

            _extendedTerms.push_back(new Gaussian("", average, .2 * diff));
            _extendedTerms.push_back(new GaussianProduct("", average, .2 * diff, average, .2 * diff));
            _extendedTerms.push_back(new Bell("", average, .25 * diff, 3.0));
            _extendedTerms.push_back(new PiShape("", min, average, average, max));
            _extendedTerms.push_back(new SigmoidDifference("", min + .25 * diff, 20 / diff,
                    20 / diff, min + .75 * diff));
            _extendedTerms.push_back(new SigmoidProduct("", min + .25 * diff, 20 / diff,
                    -20 / diff, min + .75 * diff));

            _edgeTerms.push_back(new Ramp("", min, max));
            _edgeTerms.push_back(new Sigmoid("", average, 20 / diff));
            _edgeTerms.push_back(new SShape("", min, max));
            _edgeTerms.push_back(new ZShape("", min, max));

            _fxTerms.push_back(new Constant("", 0.0));

            const Engine* engine = Model::Default()->engine();
            _fxTerms.push_back(new Linear("",
                    std::vector<scalar>(engine->inputVariables().size() + 1),
                    engine->inputVariables()));
            _fxTerms.push_back(fl::Function::create("","",engine));
        }

        void Term::setup(const fl::Variable& variable, const fl::Term* edit) {
            loadTerms(variable.getMinimum(), variable.getMaximum());

            dummyVariable->setMinimum(variable.getMinimum());
            dummyVariable->setMaximum(variable.getMaximum());
            for (int i = 0; i < variable.numberOfTerms(); ++i) {
                dummyVariable->addTerm(variable.getTerm(i)->copy());
                if (variable.getTerm(i) == edit) {
                    indexOfEditingTerm = i;
                }
            }

            ui->setupUi(this);

            viewer = new Viewer;
            viewer->setup(dummyVariable);
            viewer->enablePropertiesButton(false);
            ui->toolboxLayout->insertWidget(0, viewer);
            //            viewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            //            ui->splitter->insertWidget(0, viewer);
            //            QList<int> sizes;
            //            sizes << .25 * size().width() << .75 * size().width();
            //            ui->splitter->setSizes(sizes);
            //            ui->splitter->setEnabled(false);

            //            QFont ttFont = Window::mainWindow()->typeWriterFont();
            //            ttFont.setPointSize(ttFont.pointSize() - 1);
            //            ui->ptx_discrete->setFont(ttFont);

            setWindowTitle(edit ? "Edit term" : "Add term");

            layout()->setSizeConstraint(QLayout::SetFixedSize);
            this->adjustSize();

            QRect scr = parentWidget()
                    ? parentWidget()->geometry() : Window::mainWindow()->geometry();
            move(scr.center().x() - rect().center().x(), scr.top());

            _sbx.clear();
            _sbx.push_back(ui->sbx_bell_center);
            _sbx.push_back(ui->sbx_bell_width);
            _sbx.push_back(ui->sbx_bell_slope);

            _sbx.push_back(ui->sbx_gaussian_center);
            _sbx.push_back(ui->sbx_gaussian_width);

            _sbx.push_back(ui->sbx_gaussian_prod_center_a);
            _sbx.push_back(ui->sbx_gaussian_prod_center_b);
            _sbx.push_back(ui->sbx_gaussian_prod_width_a);
            _sbx.push_back(ui->sbx_gaussian_prod_width_b);

            _sbx.push_back(ui->sbx_pishape_bl);
            _sbx.push_back(ui->sbx_pishape_tl);
            _sbx.push_back(ui->sbx_pishape_tr);
            _sbx.push_back(ui->sbx_pishape_br);

            _sbx.push_back(ui->sbx_ramp_start);
            _sbx.push_back(ui->sbx_ramp_end);

            _sbx.push_back(ui->sbx_rectangle_max);
            _sbx.push_back(ui->sbx_rectangle_min);

            _sbx.push_back(ui->sbx_sshape_start);
            _sbx.push_back(ui->sbx_sshape_end);

            _sbx.push_back(ui->sbx_sigmoid_inflection);
            _sbx.push_back(ui->sbx_sigmoid_slope);

            _sbx.push_back(ui->sbx_sigmoid_diff_falling);
            _sbx.push_back(ui->sbx_sigmoid_diff_left);
            _sbx.push_back(ui->sbx_sigmoid_diff_rising);
            _sbx.push_back(ui->sbx_sigmoid_diff_right);

            _sbx.push_back(ui->sbx_sigmoid_prod_falling);
            _sbx.push_back(ui->sbx_sigmoid_prod_left);
            _sbx.push_back(ui->sbx_sigmoid_prod_rising);
            _sbx.push_back(ui->sbx_sigmoid_prod_right);

            _sbx.push_back(ui->sbx_trapezoid_a);
            _sbx.push_back(ui->sbx_trapezoid_b);
            _sbx.push_back(ui->sbx_trapezoid_c);
            _sbx.push_back(ui->sbx_trapezoid_d);

            _sbx.push_back(ui->sbx_triangle_a);
            _sbx.push_back(ui->sbx_triangle_b);
            _sbx.push_back(ui->sbx_triangle_c);

            _sbx.push_back(ui->sbx_zshape_start);
            _sbx.push_back(ui->sbx_zshape_end);

            _sbx.push_back(ui->sbx_constant);

            for (std::size_t i = 0; i < _sbx.size(); ++i) {
                _sbx.at(i)->setMinimum(-10000000);
                _sbx.at(i)->setMaximum(10000000);
                _sbx.at(i)->setValue(0.0);
                _sbx.at(i)->setSingleStep(fl::Op::max(scalar(0.01),
                        (dummyVariable->getMaximum() - dummyVariable->getMinimum()) / 100));
                _sbx.at(i)->setAlignment(Qt::AlignHCenter);
                _sbx.at(i)->setAccelerated(true);
                _sbx.at(i)->setKeyboardTracking(false);
                _sbx.at(i)->setDecimals(fuzzylite::decimals());
            }

            for (std::size_t i = 0; i < _basicTerms.size(); ++i) {
                loadFrom(_basicTerms.at(i));
            }
            for (std::size_t i = 0; i < _extendedTerms.size(); ++i) {
                loadFrom(_extendedTerms.at(i));
            }
            for (std::size_t i = 0; i < _edgeTerms.size(); ++i) {
                loadFrom(_edgeTerms.at(i));
            }
            for (std::size_t i = 0; i < _fxTerms.size(); ++i) {
                loadFrom(_fxTerms.at(i));
            }

            ui->basicTermToolbox->setCurrentIndex(0);
            ui->extendedTermToolbox->setCurrentIndex(0);
            ui->edgeTermToolbox->setCurrentIndex(0);
            ui->fxTermToolbox->setCurrentIndex(0);
            if (edit) {
                loadFrom(edit);
                ui->led_name->setText(QString::fromStdString(edit->getName()));
                redraw();
            } else {
                dummyVariable->addTerm(_basicTerms.front()->copy()); //Add the triangle by default
                indexOfEditingTerm = dummyVariable->numberOfTerms() - 1;
                setCurrentToolbox(0);
            }
            connect();
        }

        void Term::accept() {
            dummyVariable->getTerm(indexOfEditingTerm)->setName(
                    fl::Op::makeValidId(ui->led_name->text().toStdString()));
            QDialog::accept();
        }

        void Term::connect() {
            QObject::connect(Model::Default(), SIGNAL(engineChanged()),
                    this, SLOT(onEngineChanged()));
            QObject::connect(Window::mainWindow(), SIGNAL(engineVariableChanged()),
                    this, SLOT(onEngineChanged()));
            QObject::connect(ui->basicTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);
            QObject::connect(ui->extendedTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);
            QObject::connect(ui->edgeTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);
            QObject::connect(ui->fxTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);

            QObject::connect(ui->tabTerms, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeTab(int)), Qt::QueuedConnection);

            QObject::connect(viewer, SIGNAL(valueChanged(double)),
                    this, SLOT(showSelectedTerm()));

            QObject::connect(ui->btn_wizard, SIGNAL(clicked()),
                    this, SLOT(onClickWizard()));

            //BASIC
            //Triangle
            QObject::connect(ui->sbx_triangle_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTriangle(double)));
            QObject::connect(ui->sbx_triangle_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTriangle(double)));
            QObject::connect(ui->sbx_triangle_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTriangle(double)));

            //Trapezoid
            QObject::connect(ui->sbx_trapezoid_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));
            QObject::connect(ui->sbx_trapezoid_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));
            QObject::connect(ui->sbx_trapezoid_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));
            QObject::connect(ui->sbx_trapezoid_d, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));

            //Rectangle
            QObject::connect(ui->sbx_rectangle_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRectangle(double)));
            QObject::connect(ui->sbx_rectangle_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRectangle(double)));

            //Discrete
            QObject::connect(ui->btn_discrete_parse, SIGNAL(clicked()),
                    this, SLOT(onClickDiscreteParser()));

            //EXTENDED
            //Gaussian
            QObject::connect(ui->sbx_gaussian_center, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::connect(ui->sbx_gaussian_width, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));

            //GaussianProduct
            QObject::connect(ui->sbx_gaussian_prod_center_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussianProduct(double)));
            QObject::connect(ui->sbx_gaussian_prod_center_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussianProduct(double)));
            QObject::connect(ui->sbx_gaussian_prod_width_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussianProduct(double)));
            QObject::connect(ui->sbx_gaussian_prod_width_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussianProduct(double)));
            //Bell
            QObject::connect(ui->sbx_bell_center, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_width, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_slope, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));

            //Pi-Shape
            QObject::connect(ui->sbx_pishape_bl, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::connect(ui->sbx_pishape_tl, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::connect(ui->sbx_pishape_tr, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::connect(ui->sbx_pishape_br, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));

            //SigmoidDiff
            QObject::connect(ui->sbx_sigmoid_diff_rising, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidDiff(double)));
            QObject::connect(ui->sbx_sigmoid_diff_left, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidDiff(double)));
            QObject::connect(ui->sbx_sigmoid_diff_falling, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidDiff(double)));
            QObject::connect(ui->sbx_sigmoid_diff_right, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidDiff(double)));

            //SigmoidProd
            QObject::connect(ui->sbx_sigmoid_prod_rising, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidProd(double)));
            QObject::connect(ui->sbx_sigmoid_prod_left, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidProd(double)));
            QObject::connect(ui->sbx_sigmoid_prod_falling, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidProd(double)));
            QObject::connect(ui->sbx_sigmoid_prod_right, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoidProd(double)));
            //EDGES
            //Ramp
            QObject::connect(ui->sbx_ramp_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRamp(double)));
            QObject::connect(ui->sbx_ramp_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRamp(double)));

            //Sigmoid
            QObject::connect(ui->sbx_sigmoid_inflection, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::connect(ui->sbx_sigmoid_slope, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));

            //S-Shape
            QObject::connect(ui->sbx_sshape_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSShape(double)));
            QObject::connect(ui->sbx_sshape_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSShape(double)));

            //Z-Shape
            QObject::connect(ui->sbx_zshape_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxZShape(double)));
            QObject::connect(ui->sbx_zshape_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxZShape(double)));

            //FX TERMS
            //Constant
            QObject::connect(ui->sbx_constant, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxConstant(double)));

            QObject::connect(ui->lst_coefficients, SIGNAL(itemChanged(QListWidgetItem*)),
                    this, SLOT(onChangeLinearCoefficient(QListWidgetItem*)));
            QObject::connect(ui->lst_variables->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lst_coefficients->verticalScrollBar(), SLOT(setValue(int)));
            QObject::connect(ui->lst_coefficients->verticalScrollBar(), SIGNAL(valueChanged(int)),
                    ui->lst_variables->verticalScrollBar(), SLOT(setValue(int)));

            QObject::connect(ui->btn_function_process, SIGNAL(clicked()),
                    this, SLOT(onClickFunctionProcess()));
            QObject::connect(ui->btn_function_variable, SIGNAL(clicked()),
                    this, SLOT(onClickFunctionVariable()));
            QObject::connect(ui->btn_function_builtin, SIGNAL(clicked()),
                    this, SLOT(onClickFunctionBuiltIn()));
        }

        void Term::showEvent(QShowEvent* event) {
            redraw();
            QWidget::showEvent(event);
        }

        void Term::resizeEvent(QResizeEvent* event) {
            redraw();
            QWidget::resizeEvent(event);
        }

        void Term::redraw() {
            viewer->refresh();
            showSelectedTerm();
            this->adjustSize();
        }

        void Term::showSelectedTerm() {
            viewer->draw(dummyVariable->getTerm(indexOfEditingTerm));
        }

        fl::Term* Term::selectedTerm() const {
            return dummyVariable->getTerm(indexOfEditingTerm);
        }

        void Term::setCurrentToolbox(int index) {
            ui->basicTermToolbox->setVisible(index == 0);
            ui->extendedTermToolbox->setVisible(index == 1);
            ui->edgeTermToolbox->setVisible(index == 2);
            ui->fxTermToolbox->setVisible(index == 3);
            ui->tabTerms->setCurrentIndex(index);
        }

        /**
         * Events
         */

        void Term::onClickWizard() {
            Wizard window(this);
            window.setup(ui->led_name->text().toStdString());
            window.ui->sbx_separation->setMinimum(-10000000);
            window.ui->sbx_separation->setMaximum(10000000);
            window.ui->sbx_separation->setSingleStep(fl::Op::max(scalar(0.01),
                    (dummyVariable->getMaximum() - dummyVariable->getMinimum()) / 100));

            if (not window.exec()) return;

            int copies = window.ui->sbx_copies->value();
            QStringList names = window.ui->ptx_terms->toPlainText().split(
                    QRegExp("\\s+"));
            while (names.size() < copies) {
                names.append("");
            }

            double distance = window.ui->sbx_separation->value();
            for (int i = 0; i < copies; ++i) {
                fl::Term* copy = dummyVariable->getTerm(indexOfEditingTerm)->copy();
                copy->setName(fl::Op::makeValidId(names.at(i).toStdString()));
                scalar separationDistance = distance * (i + 1);
                dummyVariable->insertTerm(copy, indexOfEditingTerm + i + 1);
                //BASIC
                if (copy->className() == Triangle().className()) {
                    Triangle* term = dynamic_cast<Triangle*> (copy);
                    term->setA(term->getA() + separationDistance);
                    term->setB(term->getB() + separationDistance);
                    term->setC(term->getC() + separationDistance);

                } else if (copy->className() == Trapezoid().className()) {
                    Trapezoid* term = dynamic_cast<Trapezoid*> (copy);
                    term->setA(term->getA() + separationDistance);
                    term->setB(term->getB() + separationDistance);
                    term->setC(term->getC() + separationDistance);
                    term->setD(term->getD() + separationDistance);

                } else if (copy->className() == Rectangle().className()) {
                    Rectangle* term = dynamic_cast<Rectangle*> (copy);
                    term->setStart(term->getStart() + separationDistance);
                    term->setEnd(term->getEnd() + separationDistance);

                } else if (copy->className() == Discrete().className()) {
                    Discrete* term = dynamic_cast<Discrete*> (copy);
                    for (std::size_t i = 0; i < term->x.size(); ++i) {
                        term->x.at(i) = term->x.at(i) + separationDistance;
                    }

                    //EXTENDED
                } else if (copy->className() == Gaussian().className()) {
                    Gaussian* term = dynamic_cast<Gaussian*> (copy);
                    term->setMean(term->getMean() + separationDistance);

                } else if (copy->className() == GaussianProduct().className()) {
                    GaussianProduct* term = dynamic_cast<GaussianProduct*> (copy);
                    term->setMeanA(term->getMeanA() + separationDistance);
                    term->setMeanB(term->getMeanB() + separationDistance);

                } else if (copy->className() == Bell().className()) {
                    Bell* term = dynamic_cast<Bell*> (copy);
                    term->setCenter(term->getCenter() + separationDistance);

                } else if (copy->className() == PiShape().className()) {
                    PiShape* term = dynamic_cast<PiShape*> (copy);
                    term->setBottomLeft(term->getBottomLeft() + separationDistance);
                    term->setTopLeft(term->getTopLeft() + separationDistance);
                    term->setTopRight(term->getTopRight() + separationDistance);
                    term->setBottomRight(term->getBottomRight() + separationDistance);

                } else if (copy->className() == SigmoidDifference().className()) {
                    SigmoidDifference* term = dynamic_cast<SigmoidDifference*> (copy);
                    term->setLeft(term->getLeft() + separationDistance);
                    term->setRight(term->getRight() + separationDistance);

                } else if (copy->className() == SigmoidProduct().className()) {
                    SigmoidProduct* term = dynamic_cast<SigmoidProduct*> (copy);
                    term->setLeft(term->getLeft() + separationDistance);
                    term->setRight(term->getRight() + separationDistance);

                    //EDGE
                } else if (copy->className() == Ramp().className()) {
                    Ramp* term = dynamic_cast<Ramp*> (copy);
                    term->setStart(term->getStart() + separationDistance);
                    term->setEnd(term->getEnd() + separationDistance);

                } else if (copy->className() == Sigmoid().className()) {
                    Sigmoid* term = dynamic_cast<Sigmoid*> (copy);
                    term->setInflection(term->getInflection() + separationDistance);

                } else if (copy->className() == SShape().className()) {
                    SShape* term = dynamic_cast<SShape*> (copy);
                    term->setStart(term->getStart() + separationDistance);
                    term->setEnd(term->getEnd() + separationDistance);

                } else if (copy->className() == ZShape().className()) {
                    ZShape* term = dynamic_cast<ZShape*> (copy);
                    term->setStart(term->getStart() + separationDistance);
                    term->setEnd(term->getEnd() + separationDistance);

                    //FX
                } else if (copy->className() == Constant().className()) {
                    Constant* term = dynamic_cast<Constant*> (copy);
                    term->setValue(term->getValue() + separationDistance);

                } else if (copy->className() == Linear().className()) {
                    Linear* term = dynamic_cast<Linear*> (copy);
                    if (not term->coefficients.empty())
                        term->coefficients.back() =
                            term->coefficients.back() + separationDistance;

                } else if (copy->className() == Function().className()) {
                    Function* term = dynamic_cast<Function*> (copy);
                    std::string infix = term->space(term->getFormula());
                    std::string replacement = " x ";
                    replacement += (separationDistance < 0 ? "- " : "+ ");
                    infix = fl::Op::findReplace(infix, " x ",
                            replacement + fl::Op::str(std::fabs(separationDistance)));
                    term->load(infix, term->getEngine());

                } else {
                    std::ostringstream ex;
                    ex << "[internal error] term of class <" <<
                            copy->className() << "> not implemented for copying";
                    throw fl::Exception(ex.str(), FL_AT);
                }
            }
            redraw();
        }

        void Term::onChangeToolBoxIndex(int index) {
            (void) index;
            delete dummyVariable->removeTerm(indexOfEditingTerm);
            switch (ui->tabTerms->currentIndex()) {
                case 0: dummyVariable->insertTerm(_basicTerms[ui->basicTermToolbox->currentIndex()]->copy(),
                            indexOfEditingTerm);
                    break;
                case 1:
                    dummyVariable->insertTerm(_extendedTerms[ui->extendedTermToolbox->currentIndex()]->copy(),
                            indexOfEditingTerm);
                    break;
                case 2:
                    dummyVariable->insertTerm(_edgeTerms[ui->edgeTermToolbox->currentIndex()]->copy(),
                            indexOfEditingTerm);
                    break;
                case 3:
                    dummyVariable->insertTerm(_fxTerms[ui->fxTermToolbox->currentIndex()]->copy(),
                            indexOfEditingTerm);
                    break;
                default:
                    throw fl::Exception("[internal error] index out of bounds", FL_AT);
            }
            std::string className = selectedTerm()->className();
            //BASIC
            if (className == Triangle().className()) onChangeSpinBoxTriangle(0);
            else if (className == Trapezoid().className()) onChangeSpinBoxTrapezoid(0);
            else if (className == Rectangle().className()) onChangeSpinBoxRectangle(0);
            else if (className == Discrete().className()) onClickDiscreteParser();
                //EXTENDED
            else if (className == Gaussian().className()) onChangeSpinBoxGaussian(0);
            else if (className == GaussianProduct().className()) onChangeSpinBoxGaussianProduct(0);
            else if (className == Bell().className()) onChangeSpinBoxBell(0);
            else if (className == PiShape().className()) onChangeSpinBoxPiShape(0);
            else if (className == SigmoidDifference().className()) onChangeSpinBoxSigmoidDiff(0);
            else if (className == SigmoidProduct().className()) onChangeSpinBoxSigmoidProd(0);
                //EDGES
            else if (className == Ramp().className()) onChangeSpinBoxRamp(0);
            else if (className == Sigmoid().className()) onChangeSpinBoxSigmoid(0);
            else if (className == SShape().className()) onChangeSpinBoxSShape(0);
            else if (className == ZShape().className()) onChangeSpinBoxZShape(0);
                //FX
            else if (className == Constant().className()) onChangeSpinBoxConstant(0);
            else if (className == Linear().className()) onChangeLinearCoefficient(NULL);
            else if (className == Function().className()) onClickFunctionProcess();

            else throw fl::Exception("[term error] term of class <" +
                    className + "> not recognized", FL_AT);


            redraw();
            //            this->adjustSize();
        }

        void Term::onChangeTab(int index) {
            setCurrentToolbox(index);
            onChangeToolBoxIndex(-1);
            redraw();
            //            this->adjustSize();
        }

        void Term::onChangeSpinBoxTriangle(double) {
            Triangle* term = dynamic_cast<Triangle*> (selectedTerm());
            term->setA(ui->sbx_triangle_a->value());
            term->setB(ui->sbx_triangle_b->value());
            term->setC(ui->sbx_triangle_c->value());

            redraw();
        }

        void Term::onChangeSpinBoxTrapezoid(double) {
            Trapezoid* term = dynamic_cast<Trapezoid*> (selectedTerm());
            term->setA(ui->sbx_trapezoid_a->value());
            term->setB(ui->sbx_trapezoid_b->value());
            term->setC(ui->sbx_trapezoid_c->value());
            term->setD(ui->sbx_trapezoid_d->value());
            redraw();
        }

        void Term::onChangeSpinBoxRectangle(double) {
            Rectangle* term = dynamic_cast<Rectangle*> (selectedTerm());
            term->setStart(ui->sbx_rectangle_min->value());
            term->setEnd(ui->sbx_rectangle_max->value());
            redraw();
        }

        void Term::onClickDiscreteParser() {
            std::string values = ui->ptx_discrete->toPlainText().toStdString();
            for (std::size_t i = 0; i < values.size(); ++i) {
                if (values.at(i) == '.' or (values.at(i) >= '0' and values.at(i) <= '9')) {
                    //do nothing
                } else {//ignore the rest
                    values.at(i) = ' ';
                }
            }

            std::vector<scalar> xValues, yValues;
            std::istringstream tokenizer(values);
            std::string token;
            bool next = true;
            while (tokenizer >> token) {
                try {
                    scalar value = fl::Op::toScalar(token);
                    if (next) xValues.push_back(value);
                    else {
                        if (fl::Op::isLt(value, 0.0) or fl::Op::isGt(value, 1.0)) {
                            QMessageBox::critical(this, tr("Error"),
                                    "[discrete term] y values must lie within [0.0, 1.0]",
                                    QMessageBox::Ok);
                            return;
                        }
                        yValues.push_back(value);
                    }
                    next = not next;
                } catch (...) {
                    QMessageBox::critical(this, tr("Error"),
                            "[discrete term] vectors x and y must contain numeric values",
                            QMessageBox::Ok);
                    return;
                }
            }

            if (xValues.size() != yValues.size()) {
                QString message = QString("[discrete term] the lengths of vectors x and y differ: ");
                message += QString("x[") + QString::number(xValues.size()) +
                        QString("] and y[") + QString::number(xValues.size()) + QString("]");
                QMessageBox::critical(this, tr("Error"),
                        message, QMessageBox::Ok);
                return;
            }

            Discrete* term = dynamic_cast<Discrete*> (selectedTerm());
            term->x = xValues;
            term->y = yValues;

            loadFrom(term);
            redraw();
            ui->ptx_discrete->setFocus();
            QTextCursor tc = ui->ptx_discrete->textCursor();
            tc.movePosition(QTextCursor::End);
            ui->ptx_discrete->setTextCursor(tc);
        }

        void Term::onChangeSpinBoxGaussian(double) {
            Gaussian* term = dynamic_cast<Gaussian*> (selectedTerm());
            term->setMean(ui->sbx_gaussian_center->value());
            term->setStandardDeviation(ui->sbx_gaussian_width->value());
            redraw();
        }

        void Term::onChangeSpinBoxGaussianProduct(double) {
            GaussianProduct* term = dynamic_cast<GaussianProduct*> (selectedTerm());
            term->setMeanA(ui->sbx_gaussian_prod_center_a->value());
            term->setMeanB(ui->sbx_gaussian_prod_center_b->value());
            term->setStandardDeviationA(ui->sbx_gaussian_prod_width_a->value());
            term->setStandardDeviationB(ui->sbx_gaussian_prod_width_b->value());
            redraw();
        }

        void Term::onChangeSpinBoxBell(double) {
            Bell* term = dynamic_cast<Bell*> (selectedTerm());
            term->setCenter(ui->sbx_bell_center->value());
            term->setWidth(ui->sbx_bell_width->value());
            term->setSlope(ui->sbx_bell_slope->value());
            redraw();
        }

        void Term::onChangeSpinBoxPiShape(double) {
            PiShape* term = dynamic_cast<PiShape*> (selectedTerm());
            term->setBottomLeft(ui->sbx_pishape_bl->value());
            term->setTopLeft(ui->sbx_pishape_tl->value());
            term->setTopRight(ui->sbx_pishape_tr->value());
            term->setBottomRight(ui->sbx_pishape_br->value());
            redraw();
        }

        void Term::onChangeSpinBoxSigmoidDiff(double) {
            SigmoidDifference* term = dynamic_cast<SigmoidDifference*> (selectedTerm());
            term->setFalling(ui->sbx_sigmoid_diff_falling->value());
            term->setLeft(ui->sbx_sigmoid_diff_left->value());
            term->setRight(ui->sbx_sigmoid_diff_right->value());
            term->setRising(ui->sbx_sigmoid_diff_rising->value());
            redraw();
        }

        void Term::onChangeSpinBoxSigmoidProd(double) {
            SigmoidProduct* term = dynamic_cast<SigmoidProduct*> (selectedTerm());
            term->setFalling(ui->sbx_sigmoid_prod_falling->value());
            term->setLeft(ui->sbx_sigmoid_prod_left->value());
            term->setRight(ui->sbx_sigmoid_prod_right->value());
            term->setRising(ui->sbx_sigmoid_prod_rising->value());
            redraw();
        }

        //EDGES

        void Term::onChangeSpinBoxRamp(double) {
            Ramp* term = dynamic_cast<Ramp*> (selectedTerm());
            term->setStart(ui->sbx_ramp_start->value());
            term->setEnd(ui->sbx_ramp_end->value());
            redraw();
        }

        void Term::onChangeSpinBoxSigmoid(double) {
            Sigmoid* term = dynamic_cast<Sigmoid*> (selectedTerm());
            term->setInflection(ui->sbx_sigmoid_inflection->value());
            term->setSlope(ui->sbx_sigmoid_slope->value());
            redraw();
        }

        void Term::onChangeSpinBoxSShape(double) {
            SShape* term = dynamic_cast<SShape*> (selectedTerm());
            term->setStart(ui->sbx_sshape_start->value());
            term->setEnd(ui->sbx_sshape_end->value());
            redraw();
        }

        void Term::onChangeSpinBoxZShape(double) {
            ZShape* term = dynamic_cast<ZShape*> (selectedTerm());
            term->setStart(ui->sbx_zshape_start->value());
            term->setEnd(ui->sbx_zshape_end->value());
            redraw();
        }

        void Term::onChangeSpinBoxConstant(double) {
            Constant* term = dynamic_cast<Constant*> (selectedTerm());
            term->setValue(ui->sbx_constant->value());
            redraw();
        }

        void Term::onChangeLinearCoefficient(QListWidgetItem* item) {
            if (not item) {
                redraw();
                return;
            }
            try {
                fl::Op::toScalar(item->text().toStdString());
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, tr("Linear Error"),
                        QString::fromStdString(ex.getWhat()), QMessageBox::Ok);
                item->setText(QString::fromStdString(fl::Op::str(0.0)));
            }
            Linear* term = dynamic_cast<Linear*> (selectedTerm());
            for (int i = 0; i < ui->lst_coefficients->count(); ++i) {
                try {
                    term->coefficients.at(i) =
                            fl::Op::toScalar(ui->lst_coefficients->item(i)->text().toStdString());
                } catch (fl::Exception& ex) {
                    FL_LOG(ex.what());
                    term->coefficients.at(i) = 0.0;
                }
            }
            redraw();
        }

        void Term::onClickFunctionProcess() {
            Function* term = dynamic_cast<Function*> (selectedTerm());
            std::string infix = ui->ptx_function->toPlainText().toStdString();
            term->setFormula(infix);
            try {
                term->load();
                term->membership(0);
                QString postfix = "f(x)=";
                if (term->root) {
                    postfix += QString::fromStdString(term->root->toPostfix());
                }
                ui->lbl_function->setText(postfix);
                redraw();
            } catch (fl::Exception& ex) {
                QMessageBox::critical(this, "Function error",
                        Window::toHtmlEscaped(QString::fromStdString(ex.getWhat())));
            }
        }

        void Term::onClickFunctionVariable() {
            if (ui->btn_function_variable->isChecked()) {
                std::vector<QAction*> actions;
                actions.push_back(new QAction("x", this));
                actions.push_back(NULL);
                const Engine* engine = Model::Default()->engine();
                for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
                    actions.push_back(new QAction(QString::fromStdString(
                            engine->getInputVariable(i)->getName()), this));
                }
                actions.push_back(NULL);
                for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                    actions.push_back(new QAction(QString::fromStdString(
                            engine->getOutputVariable(i)->getName()), this));
                }

                QMenu menu(this);
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
                        this, SLOT(onClickVariable(const QString &)));

                menu.exec(QCursor::pos() + QPoint(1, 0));
                for (std::size_t i = 0; i < actions.size(); ++i) {
                    if (actions.at(i)) {
                        actions.at(i)->deleteLater();
                    }
                }
                ui->btn_function_variable->setChecked(false);
            }
        }

        void Term::onClickFunctionBuiltIn() {
            if (ui->btn_function_builtin->isChecked()) {
                std::vector<QAction*> actions;

                Function f;
                f.loadBuiltInFunctions();
                for (std::map<std::string, Function::Operator*>::iterator it =
                        f.operators.begin(); it != f.operators.end(); ++it) {
                    actions.push_back(new QAction(QString::fromStdString(
                            it->first), this));
                }

                actions.push_back(NULL);
                for (std::map<std::string, Function::BuiltInFunction*>::iterator it =
                        f.functions.begin(); it != f.functions.end(); ++it) {
                    actions.push_back(new QAction(QString::fromStdString(
                            it->first), this));
                }

                QMenu menu(this);
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
                        this, SLOT(onClickBuiltIn(const QString &)));

                menu.exec(QCursor::pos() + QPoint(1, 0));
                for (std::size_t i = 0; i < actions.size(); ++i) {
                    if (actions.at(i)) {
                        actions.at(i)->deleteLater();
                    }
                }
                ui->btn_function_builtin->setChecked(false);
            }
        }

        void Term::onClickVariable(const QString& variable) {
            QTextCursor cursor = ui->ptx_function->textCursor();
            cursor.insertText(variable);
            ui->ptx_function->setFocus();
        }

        void Term::onClickBuiltIn(const QString& builtIn) {
            QTextCursor cursor = ui->ptx_function->textCursor();
            cursor.insertText(builtIn);
            ui->ptx_function->setFocus();
        }

        void Term::loadFrom(const fl::Term* x) {
            //BASIC
            if (x->className() == Triangle().className()) {
                const Triangle* term = dynamic_cast<const Triangle*> (x);
                scalar params[] = {term->getA(), term->getB(), term->getC()};
                ui->sbx_triangle_a->setValue(params[0]);
                ui->sbx_triangle_b->setValue(params[1]);
                ui->sbx_triangle_c->setValue(params[2]);
                ui->basicTermToolbox->setCurrentIndex(0);
                setCurrentToolbox(0);

            } else if (x->className() == Trapezoid().className()) {
                const Trapezoid* term = dynamic_cast<const Trapezoid*> (x);
                scalar params[] = {term->getA(), term->getB(), term->getC(), term->getD()};
                ui->sbx_trapezoid_a->setValue(params[0]);
                ui->sbx_trapezoid_b->setValue(params[1]);
                ui->sbx_trapezoid_c->setValue(params[2]);
                ui->sbx_trapezoid_d->setValue(params[3]);
                ui->basicTermToolbox->setCurrentIndex(1);
                setCurrentToolbox(0);

            } else if (x->className() == Rectangle().className()) {
                const Rectangle* term = dynamic_cast<const Rectangle*> (x);
                scalar params[] = {term->getStart(), term->getEnd()};
                ui->sbx_rectangle_min->setValue(params[0]);
                ui->sbx_rectangle_max->setValue(params[1]);
                ui->basicTermToolbox->setCurrentIndex(2);
                setCurrentToolbox(0);


            } else if (x->className() == Discrete().className()) {
                const Discrete* term = dynamic_cast<const Discrete*> (x);
                std::ostringstream xy;
                int size = std::min(term->x.size(), term->y.size());
                for (int i = 0; i < size; ++i) {
                    xy << "("
                            << fl::Op::str(term->x.at(i)) << ", "
                            << fl::Op::str(term->y.at(i)) << ")";
                    if (i + 1 < size) xy << "\n";
                }

                ui->ptx_discrete->setPlainText(QString::fromStdString(xy.str()));

                ui->basicTermToolbox->setCurrentIndex(3);
                setCurrentToolbox(0);
                //                onClickDiscreteParser();

                //EXTENDED
            } else if (x->className() == Gaussian().className()) {
                const Gaussian* term = dynamic_cast<const Gaussian*> (x);
                scalar params[] = {term->getMean(), term->getStandardDeviation()};
                ui->sbx_gaussian_center->setValue(params[0]);
                ui->sbx_gaussian_width->setValue(params[1]);
                ui->extendedTermToolbox->setCurrentIndex(0);
                setCurrentToolbox(1);

            } else if (x->className() == GaussianProduct().className()) {
                const GaussianProduct* term = dynamic_cast<const GaussianProduct*> (x);
                scalar params[] = {term->getMeanA(), term->getStandardDeviationA(),
                    term->getMeanB(), term->getStandardDeviationB()};
                ui->sbx_gaussian_prod_center_a->setValue(params[0]);
                ui->sbx_gaussian_prod_width_a->setValue(params[1]);
                ui->sbx_gaussian_prod_center_b->setValue(params[2]);
                ui->sbx_gaussian_prod_width_b->setValue(params[3]);
                ui->extendedTermToolbox->setCurrentIndex(1);
                setCurrentToolbox(1);

            } else if (x->className() == Bell().className()) {
                const Bell* term = dynamic_cast<const Bell*> (x);
                scalar params[] = {term->getCenter(), term->getWidth(), term->getSlope()};
                ui->sbx_bell_center->setValue(params[0]);
                ui->sbx_bell_width->setValue(params[1]);
                ui->sbx_bell_slope->setValue(params[2]);
                ui->extendedTermToolbox->setCurrentIndex(2);
                setCurrentToolbox(1);

            } else if (x->className() == PiShape().className()) {
                const PiShape* term = dynamic_cast<const PiShape*> (x);
                scalar params[] = {term->getBottomLeft(), term->getTopLeft(),
                    term->getTopRight(), term->getBottomRight()};
                ui->sbx_pishape_bl->setValue(params[0]);
                ui->sbx_pishape_tl->setValue(params[1]);
                ui->sbx_pishape_tr->setValue(params[2]);
                ui->sbx_pishape_br->setValue(params[3]);
                ui->extendedTermToolbox->setCurrentIndex(3);
                setCurrentToolbox(1);

            } else if (x->className() == SigmoidDifference().className()) {
                const SigmoidDifference* term = dynamic_cast<const SigmoidDifference*> (x);
                scalar params[] = {term->getLeft(), term->getRising(),
                    term->getFalling(), term->getRight()};
                ui->sbx_sigmoid_diff_left->setValue(params[0]);
                ui->sbx_sigmoid_diff_rising->setValue(params[1]);
                ui->sbx_sigmoid_diff_falling->setValue(params[2]);
                ui->sbx_sigmoid_diff_right->setValue(params[3]);

                ui->extendedTermToolbox->setCurrentIndex(4);
                setCurrentToolbox(1);

            } else if (x->className() == SigmoidProduct().className()) {
                const SigmoidProduct* term = dynamic_cast<const SigmoidProduct*> (x);
                scalar params[] = {term->getLeft(), term->getRising(),
                    term->getFalling(), term->getRight()};
                ui->sbx_sigmoid_prod_left->setValue(params[0]);
                ui->sbx_sigmoid_prod_rising->setValue(params[1]);
                ui->sbx_sigmoid_prod_falling->setValue(params[2]);
                ui->sbx_sigmoid_prod_right->setValue(params[3]);

                ui->extendedTermToolbox->setCurrentIndex(5);
                setCurrentToolbox(1);

                //EDGE
            } else if (x->className() == Ramp().className()) {
                const Ramp* term = dynamic_cast<const Ramp*> (x);
                scalar params[] = {term->getStart(), term->getEnd()};
                ui->sbx_ramp_start->setValue(params[0]);
                ui->sbx_ramp_end->setValue(params[1]);
                ui->edgeTermToolbox->setCurrentIndex(0);
                setCurrentToolbox(2);

            } else if (x->className() == Sigmoid().className()) {
                const Sigmoid* term = dynamic_cast<const Sigmoid*> (x);
                scalar params[] = {term->getInflection(), term->getSlope()};
                ui->sbx_sigmoid_inflection->setValue(params[0]);
                ui->sbx_sigmoid_slope->setValue(params[1]);
                ui->edgeTermToolbox->setCurrentIndex(1);
                setCurrentToolbox(2);

            } else if (x->className() == SShape().className()) {
                const SShape* term = dynamic_cast<const SShape*> (x);
                scalar params[] = {term->getStart(), term->getEnd()};
                ui->sbx_sshape_start->setValue(params[0]);
                ui->sbx_sshape_end->setValue(params[1]);
                ui->edgeTermToolbox->setCurrentIndex(2);
                setCurrentToolbox(2);

            } else if (x->className() == ZShape().className()) {
                const ZShape* term = dynamic_cast<const ZShape*> (x);
                scalar params[] = {term->getStart(), term->getEnd()};
                ui->sbx_zshape_start->setValue(params[0]);
                ui->sbx_zshape_end->setValue(params[1]);
                ui->edgeTermToolbox->setCurrentIndex(3);
                setCurrentToolbox(2);

                //FX
            } else if (x->className() == Constant().className()) {
                const Constant* term = dynamic_cast<const Constant*> (x);
                ui->sbx_constant->setValue(term->getValue());
                ui->fxTermToolbox->setCurrentIndex(0);
                setCurrentToolbox(3);

            } else if (x->className() == Linear().className()) {
                const Linear* term = dynamic_cast<const Linear*> (x);
                ui->lst_variables->clear();
                ui->lst_coefficients->clear();

                for (std::size_t i = 0; i < term->inputVariables.size(); ++i) {
                    QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(
                            term->inputVariables.at(i)->getName()));
                    item->setToolTip(item->text());
                    ui->lst_variables->addItem(item);
                }
                ui->lst_variables->addItem("c");

                for (std::size_t i = 0; i < term->coefficients.size(); ++i) {
                    QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(
                            fl::Op::str(term->coefficients.at(i))));
                    item->setToolTip(item->text());
                    item->setFlags(item->flags() | Qt::ItemIsEditable);
                    ui->lst_coefficients->addItem(item);
                }

                ui->fxTermToolbox->setCurrentIndex(1);
                setCurrentToolbox(3);

            } else if (x->className() == Function().className()) {
                const Function* term = dynamic_cast<const Function*> (x);
                ui->ptx_function->setPlainText(QString::fromStdString(term->getFormula()));
                QString postfix = "f(x)=";
                if (term->root) {
                    try {
                        term->membership(0);
                        postfix += QString::fromStdString(term->root->toPostfix());
                    } catch (fl::Exception& ex) {
                        QMessageBox::critical(this, "Function error",
                                Window::toHtmlEscaped(QString::fromStdString(ex.getWhat())));
                    }
                }
                ui->lbl_function->setText(postfix);
                ui->fxTermToolbox->setCurrentIndex(2);
                setCurrentToolbox(3);

            } else {
                std::ostringstream ex;
                ex << "[internal error] Term class <" << x->className() << "> not registered";
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

        void Term::main() {
            Term* t = new Term;
            t->setup(fl::Variable("", 0, 1));
            t->show();
        }

    }
}
