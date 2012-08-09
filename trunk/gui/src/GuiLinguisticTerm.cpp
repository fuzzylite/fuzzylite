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
/*
 * GuiLinguisticTerm.cpp
 *
 *  Created on: Dec 14, 2009
 *      Author: jcrada
 */

#include "gui/GuiLinguisticTerm.h"

namespace fl_gui {

    GuiLinguisticTerm::GuiLinguisticTerm(QWidget* parent, Qt::WindowFlags f) :
    QDialog(parent, f), _ui(new Ui::LinguisticTerm) {
    }

    GuiLinguisticTerm::~GuiLinguisticTerm() {
        disconnect();
        delete _ui;
    }

    Ui::LinguisticTerm& GuiLinguisticTerm::getUi() const {
        return *this->_ui;
    }

    void GuiLinguisticTerm::setup() {
        getUi().setupUi(this);
        connect();
    }

    void GuiLinguisticTerm::connect() {
        QObject::connect(getUi().dsx_triangular_a, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTriangularA(double)));
        QObject::connect(getUi().dsx_triangular_b, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTriangularB(double)));
        QObject::connect(getUi().dsx_triangular_c, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTriangularC(double)));

        QObject::connect(getUi().dsx_rectangular_min, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeRectangularMin(double)));
        QObject::connect(getUi().dsx_rectangular_max, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeRectangularMax(double)));

        QObject::connect(getUi().dsx_trapezoidal_a, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTrapezoidalA(double)));
        QObject::connect(getUi().dsx_trapezoidal_b, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTrapezoidalB(double)));
        QObject::connect(getUi().dsx_trapezoidal_c, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTrapezoidalC(double)));
        QObject::connect(getUi().dsx_trapezoidal_d, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTrapezoidalD(double)));

        QObject::connect(getUi().dsx_shoulder_min, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeShoulderMin(double)));
        QObject::connect(getUi().dsx_shoulder_max, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeShoulderMax(double)));
        QObject::connect(getUi().rbt_shoulder_left, SIGNAL(clicked()), this,
                SLOT(onChangeShoulderSide()));
        QObject::connect(getUi().rbt_shoulder_right, SIGNAL(clicked()), this,
                SLOT(onChangeShoulderSide()));

        QObject::connect(getUi().dsx_singleton_value, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeSingletonValue(double)));

        QObject::connect(getUi().led_fx, SIGNAL(textChanged(const QString&)), this,
                SLOT(onChangeFxFunction(const QString&)));
        QObject::connect(getUi().dsx_fx_min, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeFxMin(double)));
        QObject::connect(getUi().dsx_fx_max, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeFxMax(double)));

        QObject::connect(getUi().tbw_lterms, SIGNAL(currentChanged(int)), this,
                SLOT(updateUi()));
    }

    void GuiLinguisticTerm::disconnect() {
        QObject::disconnect(getUi().dsx_triangular_a, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTriangularA(double)));
        QObject::disconnect(getUi().dsx_triangular_b, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTriangularB(double)));
        QObject::disconnect(getUi().dsx_triangular_c, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeTriangularC(double)));

        QObject::disconnect(getUi().dsx_rectangular_min, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeRectangularMin(double)));
        QObject::disconnect(getUi().dsx_rectangular_max, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeRectangularMax(double)));

        QObject::disconnect(getUi().dsx_trapezoidal_a, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidalA(double)));
        QObject::disconnect(getUi().dsx_trapezoidal_b, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidalB(double)));
        QObject::disconnect(getUi().dsx_trapezoidal_c, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidalC(double)));
        QObject::disconnect(getUi().dsx_trapezoidal_d, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidalD(double)));

        QObject::disconnect(getUi().dsx_shoulder_min, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeShoulderMin(double)));
        QObject::disconnect(getUi().dsx_shoulder_max, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeShoulderMax(double)));
        QObject::disconnect(getUi().rbt_shoulder_left, SIGNAL(clicked()), this,
                SLOT(onChangeShoulderSide()));
        QObject::disconnect(getUi().rbt_shoulder_right, SIGNAL(clicked()), this,
                SLOT(onChangeShoulderSide()));

        QObject::disconnect(getUi().dsx_singleton_value, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeSingletonValue(double)));

        QObject::disconnect(getUi().led_fx, SIGNAL(textChanged(const QString&)), this,
                SLOT(onChangeFxFunction(const QString&)));
        QObject::disconnect(getUi().dsx_fx_min, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeFxMin(double)));
        QObject::disconnect(getUi().dsx_fx_max, SIGNAL(valueChanged(double)), this,
                SLOT(onChangeFxMax(double)));

        QObject::disconnect(getUi().tbw_lterms, SIGNAL(currentChanged(int)), this,
                SLOT(updateUi()));
    }

    void GuiLinguisticTerm::showTerm(const fl::LinguisticTerm& term) {
        getUi().led_name->setText(QString::fromStdString(term.name()));
        switch (term.type()) {
            case fl::LinguisticTerm::MF_TRIANGULAR:
            {
                const fl::TriangularTerm* dy_term =
                        dynamic_cast<const fl::TriangularTerm*> (&term);
                fl::FuzzyException::Assert(FL_AT, dy_term != NULL);
                getUi().dsx_triangular_a->setValue(dy_term->a());
                getUi().dsx_triangular_b->setValue(dy_term->b());
                getUi().dsx_triangular_c->setValue(dy_term->c());
                getUi().tbw_lterms->setCurrentIndex(MF_TRIANGULAR);
                break;
            }
            case fl::LinguisticTerm::MF_RECTANGULAR:
            {
                const fl::RectangularTerm* dy_term =
                        dynamic_cast<const fl::RectangularTerm*> (&term);
                fl::FuzzyException::Assert(FL_AT, dy_term != NULL);
                getUi().dsx_rectangular_min->setValue(dy_term->minimum());
                getUi().dsx_rectangular_max->setValue(dy_term->maximum());
                getUi().tbw_lterms->setCurrentIndex(MF_RECTANGULAR);
                break;
            }
            case fl::LinguisticTerm::MF_TRAPEZOIDAL:
            {
                const fl::TrapezoidalTerm* dy_term =
                        dynamic_cast<const fl::TrapezoidalTerm*> (&term);
                fl::FuzzyException::Assert(FL_AT, dy_term != NULL);
                getUi().dsx_trapezoidal_a->setValue(dy_term->a());
                getUi().dsx_trapezoidal_b->setValue(dy_term->b());
                getUi().dsx_trapezoidal_c->setValue(dy_term->c());
                getUi().dsx_trapezoidal_d->setValue(dy_term->d());
                getUi().tbw_lterms->setCurrentIndex(MF_TRAPEZOIDAL);
                break;
            }
            case fl::LinguisticTerm::MF_SHOULDER:
            {
                const fl::ShoulderTerm* dy_term =
                        dynamic_cast<const fl::ShoulderTerm*> (&term);
                fl::FuzzyException::Assert(FL_AT, dy_term != NULL);
                getUi().dsx_shoulder_min->setValue(dy_term->minimum());
                getUi().dsx_shoulder_max->setValue(dy_term->maximum());
                if (dy_term->isLeft()) {
                    getUi().rbt_shoulder_left->setChecked(true);
                } else {
                    getUi().rbt_shoulder_right->setChecked(true);
                }
                getUi().tbw_lterms->setCurrentIndex(MF_SHOULDER);
                break;
            }
            case fl::LinguisticTerm::MF_SINGLETON:
            {
                const fl::SingletonTerm* dy_term =
                        dynamic_cast<const fl::SingletonTerm*> (&term);
                fl::FuzzyException::Assert(FL_AT, dy_term != NULL);
                getUi().dsx_singleton_value->setValue(dy_term->value());
                getUi().tbw_lterms->setCurrentIndex(MF_SINGLETON);
                break;
            }
            case fl::LinguisticTerm::MF_FUNCTION:
            {
                const fl::FunctionTerm* dy_term =
                        dynamic_cast<const fl::FunctionTerm*> (&term);
                fl::FuzzyException::Assert(FL_AT, dy_term != NULL);
                getUi().led_fx->setText(dy_term->infixFunction().c_str());
                getUi().dsx_fx_min->setValue(dy_term->minimum());
                getUi().dsx_fx_max->setValue(dy_term->maximum());
                getUi().tbw_lterms->setCurrentIndex(MF_FUNCTION);
                break;
            }
            default:
                FL_LOG("unknown type: " << term.type())
                        ;
        }
    }

    fl::LinguisticTerm* GuiLinguisticTerm::selectedTerm() const {
        int function = getUi().tbw_lterms->currentIndex();
        switch (function) {
            case MF_TRIANGULAR:
                return new fl::TriangularTerm(_triangular);
            case MF_RECTANGULAR:
                return new fl::RectangularTerm(_rectangular);
            case MF_TRAPEZOIDAL:
                return new fl::TrapezoidalTerm(_trapezoidal);
            case MF_SHOULDER:
                return new fl::ShoulderTerm(_shoulder);
            case MF_SINGLETON:
                return new fl::SingletonTerm(_singleton);
            case MF_FUNCTION:
                return new fl::FunctionTerm(_fx);
        }
        return NULL;
    }

    void GuiLinguisticTerm::updateUi() {
        getUi().gvw_canvas->clear();
        switch (getUi().tbw_lterms->currentIndex()) {
            case MF_TRIANGULAR:
                _triangular.setName(getUi().led_name->text().toStdString());
                _triangular.setA(getUi().dsx_triangular_a->value());
                _triangular.setB(getUi().dsx_triangular_b->value());
                _triangular.setC(getUi().dsx_triangular_c->value());
                getUi().gvw_canvas->setMinimumLVar(_triangular.minimum());
                getUi().gvw_canvas->setMaximumLVar(_triangular.maximum());
                getUi().gvw_canvas->drawTerm(_triangular);

                break;
            case MF_RECTANGULAR:
                _rectangular.setName(getUi().led_name->text().toStdString());
                _rectangular.setMinimum(getUi().dsx_rectangular_min->value());
                _rectangular.setMaximum(getUi().dsx_rectangular_max->value());

                getUi().gvw_canvas->setMinimumLVar(_rectangular.minimum() - 0.25
                        * (_rectangular.maximum() - _rectangular.minimum()));
                getUi().gvw_canvas->setMaximumLVar(_rectangular.maximum() + 0.25
                        * (_rectangular.maximum() - _rectangular.minimum()));
                getUi().gvw_canvas->drawTerm(_rectangular);
                break;
            case MF_TRAPEZOIDAL:
                _trapezoidal.setName(getUi().led_name->text().toStdString());
                _trapezoidal.setA(getUi().dsx_trapezoidal_a->value());
                _trapezoidal.setB(getUi().dsx_trapezoidal_b->value());
                _trapezoidal.setC(getUi().dsx_trapezoidal_c->value());
                _trapezoidal.setD(getUi().dsx_trapezoidal_d->value());
                getUi().gvw_canvas->setMinimumLVar(_trapezoidal.minimum());
                getUi().gvw_canvas->setMaximumLVar(_trapezoidal.maximum());
                getUi().gvw_canvas->drawTerm(_trapezoidal);
                break;
            case MF_SHOULDER:
                _shoulder.setName(getUi().led_name->text().toStdString());
                _shoulder.setLeft(getUi().rbt_shoulder_left->isChecked());
                _shoulder.setMinimum(getUi().dsx_shoulder_min->value());
                _shoulder.setMaximum(getUi().dsx_shoulder_max->value());
                getUi().gvw_canvas->setMinimumLVar(_shoulder.minimum());
                getUi().gvw_canvas->setMaximumLVar(_shoulder.maximum());
                getUi().gvw_canvas->drawTerm(_shoulder);
                break;
            case MF_SINGLETON:
                _singleton.setName(getUi().led_name->text().toStdString());
                _singleton.setValue(getUi().dsx_singleton_value->value());
                getUi().gvw_canvas->setMinimumLVar(_singleton.value() - 1 - 0.95
                        * (_singleton.value() + 1 - _singleton.value() - 1));
                getUi().gvw_canvas->setMaximumLVar(_singleton.value() + 1 + 0.95
                        * (_singleton.value() + 1 - _singleton.value() - 1));
                getUi().gvw_canvas->drawTerm(_singleton);
                break;
            case MF_FUNCTION:
                _fx.setName(getUi().led_name->text().toStdString());
                _fx.setInfixFunction(getUi().led_fx->text().toStdString());
                _fx.setMinimum(getUi().dsx_fx_min->value());
                _fx.setMaximum(getUi().dsx_fx_max->value());
                if (_fx.isValid()) {
                    getUi().gvw_canvas->setMinimumLVar(_fx.minimum());
                    getUi().gvw_canvas->setMaximumLVar(_fx.maximum());
                    getUi().gvw_canvas->drawTerm(_fx);
                }
                break;
        }

    }

    void GuiLinguisticTerm::onChangeTriangularA(double a) {
        if (a > getUi().dsx_triangular_b->value()) {
            getUi().dsx_triangular_b->setValue(a);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeTriangularB(double b) {
        if (b > getUi().dsx_triangular_c->value()) {
            getUi().dsx_triangular_c->setValue(b);
        }
        if (b < getUi().dsx_triangular_a->value()) {
            getUi().dsx_triangular_a->setValue(b);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeTriangularC(double c) {
        if (c < getUi().dsx_triangular_b->value()) {
            getUi().dsx_triangular_b->setValue(c);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeRectangularMin(double min) {
        if (min > getUi().dsx_rectangular_max->value()) {
            getUi().dsx_rectangular_max->setValue(min);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeRectangularMax(double max) {
        if (max < getUi().dsx_rectangular_min->value()) {
            getUi().dsx_rectangular_min->setValue(max);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeTrapezoidalA(double a) {
        if (a > getUi().dsx_trapezoidal_b->value()) {
            getUi().dsx_trapezoidal_b->setValue(a);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeTrapezoidalB(double b) {
        if (b > getUi().dsx_trapezoidal_c->value()) {
            getUi().dsx_trapezoidal_c->setValue(b);
        }
        if (b < getUi().dsx_trapezoidal_a->value()) {
            getUi().dsx_trapezoidal_a->setValue(b);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeTrapezoidalC(double c) {
        if (c > getUi().dsx_trapezoidal_d->value()) {
            getUi().dsx_trapezoidal_d->setValue(c);
        }
        if (c < getUi().dsx_trapezoidal_b->value()) {
            getUi().dsx_trapezoidal_b->setValue(c);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeTrapezoidalD(double d) {
        if (d < getUi().dsx_trapezoidal_c->value()) {
            getUi().dsx_trapezoidal_c->setValue(d);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeShoulderMin(double min) {
        if (min > getUi().dsx_shoulder_max->value()) {
            getUi().dsx_shoulder_max->setValue(min);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeShoulderMax(double max) {
        if (max < getUi().dsx_shoulder_min->value()) {
            getUi().dsx_shoulder_min->setValue(max);
        }
        updateUi();
    }

    void GuiLinguisticTerm::onChangeShoulderSide() {
        _shoulder.setLeft(getUi().rbt_shoulder_left->isChecked());
        updateUi();
    }

    void GuiLinguisticTerm::onChangeSingletonValue(double value) {
        (void) value;
        updateUi();
    }

    void GuiLinguisticTerm::onChangeFxFunction(const QString& fx) {
        _fx.setInfixFunction(fx.toStdString());
        QPalette palette;
        palette.setColor(QPalette::Text, (_fx.isValid() ? Qt::green : Qt::red));
        getUi().led_fx->setPalette(palette);
        updateUi();
    }

    void GuiLinguisticTerm::onChangeFxMin(double min) {
        _fx.setMinimum(min);
        updateUi();
    }

    void GuiLinguisticTerm::onChangeFxMax(double max) {
        _fx.setMaximum(max);
        updateUi();
    }

} // namespace fl_gui
