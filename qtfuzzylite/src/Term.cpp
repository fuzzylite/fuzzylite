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

#include <fl/Headers.h>

#include <QtGui/QMessageBox>

namespace fl {
    namespace qt {

        Term::Term(QWidget* parent, Qt::WindowFlags f)
        : QDialog(parent, f),
        ui(new Ui::Term), viewer(NULL), dummyVariable(new Variable),
        indexOfEditingTerm(-1) {
            setWindowFlags(Qt::Tool);
        }

        Term::~Term() {
            disconnect();
            delete ui;
            for (std::size_t i = 0; i < _basicTerms.size(); ++i) {
                delete _basicTerms[i];
            }
            for (std::size_t i = 0; i < _extendedTerms.size(); ++i) {
                delete _extendedTerms[i];
            }
            for (int i = dummyVariable->numberOfTerms() - 1; i >= 0; --i) {
                dummyVariable->removeTerm(i);
            }
            delete dummyVariable;

        }

        void Term::loadTerms(scalar min, scalar max) {
            scalar average = (min + max) / 2;
            scalar diff = (max - min);
            //It MUST follow the order of the toolbox.
            _basicTerms.push_back(new Triangle("", min, average, max));
            _basicTerms.push_back(new Trapezoid("", min, min + .25 * diff,
                    min + .75 * diff, max));
            _basicTerms.push_back(new Rectangle("", min + .25 * diff, min + .75 * max));
            _basicTerms.push_back(new Ramp("", min, max));

            std::vector<std::pair<scalar, scalar> > xy;
            xy.push_back(std::make_pair(min, 0));
            xy.push_back(std::make_pair(min + .25 * diff, 1));
            xy.push_back(std::make_pair(min + .5 * diff, .5));
            xy.push_back(std::make_pair(min + .75 * diff, 1));
            xy.push_back(std::make_pair(max, 0));
            _basicTerms.push_back(new Discrete("", xy));

            _extendedTerms.push_back(new Gaussian("", average, .2 * diff));
            _extendedTerms.push_back(new Bell("", average, .25 * diff, 3.0));
            _extendedTerms.push_back(new PiShape("", min, average, average, max));

            _extendedTerms.push_back(new Sigmoid("", average, 20 * average));
            _extendedTerms.push_back(new SShape("", min, max));
            _extendedTerms.push_back(new ZShape("", min, max));

        }

        void Term::setup(scalar minimum, scalar maximum) {
            dummyVariable->setMinimum(minimum);
            dummyVariable->setMaximum(maximum);
            loadTerms(dummyVariable->getMinimum(), dummyVariable->getMaximum());
            dummyVariable->addTerm(_basicTerms[0]); //Add the triangle by default
            ui->setupUi(this);

            viewer = new Viewer;
            viewer->setup(dummyVariable);
            ui->lyt_terms->insertWidget(0, viewer);

            setWindowTitle("Add term");
            layout()->setSizeConstraint(QLayout::SetFixedSize);
            this->adjustSize();
            QRect scr = Window::mainWindow()->geometry();
            move(scr.center() - rect().center());

            ui->basicTermToolbox->setCurrentIndex(0);
            ui->extendedTermToolbox->setCurrentIndex(0);
            ui->tabTerms->setCurrentIndex(0);

            _sbx.clear();
            _sbx.push_back(ui->sbx_bell_center);
            _sbx.push_back(ui->sbx_bell_width);
            _sbx.push_back(ui->sbx_bell_slope);

            _sbx.push_back(ui->sbx_gaussian_center);
            _sbx.push_back(ui->sbx_gaussian_width);

            _sbx.push_back(ui->sbx_pishape_a);
            _sbx.push_back(ui->sbx_pishape_b);
            _sbx.push_back(ui->sbx_pishape_c);
            _sbx.push_back(ui->sbx_pishape_d);

            _sbx.push_back(ui->sbx_ramp_start);
            _sbx.push_back(ui->sbx_ramp_end);

            _sbx.push_back(ui->sbx_rectangle_max);
            _sbx.push_back(ui->sbx_rectangle_min);

            _sbx.push_back(ui->sbx_sshape_start);
            _sbx.push_back(ui->sbx_sshape_end);

            _sbx.push_back(ui->sbx_sigmoid_inflection);
            _sbx.push_back(ui->sbx_sigmoid_slope);

            _sbx.push_back(ui->sbx_trapezoid_a);
            _sbx.push_back(ui->sbx_trapezoid_b);
            _sbx.push_back(ui->sbx_trapezoid_c);
            _sbx.push_back(ui->sbx_trapezoid_d);

            _sbx.push_back(ui->sbx_triangle_a);
            _sbx.push_back(ui->sbx_triangle_b);
            _sbx.push_back(ui->sbx_triangle_c);

            _sbx.push_back(ui->sbx_zshape_start);
            _sbx.push_back(ui->sbx_zshape_end);

            for (std::size_t i = 0; i < _sbx.size(); ++i) {
                _sbx[i]->setMinimum(-1000);
                _sbx[i]->setMaximum(1000);
                _sbx[i]->setValue(0.0);
                _sbx[i]->setSingleStep((maximum - minimum) / 100);
            }

            for (std::size_t i = 0; i < _basicTerms.size(); ++i) {
                loadFrom(_basicTerms[i]);
            }
            for (std::size_t i = 0; i < _extendedTerms.size(); ++i) {
                loadFrom(_extendedTerms[i]);
            }
            connect();
        }

        void Term::setup(const fl::Variable* variable) {
            dummyVariable->setMinimum(variable->getMinimum());
            dummyVariable->setMaximum(variable->getMaximum());
            for (int i = 0; i < variable->numberOfTerms(); ++i) {
                dummyVariable->addTerm(variable->getTerm(i));
            }
            setup(dummyVariable->getMinimum(), dummyVariable->getMaximum());
        }

        fl::Term* Term::getSelectedTerm() const {
            if (ui->tabTerms->currentIndex() == 0) {
                int index = ui->basicTermToolbox->currentIndex();
                return _basicTerms[index];
            }
            int index = ui->extendedTermToolbox->currentIndex();
            return _extendedTerms[index];
        }

        void Term::accept() {
            getSelectedTerm()->setName(ui->led_name->text().toStdString());
            QDialog::accept();
        }

        void Term::connect() {
            QObject::connect(ui->basicTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);
            QObject::connect(ui->extendedTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);

            QObject::connect(ui->tabTerms, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeTab(int)), Qt::QueuedConnection);

            QObject::connect(viewer, SIGNAL(valueChanged(double)),
                    this, SLOT(showSelectedTerm()));

            QObject::connect(ui->btn_wizard, SIGNAL(clicked()),
                    this, SLOT(onClickWizard()));

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

            //Ramp
            QObject::connect(ui->sbx_ramp_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRamp(double)));
            QObject::connect(ui->sbx_ramp_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRamp(double)));

            //Discrete
            QObject::connect(ui->btn_discrete_parse, SIGNAL(clicked()),
                    this, SLOT(onClickDiscreteParser()));

            //Gaussian
            QObject::connect(ui->sbx_gaussian_center, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::connect(ui->sbx_gaussian_width, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));

            //Bell
            QObject::connect(ui->sbx_bell_center, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_width, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_slope, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));

            //Pi-Shape
            QObject::connect(ui->sbx_pishape_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::connect(ui->sbx_pishape_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::connect(ui->sbx_pishape_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::connect(ui->sbx_pishape_d, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));


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

            //S-Shape
            QObject::connect(ui->sbx_zshape_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxZShape(double)));
            QObject::connect(ui->sbx_zshape_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxZShape(double)));

        }

        void Term::disconnect() {
            QObject::disconnect(ui->basicTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)));
            QObject::disconnect(ui->extendedTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)));

            QObject::disconnect(ui->tabTerms, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeTab(int)));
            QObject::disconnect(viewer, SIGNAL(valueChanged(double)),
                    this, SLOT(redraw()));

            //Triangle
            QObject::disconnect(ui->sbx_triangle_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTriangle(double)));
            QObject::disconnect(ui->sbx_triangle_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTriangle(double)));
            QObject::disconnect(ui->sbx_triangle_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTriangle(double)));

            //Trapezoid
            QObject::disconnect(ui->sbx_trapezoid_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));
            QObject::disconnect(ui->sbx_trapezoid_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));
            QObject::disconnect(ui->sbx_trapezoid_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));
            QObject::disconnect(ui->sbx_trapezoid_d, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxTrapezoid(double)));

            //Rectangle
            QObject::disconnect(ui->sbx_rectangle_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRectangle(double)));
            QObject::disconnect(ui->sbx_rectangle_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRectangle(double)));

            //Ramp
            QObject::disconnect(ui->sbx_ramp_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRamp(double)));
            QObject::disconnect(ui->sbx_ramp_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRamp(double)));

            //Discrete
            QObject::disconnect(ui->btn_discrete_parse, SIGNAL(clicked()),
                    this, SLOT(onClickDiscreteParser()));

            //Gaussian
            QObject::disconnect(ui->sbx_gaussian_center, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::disconnect(ui->sbx_gaussian_width, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));

            //Bell
            QObject::disconnect(ui->sbx_bell_center, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::disconnect(ui->sbx_bell_width, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::disconnect(ui->sbx_bell_slope, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));

            //Pi-Shape
            QObject::disconnect(ui->sbx_pishape_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::disconnect(ui->sbx_pishape_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::disconnect(ui->sbx_pishape_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));
            QObject::disconnect(ui->sbx_pishape_d, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxPiShape(double)));


            //Sigmoid
            QObject::disconnect(ui->sbx_sigmoid_inflection, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::disconnect(ui->sbx_sigmoid_slope, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));

            //S-Shape
            QObject::disconnect(ui->sbx_sshape_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSShape(double)));
            QObject::disconnect(ui->sbx_sshape_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSShape(double)));

            //S-Shape
            QObject::disconnect(ui->sbx_zshape_start, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxZShape(double)));
            QObject::disconnect(ui->sbx_zshape_end, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxZShape(double)));
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
        }

        void Term::showSelectedTerm() {
            fl::Term* selectedTerm = getSelectedTerm();
            viewer->draw(selectedTerm);
        }

        void Term::edit(const fl::Term* x) {
            //remove added term at setup.
            dummyVariable->removeTerm(dummyVariable->numberOfTerms() - 1);
            for (int i = 0; i < dummyVariable->numberOfTerms(); ++i) {
                if (dummyVariable->getTerm(i) == x) {
                    indexOfEditingTerm = i;
                    break;
                }
            }
            setWindowTitle("Edit term");
            ui->led_name->setText(QString::fromStdString(x->getName()));
            if (x->className() == Triangle().className()) {
                ui->basicTermToolbox->setCurrentIndex(0);
                ui->tabTerms->setCurrentIndex(0);
                onChangeToolBoxIndex(-1);
            } else if (x->className() == Trapezoid().className()) {
                ui->basicTermToolbox->setCurrentIndex(1);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == Rectangle().className()) {
                ui->basicTermToolbox->setCurrentIndex(2);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == Ramp().className()) {
                ui->basicTermToolbox->setCurrentIndex(3);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == Discrete().className()) {
                ui->basicTermToolbox->setCurrentIndex(4);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == Gaussian().className()) {
                ui->extendedTermToolbox->setCurrentIndex(0);
                ui->tabTerms->setCurrentIndex(1);

            } else if (x->className() == Bell().className()) {
                ui->extendedTermToolbox->setCurrentIndex(1);
                ui->tabTerms->setCurrentIndex(1);

            } else if (x->className() == PiShape().className()) {
                ui->extendedTermToolbox->setCurrentIndex(2);
                ui->tabTerms->setCurrentIndex(1);

            } else if (x->className() == Sigmoid().className()) {
                ui->extendedTermToolbox->setCurrentIndex(3);
                ui->tabTerms->setCurrentIndex(1);

            } else if (x->className() == SShape().className()) {
                ui->extendedTermToolbox->setCurrentIndex(4);
                ui->tabTerms->setCurrentIndex(1);

            } else if (x->className() == ZShape().className()) {
                ui->extendedTermToolbox->setCurrentIndex(5);
                ui->tabTerms->setCurrentIndex(1);

            }

            loadFrom(x);
            redraw();
        }

        fl::Term* Term::copySelectedTerm() const {
            fl::Term* x = getSelectedTerm();
            if (x->className() == Triangle().className()) {
                const Triangle* term = dynamic_cast<const Triangle*> (x);
                return new Triangle(*term);

            } else if (x->className() == Trapezoid().className()) {
                const Trapezoid* term = dynamic_cast<const Trapezoid*> (x);
                return new Trapezoid(*term);

            } else if (x->className() == Rectangle().className()) {
                const Rectangle* term = dynamic_cast<const Rectangle*> (x);
                return new Rectangle(*term);

            } else if (x->className() == Ramp().className()) {
                const Ramp* term = dynamic_cast<const Ramp*> (x);
                return new Ramp(*term);

            } else if (x->className() == Discrete().className()) {
                const Discrete* term = dynamic_cast<const Discrete*> (x);
                return new Discrete(*term);

            } else if (x->className() == Gaussian().className()) {
                const Gaussian* term = dynamic_cast<const Gaussian*> (x);
                return new Gaussian(*term);

            } else if (x->className() == Bell().className()) {
                const Bell* term = dynamic_cast<const Bell*> (x);
                return new Bell(*term);

            } else if (x->className() == PiShape().className()) {
                const PiShape* term = dynamic_cast<const PiShape*> (x);
                return new PiShape(*term);

            } else if (x->className() == Sigmoid().className()) {
                const Sigmoid* term = dynamic_cast<const Sigmoid*> (x);
                return new Sigmoid(*term);

            } else if (x->className() == SShape().className()) {
                const SShape* term = dynamic_cast<const SShape*> (x);
                return new SShape(*term);

            } else if (x->className() == ZShape().className()) {
                const ZShape* term = dynamic_cast<const ZShape*> (x);
                return new ZShape(*term);
            }
            std::ostringstream ex;
            ex << "trying to copy unknown term class <" << x->className() << ">";
            throw fl::Exception(ex.str());
        }

        /**
         * Events
         */

        void Term::onClickWizard() {
            Wizard window(this);
            window.setup(ui->led_name->text().toStdString());
            if (not window.exec()) return;

            int copies = window.ui->sbx_copies->value();
            QStringList names = window.ui->ptx_terms->toPlainText().split(
                    QRegExp("\\s+"));
            while (names.size() < copies) {
                names.append("");
            }

            scalar x = Model::Default()->configuration()->getDefuzzifier()->defuzzify(getSelectedTerm(),
                    dummyVariable->getMinimum(), dummyVariable->getMaximum());
            double separationDistance = window.ui->sbx_separation->value();
            for (int i = 0; i < copies; ++i) {
                scalar separation = x + (i + 1) * separationDistance;
                fl::Term* term = copySelectedTerm();
                term->setName(names[i].toStdString());
                newTerms.push_back(term);
                if (indexOfEditingTerm >= 0)
                    dummyVariable->insertTerm(term, indexOfEditingTerm + 1);
                else
                    dummyVariable->insertTerm(term, dummyVariable->numberOfTerms());
                if (term->className() == Triangle().className()) {
                    Triangle* term = dynamic_cast<Triangle*> (term);
                    term->setA(term->getA() + separation);
                    term->setB(term->getB() + separation);
                    term->setC(term->getC() + separation);

                } else if (term->className() == Trapezoid().className()) {
                    Trapezoid* term = dynamic_cast<Trapezoid*> (term);
                    term->setA(term->getA() + separation);
                    term->setB(term->getB() + separation);
                    term->setC(term->getC() + separation);
                    term->setD(term->getD() + separation);

                } else if (term->className() == Rectangle().className()) {
                    Rectangle* term = dynamic_cast<Rectangle*> (term);
                    term->setMinimum(term->getMinimum() + separation);
                    term->setMaximum(term->getMaximum() + separation);

                } else if (term->className() == Ramp().className()) {
                    Ramp* term = dynamic_cast<Ramp*> (term);
                    term->setStart(term->getStart() + separation);
                    term->setEnd(term->getEnd() + separation);

                } else if (term->className() == Discrete().className()) {
                    Discrete* term = dynamic_cast<Discrete*> (term);
                    for (std::size_t i = 0; i < term->x.size(); ++i) {
                        term->x[i] = term->x[i] + separation;
                    }
                } else if (term->className() == Gaussian().className()) {
                    Gaussian* term = dynamic_cast<Gaussian*> (term);
                    term->setMean(term->getMean() + separation);

                } else if (term->className() == Bell().className()) {
                    Bell* term = dynamic_cast<Bell*> (term);
                    term->setCenter(term->getCenter() + separation);

                } else if (term->className() == PiShape().className()) {
                    PiShape* term = dynamic_cast<PiShape*> (term);
                    term->setB(term->getB() + separation);
                    term->setC(term->getC() + separation);

                } else if (term->className() == Sigmoid().className()) {
                    Sigmoid* term = dynamic_cast<Sigmoid*> (term);
                    term->setInflection(term->getInflection() + separation);

                } else if (term->className() == SShape().className()) {
                    SShape* term = dynamic_cast<SShape*> (term);
                    term->setStart(term->getStart() + separation);
                    term->setEnd(term->getEnd() + separation);

                } else if (term->className() == ZShape().className()) {
                    ZShape* term = dynamic_cast<ZShape*> (term);
                    term->setStart(term->getStart() + separation);
                    term->setEnd(term->getEnd() + separation);
                }
            }

        }

        void Term::onChangeToolBoxIndex(int index) {
            (void) index;
            if (indexOfEditingTerm >= 0) {
                dummyVariable->removeTerm(indexOfEditingTerm);
                dummyVariable->insertTerm(getSelectedTerm(), indexOfEditingTerm);
            } else {
                dummyVariable->removeTerm(dummyVariable->numberOfTerms() - 1);
                dummyVariable->addTerm(getSelectedTerm());
            }
            redraw();
            this->adjustSize();
        }

        void Term::onChangeTab(int index) {
            ui->basicTermToolbox->setVisible(index == 0);
            ui->extendedTermToolbox->setVisible(index == 1);
            onChangeToolBoxIndex(-1);
            redraw();
            this->adjustSize();
        }

        void Term::onChangeSpinBoxTriangle(double) {
            if (fl::Op::IsGt(ui->sbx_triangle_a->value(), ui->sbx_triangle_b->value())) {
                ui->sbx_triangle_b->setValue(ui->sbx_triangle_a->value() + .1);
            }
            if (fl::Op::IsGt(ui->sbx_triangle_b->value(), ui->sbx_triangle_c->value())) {
                ui->sbx_triangle_c->setValue(ui->sbx_triangle_b->value() + .1);
            }
            if (fl::Op::IsGE(ui->sbx_triangle_a->value(), ui->sbx_triangle_c->value())) {
                ui->sbx_triangle_c->setValue(ui->sbx_triangle_a->value() + .1);
            }

            Triangle* term = dynamic_cast<Triangle*> (getSelectedTerm());
            term->setA(ui->sbx_triangle_a->value());
            term->setB(ui->sbx_triangle_b->value());
            term->setC(ui->sbx_triangle_c->value());

            redraw();
        }

        void Term::onChangeSpinBoxTrapezoid(double) {
            if (fl::Op::IsGt(ui->sbx_trapezoid_a->value(), ui->sbx_trapezoid_b->value())) {
                ui->sbx_trapezoid_b->setValue(ui->sbx_trapezoid_a->value() + .1);
            }
            if (fl::Op::IsGt(ui->sbx_trapezoid_b->value(), ui->sbx_trapezoid_c->value())) {
                ui->sbx_trapezoid_c->setValue(ui->sbx_trapezoid_b->value() + .1);
            }
            if (fl::Op::IsGt(ui->sbx_trapezoid_c->value(), ui->sbx_trapezoid_d->value())) {
                ui->sbx_trapezoid_d->setValue(ui->sbx_trapezoid_c->value() + .1);
            }
            if (fl::Op::IsGE(ui->sbx_trapezoid_a->value(), ui->sbx_trapezoid_d->value())) {
                ui->sbx_trapezoid_d->setValue(ui->sbx_trapezoid_a->value() + .1);
            }

            Trapezoid* term = dynamic_cast<Trapezoid*> (getSelectedTerm());
            term->setA(ui->sbx_trapezoid_a->value());
            term->setB(ui->sbx_trapezoid_b->value());
            term->setC(ui->sbx_trapezoid_c->value());
            term->setD(ui->sbx_trapezoid_d->value());
            redraw();
        }

        void Term::onChangeSpinBoxRectangle(double) {
            if (fl::Op::IsGE(ui->sbx_rectangle_min->value(), ui->sbx_rectangle_max->value())) {
                ui->sbx_rectangle_max->setValue(ui->sbx_rectangle_min->value() + .1);
            }
            Rectangle* term = dynamic_cast<Rectangle*> (getSelectedTerm());
            term->setMinimum(ui->sbx_rectangle_min->value());
            term->setMaximum(ui->sbx_rectangle_max->value());
            redraw();
        }

        void Term::onChangeSpinBoxRamp(double) {
            Ramp* term = dynamic_cast<Ramp*> (getSelectedTerm());
            term->setStart(ui->sbx_ramp_start->value());
            term->setEnd(ui->sbx_ramp_end->value());
            redraw();
        }

        void Term::onClickDiscreteParser() {
            std::string xString = ui->led_discrete_x->text().toStdString();
            std::vector<std::string> xValues = fl::Op::Split(xString, " ");

            std::string yString = ui->led_discrete_y->text().toStdString();
            std::vector<std::string> yValues = fl::Op::Split(yString, " ");

            if (xValues.size() != yValues.size()) {
                QString xSize = QString::number(xValues.size());
                QString ySize = QString::number(yValues.size());

                QString message = QString("[Discrete] The lengths of vectors x and y differ: ");
                message += QString("x[") + xSize + QString("] and y[") + ySize + QString("]");
                QMessageBox::warning(this, tr("Warning"),
                        message, QMessageBox::Ok);
            }

            Discrete* term = dynamic_cast<Discrete*> (getSelectedTerm());
            term->x.clear();
            term->y.clear();

            int size = std::min(xValues.size(), yValues.size());
            try {
                for (int i = 0; i < size; ++i) {
                    FL_LOG("<" << xValues[i] << "," << yValues[i] << ">");
                    term->x.push_back(fl::Op::Scalar(xValues[i]));
                    term->y.push_back(fl::Op::Scalar(yValues[i]));
                }
            } catch (...) {
                QMessageBox::critical(this, tr("Error"),
                        "[Discrete] Vectors x and y must contain numeric values",
                        QMessageBox::Ok);
            }
            redraw();
        }

        void Term::onChangeSpinBoxGaussian(double) {
            Gaussian* term = dynamic_cast<Gaussian*> (getSelectedTerm());
            term->setMean(ui->sbx_gaussian_center->value());
            term->setSigma(ui->sbx_gaussian_width->value());
            redraw();
        }

        void Term::onChangeSpinBoxBell(double) {
            Bell* term = dynamic_cast<Bell*> (getSelectedTerm());
            term->setCenter(ui->sbx_bell_center->value());
            term->setWidth(ui->sbx_bell_width->value());
            term->setSlope(ui->sbx_bell_slope->value());
            redraw();
        }

        void Term::onChangeSpinBoxPiShape(double) {
            PiShape* term = dynamic_cast<PiShape*> (getSelectedTerm());
            term->setA(ui->sbx_pishape_a->value());
            term->setB(ui->sbx_pishape_b->value());
            term->setC(ui->sbx_pishape_c->value());
            term->setD(ui->sbx_pishape_d->value());
            redraw();
        }

        void Term::onChangeSpinBoxSigmoid(double) {
            Sigmoid* term = dynamic_cast<Sigmoid*> (getSelectedTerm());
            term->setInflection(ui->sbx_sigmoid_inflection->value());
            term->setSlope(ui->sbx_sigmoid_slope->value());
            redraw();
        }

        void Term::onChangeSpinBoxSShape(double) {
            if (fl::Op::IsGE(ui->sbx_sshape_start->value(), ui->sbx_sshape_end->value())) {
                ui->sbx_sshape_end->setValue(ui->sbx_sshape_start->value() + .1);
            }
            SShape* term = dynamic_cast<SShape*> (getSelectedTerm());
            term->setStart(ui->sbx_sshape_start->value());
            term->setEnd(ui->sbx_sshape_end->value());
            redraw();
        }

        void Term::onChangeSpinBoxZShape(double) {
            if (fl::Op::IsGE(ui->sbx_zshape_start->value(), ui->sbx_zshape_end->value())) {
                ui->sbx_zshape_end->setValue(ui->sbx_zshape_start->value() + .1);
            }
            ZShape* term = dynamic_cast<ZShape*> (getSelectedTerm());
            term->setStart(ui->sbx_zshape_start->value());
            term->setEnd(ui->sbx_zshape_end->value());
            redraw();
        }

        void Term::loadFrom(const fl::Term* x) {
            if (x->className() == Triangle().className()) {
                const Triangle* term = dynamic_cast<const Triangle*> (x);
                ui->sbx_triangle_a->setValue(term->getA());
                ui->sbx_triangle_b->setValue(term->getB());
                ui->sbx_triangle_c->setValue(term->getC());

            } else if (x->className() == Trapezoid().className()) {
                const Trapezoid* term = dynamic_cast<const Trapezoid*> (x);
                ui->sbx_trapezoid_a->setValue(term->getA());
                ui->sbx_trapezoid_b->setValue(term->getB());
                ui->sbx_trapezoid_c->setValue(term->getC());
                ui->sbx_trapezoid_d->setValue(term->getD());

            } else if (x->className() == Rectangle().className()) {
                const Rectangle* term = dynamic_cast<const Rectangle*> (x);
                ui->sbx_rectangle_min->setValue(term->getMinimum());
                ui->sbx_rectangle_max->setValue(term->getMaximum());

            } else if (x->className() == Ramp().className()) {
                const Ramp* term = dynamic_cast<const Ramp*> (x);
                ui->sbx_ramp_start->setValue(term->getStart());
                ui->sbx_ramp_end->setValue(term->getEnd());

            } else if (x->className() == Discrete().className()) {
                const Discrete* term = dynamic_cast<const Discrete*> (x);
                std::ostringstream ssX, ssY;
                int size = std::min(term->x.size(), term->y.size());
                for (int i = 0; i < size; ++i) {
                    ssX << term->x[i];
                    ssY << term->y[i];
                    if (i < size - 1) {
                        ssX << " ";
                        ssY << " ";
                    }
                }
                ui->led_discrete_x->setText(QString::fromStdString(ssX.str()));
                ui->led_discrete_y->setText(QString::fromStdString(ssY.str()));

            } else if (x->className() == Gaussian().className()) {
                const Gaussian* term = dynamic_cast<const Gaussian*> (x);
                ui->sbx_gaussian_center->setValue(term->getMean());
                ui->sbx_gaussian_width->setValue(term->getSigma());

            } else if (x->className() == Bell().className()) {
                const Bell* term = dynamic_cast<const Bell*> (x);
                ui->sbx_bell_center->setValue(term->getCenter());
                ui->sbx_bell_width->setValue(term->getWidth());
                ui->sbx_bell_slope->setValue(term->getSlope());

            } else if (x->className() == PiShape().className()) {
                const PiShape* term = dynamic_cast<const PiShape*> (x);
                ui->sbx_pishape_a->setValue(term->getA());
                ui->sbx_pishape_b->setValue(term->getB());
                ui->sbx_pishape_c->setValue(term->getC());
                ui->sbx_pishape_d->setValue(term->getD());

            } else if (x->className() == Sigmoid().className()) {
                const Sigmoid* term = dynamic_cast<const Sigmoid*> (x);
                ui->sbx_sigmoid_inflection->setValue(term->getInflection());
                ui->sbx_sigmoid_slope->setValue(term->getSlope());

            } else if (x->className() == SShape().className()) {
                const SShape* term = dynamic_cast<const SShape*> (x);
                ui->sbx_sshape_start->setValue(term->getStart());
                ui->sbx_sshape_end->setValue(term->getEnd());

            } else if (x->className() == ZShape().className()) {
                const ZShape* term = dynamic_cast<const ZShape*> (x);
                ui->sbx_zshape_start->setValue(term->getStart());
                ui->sbx_zshape_end->setValue(term->getEnd());
            } else {
                std::ostringstream ex;
                ex << "[internal error] Term class <" << x->className() << "> not registered";
                throw fl::Exception(ex.str());
            }
        }

        void Term::main() {
            Term* t = new Term;
            t->setup(0, 1);
            t->show();
        }

    }
}
