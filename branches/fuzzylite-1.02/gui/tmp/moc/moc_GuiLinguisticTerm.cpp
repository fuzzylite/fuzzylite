/****************************************************************************
** Meta object code from reading C++ file 'GuiLinguisticTerm.h'
**
** Created: Sat Feb 12 19:33:41 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/GuiLinguisticTerm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiLinguisticTerm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__GuiLinguisticTerm[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x0a,
      40,   38,   26,   26, 0x09,
      70,   68,   26,   26, 0x09,
     100,   98,   26,   26, 0x09,
     132,  128,   26,   26, 0x09,
     167,  163,   26,   26, 0x09,
     198,   38,   26,   26, 0x09,
     227,   68,   26,   26, 0x09,
     256,   98,   26,   26, 0x09,
     287,  285,   26,   26, 0x09,
     316,  128,   26,   26, 0x09,
     344,  163,   26,   26, 0x09,
     372,   26,   26,   26, 0x09,
     401,  395,   26,   26, 0x09,
     435,  432,   26,   26, 0x09,
     463,  128,   26,   26, 0x09,
     485,  163,   26,   26, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__GuiLinguisticTerm[] = {
    "fl_gui::GuiLinguisticTerm\0\0updateUi()\0"
    "a\0onChangeTriangularA(double)\0b\0"
    "onChangeTriangularB(double)\0c\0"
    "onChangeTriangularC(double)\0min\0"
    "onChangeRectangularMin(double)\0max\0"
    "onChangeRectangularMax(double)\0"
    "onChangeTrapezoidalA(double)\0"
    "onChangeTrapezoidalB(double)\0"
    "onChangeTrapezoidalC(double)\0d\0"
    "onChangeTrapezoidalD(double)\0"
    "onChangeShoulderMin(double)\0"
    "onChangeShoulderMax(double)\0"
    "onChangeShoulderSide()\0value\0"
    "onChangeSingletonValue(double)\0fx\0"
    "onChangeFxFunction(QString)\0"
    "onChangeFxMin(double)\0onChangeFxMax(double)\0"
};

const QMetaObject fl_gui::GuiLinguisticTerm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_fl_gui__GuiLinguisticTerm,
      qt_meta_data_fl_gui__GuiLinguisticTerm, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::GuiLinguisticTerm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::GuiLinguisticTerm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::GuiLinguisticTerm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__GuiLinguisticTerm))
        return static_cast<void*>(const_cast< GuiLinguisticTerm*>(this));
    return QDialog::qt_metacast(_clname);
}

int fl_gui::GuiLinguisticTerm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateUi(); break;
        case 1: onChangeTriangularA((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: onChangeTriangularB((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: onChangeTriangularC((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: onChangeRectangularMin((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: onChangeRectangularMax((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: onChangeTrapezoidalA((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: onChangeTrapezoidalB((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: onChangeTrapezoidalC((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: onChangeTrapezoidalD((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: onChangeShoulderMin((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: onChangeShoulderMax((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: onChangeShoulderSide(); break;
        case 13: onChangeSingletonValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: onChangeFxFunction((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: onChangeFxMin((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: onChangeFxMax((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
