/****************************************************************************
** Meta object code from reading C++ file 'calculatorUI.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../calculatorUI.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calculatorUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_calculatorUI_t {
    QByteArrayData data[15];
    char stringdata0[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_calculatorUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_calculatorUI_t qt_meta_stringdata_calculatorUI = {
    {
QT_MOC_LITERAL(0, 0, 12), // "calculatorUI"
QT_MOC_LITERAL(1, 13, 9), // "on_numBtn"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 17), // "on_bracketLeftBtn"
QT_MOC_LITERAL(4, 42, 18), // "on_bracketRightBtn"
QT_MOC_LITERAL(5, 61, 15), // "on_backspaceBtn"
QT_MOC_LITERAL(6, 77, 11), // "on_clearBtn"
QT_MOC_LITERAL(7, 89, 14), // "on_divisionBtn"
QT_MOC_LITERAL(8, 104, 20), // "on_multiplicationBtn"
QT_MOC_LITERAL(9, 125, 11), // "on_minusBtn"
QT_MOC_LITERAL(10, 137, 9), // "on_dotBtn"
QT_MOC_LITERAL(11, 147, 12), // "on_equalsBtn"
QT_MOC_LITERAL(12, 160, 10), // "on_plusBtn"
QT_MOC_LITERAL(13, 171, 9), // "on_expBtn"
QT_MOC_LITERAL(14, 181, 9) // "on_indBtn"

    },
    "calculatorUI\0on_numBtn\0\0on_bracketLeftBtn\0"
    "on_bracketRightBtn\0on_backspaceBtn\0"
    "on_clearBtn\0on_divisionBtn\0"
    "on_multiplicationBtn\0on_minusBtn\0"
    "on_dotBtn\0on_equalsBtn\0on_plusBtn\0"
    "on_expBtn\0on_indBtn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_calculatorUI[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    0,   88,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void calculatorUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<calculatorUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_numBtn(); break;
        case 1: _t->on_bracketLeftBtn(); break;
        case 2: _t->on_bracketRightBtn(); break;
        case 3: _t->on_backspaceBtn(); break;
        case 4: _t->on_clearBtn(); break;
        case 5: _t->on_divisionBtn(); break;
        case 6: _t->on_multiplicationBtn(); break;
        case 7: _t->on_minusBtn(); break;
        case 8: _t->on_dotBtn(); break;
        case 9: _t->on_equalsBtn(); break;
        case 10: _t->on_plusBtn(); break;
        case 11: _t->on_expBtn(); break;
        case 12: _t->on_indBtn(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject calculatorUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_calculatorUI.data,
    qt_meta_data_calculatorUI,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *calculatorUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *calculatorUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_calculatorUI.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int calculatorUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
