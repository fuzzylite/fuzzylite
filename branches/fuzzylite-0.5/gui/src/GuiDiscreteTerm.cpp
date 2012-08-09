#include "GuiDiscreteTerm.h"

#include <QtGui/QErrorMessage>
#include <flScalar.h>
#include <StringOperator.h>
#include <QtGui/qmessagebox.h>

using namespace fuzzy_lite;
namespace fuzzy_lite_gui {

    GuiDiscreteTerm::GuiDiscreteTerm(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags), _ui(new Ui::DiscreteTerm) {

    }

    GuiDiscreteTerm::~GuiDiscreteTerm() {
        disconnect();
        delete _ui;
    }

    Ui::DiscreteTerm& GuiDiscreteTerm::getUi() const {
        return *this->_ui;
    }

    void GuiDiscreteTerm::setup() {
        getUi().setupUi(this);

        connect();
    }

    void GuiDiscreteTerm::connect() {
        QObject::connect(getUi().btn_add_triangle, SIGNAL(clicked()),
                this, SLOT(onClickAddTriangle()));
        QObject::connect(getUi().btn_add_trapezoid, SIGNAL(clicked()),
                this, SLOT(onClickAddTrapezoid()));

        QObject::connect(getUi().dsx_triangle_a, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTriangleA(double)));
        QObject::connect(getUi().dsx_triangle_b, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTriangleB(double)));

        QObject::connect(getUi().dsx_trapezoid_a, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidA(double)));
        QObject::connect(getUi().dsx_trapezoid_b, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidB(double)));
        QObject::connect(getUi().dsx_trapezoid_c, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidC(double)));
    }

    void GuiDiscreteTerm::disconnect() {
        QObject::disconnect(getUi().btn_add_triangle, SIGNAL(clicked()),
                this, SLOT(onClickAddTriangle()));
        QObject::disconnect(getUi().btn_add_trapezoid, SIGNAL(clicked()),
                this, SLOT(onClickAddTrapezoid()));

        QObject::disconnect(getUi().dsx_triangle_a, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTriangleA(double)));
        QObject::disconnect(getUi().dsx_triangle_b, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTriangleB(double)));

        QObject::disconnect(getUi().dsx_trapezoid_a, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidA(double)));
        QObject::disconnect(getUi().dsx_trapezoid_b, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidB(double)));
        QObject::disconnect(getUi().dsx_trapezoid_c, SIGNAL(valueChanged(double)),
                this, SLOT(onChangeTrapezoidC(double)));
    }

    void GuiDiscreteTerm::accept() {
        bool ok = false;
        std::string message = isValid(ok);
        if (ok) {
            done(QDialog::Accepted);
        } else {
            QMessageBox::critical(this, "Error", QString::fromStdString(message),
                    QMessageBox::Ok, QMessageBox::Ok);
        }
    }

    std::string GuiDiscreteTerm::isValid(bool& ok) {
        QString name = getUi().led_name->text();
        if (name.size() <= 0) {
            ok = false;
            return "Name must contain at least one character";
        }
        if (name.contains(' ')) {
            ok = false;
            return "Name cannot contain spaces";
        }

        QStringList coords = getUi().ptx_coords->toPlainText().split("\n", QString::SkipEmptyParts);
        if (coords.size() < 3) {
            ok = false;
            return "Discrete terms must have at least 3 xy-coords";
        }
        flScalar x, y;
        for (int i = 0; i < coords.size(); ++i) {
            QStringList xy = coords.at(i).split(":", QString::SkipEmptyParts);
            if (xy.size() != 2) {
                ok = false;
                return "Each line must have only one xy-coord formatted as x:y";
            }
            x = (flScalar) xy.at(0).toDouble(&ok);
            if (!ok) {
                return "Invalid x-coord at line " + StringOperator::IntToString(i);
            }
            y = (flScalar) xy.at(1).toDouble(&ok);
            if (!ok) {
                return "Invalid y-coord at line " + StringOperator::IntToString(i);
            }
        }
        ok = true;
        return "";
    }

    void GuiDiscreteTerm::onClickAddTriangle() {
        getUi().ptx_coords->appendPlainText(
                getUi().dsx_triangle_a->text() + ":" +
                QString::number(0.0));
        getUi().ptx_coords->appendPlainText(
                getUi().dsx_triangle_b->text() + ":" +
                QString::number(1.0));
        getUi().ptx_coords->appendPlainText(
                getUi().dsx_triangle_c->text() + ":" +
                QString::number(0.0));
    }

    void GuiDiscreteTerm::onClickAddTrapezoid() {
        getUi().ptx_coords->appendPlainText(
                getUi().dsx_trapezoid_a->text() + ":" +
                QString::number(0.0));
        getUi().ptx_coords->appendPlainText(
                getUi().dsx_trapezoid_b->text() + ":" +
                QString::number(1.0));
        getUi().ptx_coords->appendPlainText(
                getUi().dsx_trapezoid_c->text() + ":" +
                QString::number(1.0));
        getUi().ptx_coords->appendPlainText(
                getUi().dsx_trapezoid_d->text() + ":" +
                QString::number(0.0));
    }

    void GuiDiscreteTerm::onChangeTriangleA(double a) {
        if (getUi().dsx_triangle_b->value() <= a) {
            getUi().dsx_triangle_b->setValue(a);
        }
    }

    void GuiDiscreteTerm::onChangeTriangleB(double b) {
        if (getUi().dsx_triangle_c->value() <= b) {
            getUi().dsx_triangle_c->setValue(b);
        }
    }

    void GuiDiscreteTerm::onChangeTrapezoidA(double a) {
        if (getUi().dsx_trapezoid_b->value() <= a) {
            getUi().dsx_trapezoid_b->setValue(a);
        }
    }

    void GuiDiscreteTerm::onChangeTrapezoidB(double b) {
        if (getUi().dsx_trapezoid_c->value() <= b) {
            getUi().dsx_trapezoid_c->setValue(b);
        }
    }

    void GuiDiscreteTerm::onChangeTrapezoidC(double c) {
        if (getUi().dsx_trapezoid_d->value() <= c) {
            getUi().dsx_trapezoid_d->setValue(c);
        }
    }
}
