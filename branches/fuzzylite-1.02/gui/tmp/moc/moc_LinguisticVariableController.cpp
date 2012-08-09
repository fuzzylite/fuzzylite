/****************************************************************************
** Meta object code from reading C++ file 'LinguisticVariableController.h'
**
** Created: Sat Feb 12 19:33:49 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/LinguisticVariableController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LinguisticVariableController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__LinguisticVariableController[] = {

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
      38,   37,   37,   37, 0x09,
      55,   37,   37,   37, 0x09,
      73,   37,   37,   37, 0x09,
      93,   37,   37,   37, 0x09,
     109,   37,   37,   37, 0x09,
     131,   37,   37,   37, 0x09,
     151,  146,   37,   37, 0x09,
     187,   37,   37,   37, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__LinguisticVariableController[] = {
    "fl_gui::LinguisticVariableController\0"
    "\0onClickAddTerm()\0onClickEditTerm()\0"
    "onClickRemoveTerm()\0onClickWizard()\0"
    "onChangeName(QString)\0onChangeTerm()\0"
    "item\0onDoubleClickTerm(QListWidgetItem*)\0"
    "draw()\0"
};

const QMetaObject fl_gui::LinguisticVariableController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_fl_gui__LinguisticVariableController,
      qt_meta_data_fl_gui__LinguisticVariableController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::LinguisticVariableController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::LinguisticVariableController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::LinguisticVariableController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__LinguisticVariableController))
        return static_cast<void*>(const_cast< LinguisticVariableController*>(this));
    return QObject::qt_metacast(_clname);
}

int fl_gui::LinguisticVariableController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onClickAddTerm(); break;
        case 1: onClickEditTerm(); break;
        case 2: onClickRemoveTerm(); break;
        case 3: onClickWizard(); break;
        case 4: onChangeName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: onChangeTerm(); break;
        case 6: onDoubleClickTerm((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: draw(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
