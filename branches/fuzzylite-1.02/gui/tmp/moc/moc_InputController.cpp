/****************************************************************************
** Meta object code from reading C++ file 'InputController.h'
**
** Created: Sat Feb 12 19:33:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/InputController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InputController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__InputController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x09,
      43,   24,   24,   24, 0x09,
      64,   24,   24,   24, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__InputController[] = {
    "fl_gui::InputController\0\0onClickAddInput()\0"
    "onClickRemoveInput()\0onClickEditInput()\0"
};

const QMetaObject fl_gui::InputController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_fl_gui__InputController,
      qt_meta_data_fl_gui__InputController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::InputController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::InputController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::InputController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__InputController))
        return static_cast<void*>(const_cast< InputController*>(this));
    return QObject::qt_metacast(_clname);
}

int fl_gui::InputController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onClickAddInput(); break;
        case 1: onClickRemoveInput(); break;
        case 2: onClickEditInput(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
