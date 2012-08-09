/****************************************************************************
** Meta object code from reading C++ file 'RuleController.h'
**
** Created: Sat Feb 12 19:33:47 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/RuleController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RuleController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__RuleController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x09,
      47,   23,   23,   23, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__RuleController[] = {
    "fl_gui::RuleController\0\0onClickGenerateRules()\0"
    "onClickCheckSpell()\0"
};

const QMetaObject fl_gui::RuleController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_fl_gui__RuleController,
      qt_meta_data_fl_gui__RuleController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::RuleController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::RuleController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::RuleController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__RuleController))
        return static_cast<void*>(const_cast< RuleController*>(this));
    return QObject::qt_metacast(_clname);
}

int fl_gui::RuleController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onClickGenerateRules(); break;
        case 1: onClickCheckSpell(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
