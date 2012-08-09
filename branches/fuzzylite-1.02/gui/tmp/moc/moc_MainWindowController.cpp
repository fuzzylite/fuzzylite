/****************************************************************************
** Meta object code from reading C++ file 'MainWindowController.h'
**
** Created: Sat Feb 12 19:33:48 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/MainWindowController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindowController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__MainWindowController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x09,
      43,   29,   29,   29, 0x09,
      58,   29,   29,   29, 0x09,
      73,   29,   29,   29, 0x09,
      88,   29,   29,   29, 0x09,
     106,   29,   29,   29, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__MainWindowController[] = {
    "fl_gui::MainWindowController\0\0"
    "resetModel()\0loadExample1()\0loadExample2()\0"
    "loadExample3()\0showSetupWindow()\0"
    "showTestWindow()\0"
};

const QMetaObject fl_gui::MainWindowController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_fl_gui__MainWindowController,
      qt_meta_data_fl_gui__MainWindowController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::MainWindowController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::MainWindowController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::MainWindowController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__MainWindowController))
        return static_cast<void*>(const_cast< MainWindowController*>(this));
    return QObject::qt_metacast(_clname);
}

int fl_gui::MainWindowController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: resetModel(); break;
        case 1: loadExample1(); break;
        case 2: loadExample2(); break;
        case 3: loadExample3(); break;
        case 4: showSetupWindow(); break;
        case 5: showTestWindow(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
