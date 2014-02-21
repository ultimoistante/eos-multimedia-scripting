/****************************************************************************
** Meta object code from reading C++ file 'FormMsgLogInspector.h'
**
** Created: Fri Oct 30 00:37:19 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FormMsgLogInspector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FormMsgLogInspector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FormMsgLogInspector[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,
      41,   35,   20,   20, 0x08,
      65,   20,   20,   20, 0x08,
      83,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FormMsgLogInspector[] = {
    "FormMsgLogInspector\0\0openLog(bool)\0"
    "index\0updateFilterValues(int)\0"
    "applyFilter(bool)\0resetFilter(bool)\0"
};

const QMetaObject FormMsgLogInspector::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FormMsgLogInspector,
      qt_meta_data_FormMsgLogInspector, 0 }
};

const QMetaObject *FormMsgLogInspector::metaObject() const
{
    return &staticMetaObject;
}

void *FormMsgLogInspector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FormMsgLogInspector))
        return static_cast<void*>(const_cast< FormMsgLogInspector*>(this));
    return QWidget::qt_metacast(_clname);
}

int FormMsgLogInspector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openLog((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: updateFilterValues((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: applyFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: resetFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
