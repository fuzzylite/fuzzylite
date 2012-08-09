/****************************************************************************
** Meta object code from reading C++ file 'GuiLinguisticVariable.h'
**
** Created: Sat Feb 12 19:33:40 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/GuiLinguisticVariable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiLinguisticVariable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__GuiLinguisticVariable[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   30,   30,   30, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   30,   30,   30, 0x0a,
      53,   30,   30,   30, 0x09,
      62,   30,   30,   30, 0x09,
      86,   30,   30,   30, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__GuiLinguisticVariable[] = {
    "fl_gui::GuiLinguisticVariable\0\0"
    "onUpdateUi()\0update()\0accept()\0"
    "onChangeTermSelection()\0"
    "onDoubleClickDiscreteItem(QListWidgetItem*)\0"
};

const QMetaObject fl_gui::GuiLinguisticVariable::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_fl_gui__GuiLinguisticVariable,
      qt_meta_data_fl_gui__GuiLinguisticVariable, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::GuiLinguisticVariable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::GuiLinguisticVariable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::GuiLinguisticVariable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__GuiLinguisticVariable))
        return static_cast<void*>(const_cast< GuiLinguisticVariable*>(this));
    return QDialog::qt_metacast(_clname);
}

int fl_gui::GuiLinguisticVariable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onUpdateUi(); break;
        case 1: update(); break;
        case 2: accept(); break;
        case 3: onChangeTermSelection(); break;
        case 4: onDoubleClickDiscreteItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void fl_gui::GuiLinguisticVariable::onUpdateUi()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
