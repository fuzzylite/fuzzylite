/****************************************************************************
** Meta object code from reading C++ file 'GuiGrapher.h'
**
** Created: Sat Feb 12 19:33:42 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/GuiGrapher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiGrapher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__GuiGrapher[] = {

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
      20,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   19,   19,   19, 0x0a,
      61,   52,   19,   19, 0x09,
      86,   19,   19,   19, 0x09,
     107,  105,   19,   19, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__GuiGrapher[] = {
    "fl_gui::GuiGrapher\0\0onChangeInputValue()\0"
    "updateUi()\0position\0onChangeSliderValue(int)\0"
    "onEditInputValue()\0e\0"
    "mouseDoubleClickEvent(QMouseEvent*)\0"
};

const QMetaObject fl_gui::GuiGrapher::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_fl_gui__GuiGrapher,
      qt_meta_data_fl_gui__GuiGrapher, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::GuiGrapher::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::GuiGrapher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::GuiGrapher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__GuiGrapher))
        return static_cast<void*>(const_cast< GuiGrapher*>(this));
    return QWidget::qt_metacast(_clname);
}

int fl_gui::GuiGrapher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onChangeInputValue(); break;
        case 1: updateUi(); break;
        case 2: onChangeSliderValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: onEditInputValue(); break;
        case 4: mouseDoubleClickEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void fl_gui::GuiGrapher::onChangeInputValue()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
