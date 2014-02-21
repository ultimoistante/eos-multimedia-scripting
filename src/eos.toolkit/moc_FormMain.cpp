/****************************************************************************
** Meta object code from reading C++ file 'FormMain.h'
**
** Created: Fri Oct 30 00:37:18 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FormMain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FormMain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FormMain[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      17,    9,    9,    9, 0x08,
      37,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FormMain[] = {
    "FormMain\0\0quit()\0startScriptEditor()\0"
    "startMessagesLogInspector()\0"
};

const QMetaObject FormMain::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FormMain,
      qt_meta_data_FormMain, 0 }
};

const QMetaObject *FormMain::metaObject() const
{
    return &staticMetaObject;
}

void *FormMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FormMain))
        return static_cast<void*>(const_cast< FormMain*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FormMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: quit(); break;
        case 1: startScriptEditor(); break;
        case 2: startMessagesLogInspector(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
