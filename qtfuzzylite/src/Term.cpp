/*
 * Term.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Term.h"
#include <fl/Headers.h>
#include <QtGui/QMessageBox>
#include <unistd.h>
namespace fl {
    namespace qt {

        Term::Term(QWidget* parent, Qt::WindowFlags f)
                : QDialog(parent, f), ui(new Ui::Term) {
            setWindowFlags(Qt::Tool);
            //It MUST follow the order of the toolbox.
            _basicTerms.push_back(new Triangle("", 0, 0.5, 1));
            _basicTerms.push_back(new Trapezoid("", 0, 0.25, 0.75, 1));
            _basicTerms.push_back(new Rectangle("", 0, 1));
            _basicTerms.push_back(new LeftShoulder("", 0, 1));
            _basicTerms.push_back(new RightShoulder("", 0, 1));

            std::vector<std::pair<scalar, scalar> > xy;
            xy.push_back(std::make_pair(0, 0));
            xy.push_back(std::make_pair(1, 1));
            xy.push_back(std::make_pair(2, .5));
            xy.push_back(std::make_pair(3, 1));
            xy.push_back(std::make_pair(4, 0));
            _basicTerms.push_back(new Discrete("", xy));

            _extendedTerms.push_back(new Gaussian("", 0.5, 0.0, -1.0, 1.0));
            _extendedTerms.push_back(new Bell("", 0.5, 2.0, 0.0, -1.0, 1.0));
            _extendedTerms.push_back(new Sigmoid("", 4.0, 0.0, -1.0, 1.0));
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

        }

        void Term::setup() {
            ui->setupUi(this);
            layout()->setSizeConstraint(QLayout::SetFixedSize);
            this->adjustSize();
            QRect scr = parentWidget()->geometry();
            move(scr.center() - rect().center());
            ui->basicTermToolbox->setCurrentIndex(0);
            ui->extendedTermToolbox->setCurrentIndex(0);
            ui->tabTerms->setCurrentIndex(0);

            _sbx.clear();
            _sbx.push_back(ui->sbx_bell_a);
            _sbx.push_back(ui->sbx_bell_b);
            _sbx.push_back(ui->sbx_bell_c);
            _sbx.push_back(ui->sbx_bell_max);
            _sbx.push_back(ui->sbx_bell_min);

            _sbx.push_back(ui->sbx_gaussian_c);
            _sbx.push_back(ui->sbx_gaussian_max);
            _sbx.push_back(ui->sbx_gaussian_min);
            _sbx.push_back(ui->sbx_gaussian_sigma);

            _sbx.push_back(ui->sbx_leftshoulder_max);
            _sbx.push_back(ui->sbx_leftshoulder_min);

            _sbx.push_back(ui->sbx_rectangle_max);
            _sbx.push_back(ui->sbx_rectangle_min);

            _sbx.push_back(ui->sbx_rightshoulder_max);
            _sbx.push_back(ui->sbx_rightshoulder_min);

            _sbx.push_back(ui->sbx_sigmoid_a);
            _sbx.push_back(ui->sbx_sigmoid_c);
            _sbx.push_back(ui->sbx_sigmoid_max);
            _sbx.push_back(ui->sbx_sigmoid_min);

            _sbx.push_back(ui->sbx_trapezoid_a);
            _sbx.push_back(ui->sbx_trapezoid_b);
            _sbx.push_back(ui->sbx_trapezoid_c);
            _sbx.push_back(ui->sbx_trapezoid_d);

            _sbx.push_back(ui->sbx_triangle_a);
            _sbx.push_back(ui->sbx_triangle_b);
            _sbx.push_back(ui->sbx_triangle_c);

            for (std::size_t i = 0; i < _sbx.size(); ++i) {
                _sbx[i]->setMinimum(-999999);
                _sbx[i]->setMaximum(999999);
                _sbx[i]->setValue(0);
                _sbx[i]->setSingleStep(0.1);
            }

            for (std::size_t i = 0; i < _basicTerms.size(); ++i) {
                loadFrom(_basicTerms[i]);
            }
            for (std::size_t i = 0; i < _extendedTerms.size(); ++i) {
                loadFrom(_extendedTerms[i]);
            }
            connect();
        }

        fl::Term* Term::getSelectedTerm() const {
            if (ui->tabTerms->currentIndex() == 0) {
                int index = ui->basicTermToolbox->currentIndex();
                return _basicTerms[index];
            }
            int index = ui->extendedTermToolbox->currentIndex();
            return _extendedTerms[index];
        }

        void Term::connect() {
            QObject::connect(ui->basicTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);
            QObject::connect(ui->extendedTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)), Qt::QueuedConnection);

            QObject::connect(ui->tabTerms, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeTab(int)), Qt::QueuedConnection);

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

            //Left and Right shoulders
            QObject::connect(ui->sbx_leftshoulder_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxLeftShoulder(double)));
            QObject::connect(ui->sbx_leftshoulder_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxLeftShoulder(double)));

            QObject::connect(ui->sbx_rightshoulder_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRightShoulder(double)));
            QObject::connect(ui->sbx_rightshoulder_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRightShoulder(double)));

            //Discrete
            QObject::connect(ui->btn_discrete_parse, SIGNAL(clicked()),
                    this, SLOT(onClickDiscreteParser()));

            //Gaussian
            QObject::connect(ui->sbx_gaussian_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::connect(ui->sbx_gaussian_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::connect(ui->sbx_gaussian_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::connect(ui->sbx_gaussian_sigma, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));

            //Bell
            QObject::connect(ui->sbx_bell_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::connect(ui->sbx_bell_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));

            //Sigmoid
            QObject::connect(ui->sbx_sigmoid_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::connect(ui->sbx_sigmoid_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::connect(ui->sbx_sigmoid_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::connect(ui->sbx_sigmoid_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));

        }

        void Term::disconnect() {
            QObject::disconnect(ui->basicTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)));
            QObject::disconnect(ui->extendedTermToolbox, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeToolBoxIndex(int)));

            QObject::disconnect(ui->tabTerms, SIGNAL(currentChanged(int)),
                    this, SLOT(onChangeTab(int)));

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

            //Left and Right shoulders
            QObject::disconnect(ui->sbx_leftshoulder_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxLeftShoulder(double)));
            QObject::disconnect(ui->sbx_leftshoulder_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxLeftShoulder(double)));

            QObject::disconnect(ui->sbx_rightshoulder_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRightShoulder(double)));
            QObject::disconnect(ui->sbx_rightshoulder_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxRightShoulder(double)));

            //Discrete
            QObject::disconnect(ui->btn_discrete_parse, SIGNAL(clicked()),
                    this, SLOT(onClickDiscreteParser()));

            //Gaussian
            QObject::disconnect(ui->sbx_gaussian_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::disconnect(ui->sbx_gaussian_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::disconnect(ui->sbx_gaussian_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));
            QObject::disconnect(ui->sbx_gaussian_sigma, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxGaussian(double)));

            //Bell
            QObject::disconnect(ui->sbx_bell_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::disconnect(ui->sbx_bell_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::disconnect(ui->sbx_bell_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::disconnect(ui->sbx_bell_b, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));
            QObject::disconnect(ui->sbx_bell_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxBell(double)));

            //Sigmoid
            QObject::disconnect(ui->sbx_sigmoid_min, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::disconnect(ui->sbx_sigmoid_max, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::disconnect(ui->sbx_sigmoid_a, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
            QObject::disconnect(ui->sbx_sigmoid_c, SIGNAL(valueChanged(double)),
                    this, SLOT(onChangeSpinBoxSigmoid(double)));
        }

        void Term::showEvent(QShowEvent* event) {
            ui->canvas->scene()->setSceneRect(ui->canvas->viewport()->rect());
            ui->canvas->fitInView(0, 0, ui->canvas->scene()->width(),
                    ui->canvas->scene()->height(), Qt::IgnoreAspectRatio);
            refresh();
            QWidget::showEvent(event);
        }

        void Term::resizeEvent(QResizeEvent* event) {
            refresh();
            QWidget::resizeEvent(event);
        }

        void Term::refresh() {
            fl::Term* selectedTerm = getSelectedTerm();
            ui->canvas->clear();
            ui->canvas->setMinimum(selectedTerm->minimum());
            ui->canvas->setMaximum(selectedTerm->maximum());
            ui->canvas->draw(selectedTerm);
        }

        void Term::edit(const fl::Term* x) {
            ui->led_name->setText(QString::fromStdString(x->getName()));
            loadFrom(x);
            if (x->className() == Triangle().className()) {
                ui->basicTermToolbox->setCurrentIndex(0);
                ui->tabTerms->setCurrentIndex(0);
            } else if (x->className() == Trapezoid().className()) {
                ui->basicTermToolbox->setCurrentIndex(1);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == Rectangle().className()) {
                ui->basicTermToolbox->setCurrentIndex(2);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == LeftShoulder().className()) {
                ui->basicTermToolbox->setCurrentIndex(3);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == RightShoulder().className()) {
                ui->basicTermToolbox->setCurrentIndex(4);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == Discrete().className()) {
                ui->basicTermToolbox->setCurrentIndex(5);
                ui->tabTerms->setCurrentIndex(0);

            } else if (x->className() == Gaussian().className()) {
                ui->basicTermToolbox->setCurrentIndex(0);
                ui->tabTerms->setCurrentIndex(1);

            } else if (x->className() == Bell().className()) {
                ui->basicTermToolbox->setCurrentIndex(1);
                ui->tabTerms->setCurrentIndex(1);

            } else if (x->className() == Sigmoid().className()) {
                ui->basicTermToolbox->setCurrentIndex(2);
                ui->tabTerms->setCurrentIndex(1);
            }
        }

        fl::Term* Term::copySelectedTerm() const {
            fl::Term* x = getSelectedTerm();
            if (x->className() == Triangle().className()) {
                const Triangle* term = dynamic_cast<const Triangle*>(x);
                return new Triangle(*term);

            } else if (x->className() == Trapezoid().className()) {
                const Trapezoid* term = dynamic_cast<const Trapezoid*>(x);
                return new Trapezoid(*term);

            } else if (x->className() == Rectangle().className()) {
                const Rectangle* term = dynamic_cast<const Rectangle*>(x);
                return new Rectangle(*term);

            } else if (x->className() == LeftShoulder().className()) {
                const LeftShoulder* term = dynamic_cast<const LeftShoulder*>(x);
                return new LeftShoulder(*term);

            } else if (x->className() == RightShoulder().className()) {
                const RightShoulder* term = dynamic_cast<const RightShoulder*>(x);
                return new RightShoulder(*term);

            } else if (x->className() == Discrete().className()) {
                const Discrete* term = dynamic_cast<const Discrete*>(x);
                return new Discrete(*term);

            } else if (x->className() == Gaussian().className()) {
                const Gaussian* term = dynamic_cast<const Gaussian*>(x);
                return new Gaussian(*term);

            } else if (x->className() == Bell().className()) {
                const Bell* term = dynamic_cast<const Bell*>(x);
                return new Bell(*term);

            } else if (x->className() == Sigmoid().className()) {
                const Sigmoid* term = dynamic_cast<const Sigmoid*>(x);
                return new Sigmoid(*term);
            }
            FL_LOG("trying to copy unknown term class <" << x->className() << ">");
            throw std::exception();
        }

        /**
         * Events
         */
        void Term::onChangeToolBoxIndex(int index) {
            (void) index;
            refresh();
            this->adjustSize();
        }

        void Term::onChangeTab(int index) {
            ui->basicTermToolbox->setVisible(index == 0);
            ui->extendedTermToolbox->setVisible(index == 1);
            refresh();
            this->adjustSize();
        }

        void Term::onChangeSpinBoxTriangle(double dummyValue) {
            (void) dummyValue;
            if (fl::Op::IsGt(ui->sbx_triangle_a->value(), ui->sbx_triangle_b->value())) {
                ui->sbx_triangle_b->setValue(ui->sbx_triangle_a->value() + .1);
            }
            if (fl::Op::IsGt(ui->sbx_triangle_b->value(), ui->sbx_triangle_c->value())) {
                ui->sbx_triangle_c->setValue(ui->sbx_triangle_b->value() + .1);
            }
            if (fl::Op::IsGE(ui->sbx_triangle_a->value(), ui->sbx_triangle_c->value())) {
                ui->sbx_triangle_c->setValue(ui->sbx_triangle_a->value() + .1);
            }

            Triangle* term = dynamic_cast<Triangle*>(getSelectedTerm());
            term->setA(ui->sbx_triangle_a->value());
            term->setB(ui->sbx_triangle_b->value());
            term->setC(ui->sbx_triangle_c->value());

            refresh();
        }

        void Term::onChangeSpinBoxTrapezoid(double dummyValue) {
            (void) dummyValue;
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

            Trapezoid* term = dynamic_cast<Trapezoid*>(getSelectedTerm());
            term->setA(ui->sbx_trapezoid_a->value());
            term->setB(ui->sbx_trapezoid_b->value());
            term->setC(ui->sbx_trapezoid_c->value());
            term->setD(ui->sbx_trapezoid_d->value());
            refresh();
        }

        void Term::onChangeSpinBoxRectangle(double dummyValue) {
            (void) dummyValue;
            if (fl::Op::IsGE(ui->sbx_rectangle_min->value(), ui->sbx_rectangle_max->value())) {
                ui->sbx_rectangle_max->setValue(ui->sbx_rectangle_min->value() + .1);
            }
            Rectangle* term = dynamic_cast<Rectangle*>(getSelectedTerm());
            term->setMinimum(ui->sbx_rectangle_min->value());
            term->setMaximum(ui->sbx_rectangle_max->value());
            refresh();
        }

        void Term::onChangeSpinBoxLeftShoulder(double dummyValue) {
            (void) dummyValue;
            if (fl::Op::IsGE(ui->sbx_leftshoulder_min->value(), ui->sbx_leftshoulder_max->value())) {
                ui->sbx_leftshoulder_max->setValue(ui->sbx_leftshoulder_min->value() + .1);
            }
            LeftShoulder* term = dynamic_cast<LeftShoulder*>(getSelectedTerm());
            term->setMinimum(ui->sbx_leftshoulder_min->value());
            term->setMaximum(ui->sbx_leftshoulder_max->value());
            refresh();
        }
        void Term::onChangeSpinBoxRightShoulder(double dummyValue) {
            (void) dummyValue;
            if (fl::Op::IsGE(ui->sbx_rightshoulder_min->value(), ui->sbx_rightshoulder_max->value())) {
                ui->sbx_rightshoulder_max->setValue(ui->sbx_rightshoulder_min->value() + .1);
            }
            RightShoulder* term = dynamic_cast<RightShoulder*>(getSelectedTerm());
            term->setMinimum(ui->sbx_rightshoulder_min->value());
            term->setMaximum(ui->sbx_rightshoulder_max->value());
            refresh();
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

            Discrete* term = dynamic_cast<Discrete*>(getSelectedTerm());
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
            refresh();
        }

        void Term::onChangeSpinBoxGaussian(double dummyValue) {
            (void) dummyValue;
            if (fl::Op::IsGE(ui->sbx_gaussian_min->value(), ui->sbx_gaussian_max->value())) {
                ui->sbx_gaussian_max->setValue(ui->sbx_gaussian_min->value() + .1);
            }
            Gaussian* term = dynamic_cast<Gaussian*>(getSelectedTerm());
            term->setC(ui->sbx_gaussian_c->value());
            term->setSigma(ui->sbx_gaussian_sigma->value());
            term->setMinimum(ui->sbx_gaussian_min->value());
            term->setMaximum(ui->sbx_gaussian_max->value());
            refresh();
        }

        void Term::onChangeSpinBoxBell(double dummyValue) {
            (void) dummyValue;
            if (fl::Op::IsGE(ui->sbx_bell_min->value(), ui->sbx_bell_max->value())) {
                ui->sbx_bell_max->setValue(ui->sbx_bell_min->value() + .1);
            }

            Bell* term = dynamic_cast<Bell*>(getSelectedTerm());
            term->setA(ui->sbx_bell_a->value());
            term->setB(ui->sbx_bell_b->value());
            term->setC(ui->sbx_bell_c->value());
            term->setMinimum(ui->sbx_bell_min->value());
            term->setMaximum(ui->sbx_bell_max->value());
            refresh();
        }

        void Term::onChangeSpinBoxSigmoid(double dummyValue) {
            (void) dummyValue;
            if (fl::Op::IsGE(ui->sbx_sigmoid_min->value(), ui->sbx_sigmoid_max->value())) {
                ui->sbx_sigmoid_max->setValue(ui->sbx_sigmoid_min->value() + .1);
            }
            Sigmoid* term = dynamic_cast<Sigmoid*>(getSelectedTerm());
            term->setA(ui->sbx_sigmoid_a->value());
            term->setC(ui->sbx_sigmoid_c->value());
            term->setMinimum(ui->sbx_sigmoid_min->value());
            term->setMaximum(ui->sbx_sigmoid_max->value());
            refresh();
        }

        void Term::loadFrom(const fl::Term* x) {
            if (x->className() == Triangle().className()) {
                const Triangle* term = dynamic_cast<const Triangle*>(x);
                ui->sbx_triangle_a->setValue(term->getA());
                ui->sbx_triangle_b->setValue(term->getB());
                ui->sbx_triangle_c->setValue(term->getC());

            } else if (x->className() == Trapezoid().className()) {
                const Trapezoid* term = dynamic_cast<const Trapezoid*>(x);
                ui->sbx_trapezoid_a->setValue(term->getA());
                ui->sbx_trapezoid_b->setValue(term->getB());
                ui->sbx_trapezoid_c->setValue(term->getC());
                ui->sbx_trapezoid_d->setValue(term->getD());

            } else if (x->className() == Rectangle().className()) {
                const Rectangle* term = dynamic_cast<const Rectangle*>(x);
                ui->sbx_rectangle_min->setValue(term->minimum());
                ui->sbx_rectangle_max->setValue(term->maximum());

            } else if (x->className() == LeftShoulder().className()) {
                const LeftShoulder* term = dynamic_cast<const LeftShoulder*>(x);
                ui->sbx_leftshoulder_min->setValue(term->minimum());
                ui->sbx_leftshoulder_max->setValue(term->maximum());

            } else if (x->className() == RightShoulder().className()) {
                const RightShoulder* term = dynamic_cast<const RightShoulder*>(x);
                ui->sbx_rightshoulder_min->setValue(term->minimum());
                ui->sbx_rightshoulder_max->setValue(term->maximum());

            } else if (x->className() == Discrete().className()) {
                const Discrete* term = dynamic_cast<const Discrete*>(x);
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
                const Gaussian* term = dynamic_cast<const Gaussian*>(x);
                ui->sbx_gaussian_min->setValue(term->minimum());
                ui->sbx_gaussian_max->setValue(term->maximum());
                ui->sbx_gaussian_c->setValue(term->getC());
                ui->sbx_gaussian_sigma->setValue(term->getSigma());

            } else if (x->className() == Bell().className()) {
                const Bell* term = dynamic_cast<const Bell*>(x);
                ui->sbx_bell_min->setValue(term->minimum());
                ui->sbx_bell_max->setValue(term->maximum());
                ui->sbx_bell_a->setValue(term->getA());
                ui->sbx_bell_b->setValue(term->getB());
                ui->sbx_bell_c->setValue(term->getC());

            } else if (x->className() == Sigmoid().className()) {
                const Sigmoid* term = dynamic_cast<const Sigmoid*>(x);
                ui->sbx_sigmoid_min->setValue(term->minimum());
                ui->sbx_sigmoid_max->setValue(term->maximum());
                ui->sbx_sigmoid_a->setValue(term->getA());
                ui->sbx_sigmoid_c->setValue(term->getC());
            }
        }

        void Term::main() {
            Term* t = new Term;
            t->setup();
            t->show();
        }

    } /* namespace qt */
} /* namespace fl */
