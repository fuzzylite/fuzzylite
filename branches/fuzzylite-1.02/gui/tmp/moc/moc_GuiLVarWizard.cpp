/****************************************************************************
** Meta object code from reading C++ file 'GuiLVarWizard.h'
**
** Created: Sat Feb 12 19:33:41 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/GuiLVarWizard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiLVarWizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__GuiLVarWizard[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   23,   22,   22, 0x09,
      55,   51,   22,   22, 0x09,
      79,   22,   22,   22, 0x09,
     101,   22,   22,   22, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__GuiLVarWizard[] = {
    "fl_gui::GuiLVarWizard\0\0min\0"
    "onChangeMinimum(double)\0max\0"
    "onChangeMaximum(double)\0onClickCreateLabels()\0"
    "accept()\0"
};

const QMetaObject fl_gui::GuiLVarWizard::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_fl_gui__GuiLVarWizard,
      qt_meta_data_fl_gui__GuiLVarWizard, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::GuiLVarWizard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::GuiLVarWizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::GuiLVarWizard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__GuiLVarWizard))
        return static_cast<void*>(const_cast< GuiLVarWizard*>(this));
    return QDialog::qt_metacast(_clname);
}

int fl_gui::GuiLVarWizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onChangeMinimum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: onChangeMaximum((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: onClickCreateLabels(); break;
        case 3: accept(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
