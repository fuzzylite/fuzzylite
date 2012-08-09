/****************************************************************************
** Meta object code from reading C++ file 'GuiPreferences.h'
**
** Created: Sat Feb 12 19:33:43 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/GuiPreferences.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiPreferences.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__GuiPreferences[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   24,   23,   23, 0x09,
      49,   24,   23,   23, 0x09,
      68,   24,   23,   23, 0x09,
      92,   24,   23,   23, 0x09,
     117,   24,   23,   23, 0x09,
     140,   24,   23,   23, 0x09,
     165,   24,   23,   23, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__GuiPreferences[] = {
    "fl_gui::GuiPreferences\0\0index\0"
    "onChangeTNorm(int)\0onChangeSNorm(int)\0"
    "onChangeModulation(int)\0"
    "onChangeAggregation(int)\0"
    "onChangeAlgorithm(int)\0onChangeDefuzzifier(int)\0"
    "onChangeSampleSize(int)\0"
};

const QMetaObject fl_gui::GuiPreferences::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_fl_gui__GuiPreferences,
      qt_meta_data_fl_gui__GuiPreferences, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::GuiPreferences::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::GuiPreferences::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::GuiPreferences::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__GuiPreferences))
        return static_cast<void*>(const_cast< GuiPreferences*>(this));
    return QDialog::qt_metacast(_clname);
}

int fl_gui::GuiPreferences::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onChangeTNorm((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: onChangeSNorm((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: onChangeModulation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: onChangeAggregation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: onChangeAlgorithm((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: onChangeDefuzzifier((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: onChangeSampleSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
