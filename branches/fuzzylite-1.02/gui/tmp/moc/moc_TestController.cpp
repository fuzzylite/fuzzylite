/****************************************************************************
** Meta object code from reading C++ file 'TestController.h'
**
** Created: Sat Feb 12 19:33:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/TestController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TestController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__TestController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   23,   23,   23, 0x0a,
      45,   23,   23,   23, 0x0a,
      54,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__TestController[] = {
    "fl_gui::TestController\0\0forceUpdate()\0"
    "load()\0unload()\0onInputValueChanged()\0"
};

const QMetaObject fl_gui::TestController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_fl_gui__TestController,
      qt_meta_data_fl_gui__TestController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::TestController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::TestController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::TestController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__TestController))
        return static_cast<void*>(const_cast< TestController*>(this));
    return QObject::qt_metacast(_clname);
}

int fl_gui::TestController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: forceUpdate(); break;
        case 1: load(); break;
        case 2: unload(); break;
        case 3: onInputValueChanged(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void fl_gui::TestController::forceUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
