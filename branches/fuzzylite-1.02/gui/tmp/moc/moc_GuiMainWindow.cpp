/****************************************************************************
** Meta object code from reading C++ file 'GuiMainWindow.h'
**
** Created: Sat Feb 12 19:33:44 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/GuiMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__GuiMainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x09,
      50,   43,   22,   22, 0x09,
      74,   22,   22,   22, 0x09,
      99,   22,   22,   22, 0x09,
     130,  125,   22,   22, 0x09,
     171,  125,   22,   22, 0x09,
     222,  213,   22,   22, 0x09,
     244,  213,   22,   22, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__GuiMainWindow[] = {
    "fl_gui::GuiMainWindow\0\0onShowPreferences()\0"
    "result\0onClosePreferences(int)\0"
    "onChangeInputSelection()\0"
    "onChangeOutputSelection()\0item\0"
    "onDoubleClickInputItem(QListWidgetItem*)\0"
    "onDoubleClickOutputItem(QListWidgetItem*)\0"
    "selected\0onSelectTestRule(int)\0"
    "onSelectTestActivation(int)\0"
};

const QMetaObject fl_gui::GuiMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_fl_gui__GuiMainWindow,
      qt_meta_data_fl_gui__GuiMainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::GuiMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::GuiMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::GuiMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__GuiMainWindow))
        return static_cast<void*>(const_cast< GuiMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int fl_gui::GuiMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onShowPreferences(); break;
        case 1: onClosePreferences((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: onChangeInputSelection(); break;
        case 3: onChangeOutputSelection(); break;
        case 4: onDoubleClickInputItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: onDoubleClickOutputItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: onSelectTestRule((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: onSelectTestActivation((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
