/*
 * Term.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/qt/Term.h"
#include <fl/Headers.h>
#include <QtGui/QMessageBox>

namespace fl {
    namespace qt {

        Term::Term(QWidget* parent, Qt::WindowFlags f)
                : QDialog(parent, f), ui(new Ui::Term) {
            //It MUST follow the order of the toolbox.
            _basicTerms.push_back(new Triangle("", 0, 0.5, 1));
            _basicTerms.push_back(new Trapezoid("", 0, 0.25, 0.75, 1));
            _basicTerms.push_back(new Rectangle("", 0, 1));
            _basicTerms.push_back(new LeftShoulder("", 0, 1));
            _basicTerms.push_back(new RightShoulder("", 0, 1));

            std::vector<std::pair<scalar, scalar> > xy;
            xy.push_back(std::make_pair(0.0, 0.0));
            xy.push_back(std::make_pair(0.5, 1.0));
            xy.push_back(std::make_pair(1.0, 0.0));
            _basicTerms.push_back(new Discrete("", xy));

            _advancedTerms.push_back(new Gaussian);
            _advancedTerms.push_back(new Bell);
            _advancedTerms.push_back(new Sigmoid);
        }

        Term::~Term() {
            disconnect();
            delete ui;
        }

        void Term::setup() {
            ui->setupUi(this);
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
            connect();
        }

        fl::Term* Term::getSelectedTerm() const {
            if (ui->tabTerms->currentIndex() == 0) {
                int index = ui->basicTermToolbox->currentIndex();
                return _basicTerms[index];
            }
            int index = ui->extendedTermToolbox->currentIndex();
            return _advancedTerms[index];
        }

        void Term::connect() {
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

        void Term::refresh() {
            fl::Term* selectedTerm = getSelectedTerm();
            FL_LOG(selectedTerm->toString());
            ui->canvas->clear();
            ui->canvas->setMinimum(selectedTerm->minimum());
            ui->canvas->setMaximum(selectedTerm->maximum());
            ui->canvas->draw(selectedTerm);
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

        void Term::main() {
            Term* t = new Term;
            t->setup();
            t->show();
        }

    } /* namespace qt */
} /* namespace fl */
