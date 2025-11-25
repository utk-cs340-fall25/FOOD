/****************************************************************************
** Meta object code from reading C++ file 'ShoppingListWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../mainproject/ShoppingListWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ShoppingListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ShoppingListWidget_t {
    QByteArrayData data[16];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ShoppingListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ShoppingListWidget_t qt_meta_stringdata_ShoppingListWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ShoppingListWidget"
QT_MOC_LITERAL(1, 19, 24), // "onRecipeSelectionChanged"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 62, 7), // "current"
QT_MOC_LITERAL(5, 70, 8), // "previous"
QT_MOC_LITERAL(6, 79, 25), // "onIngredientDoubleClicked"
QT_MOC_LITERAL(7, 105, 4), // "item"
QT_MOC_LITERAL(8, 110, 22), // "addSelectedIngredients"
QT_MOC_LITERAL(9, 133, 17), // "addAllIngredients"
QT_MOC_LITERAL(10, 151, 13), // "addCustomItem"
QT_MOC_LITERAL(11, 165, 19), // "showItemContextMenu"
QT_MOC_LITERAL(12, 185, 3), // "pos"
QT_MOC_LITERAL(13, 189, 11), // "setPriority"
QT_MOC_LITERAL(14, 201, 10), // "SLPriority"
QT_MOC_LITERAL(15, 212, 1) // "p"

    },
    "ShoppingListWidget\0onRecipeSelectionChanged\0"
    "\0QListWidgetItem*\0current\0previous\0"
    "onIngredientDoubleClicked\0item\0"
    "addSelectedIngredients\0addAllIngredients\0"
    "addCustomItem\0showItemContextMenu\0pos\0"
    "setPriority\0SLPriority\0p"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShoppingListWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x08 /* Private */,
       6,    1,   54,    2, 0x08 /* Private */,
       8,    0,   57,    2, 0x08 /* Private */,
       9,    0,   58,    2, 0x08 /* Private */,
      10,    0,   59,    2, 0x08 /* Private */,
      11,    1,   60,    2, 0x08 /* Private */,
      13,    2,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, 0x80000000 | 3,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 14,    7,   15,

       0        // eod
};

void ShoppingListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ShoppingListWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onRecipeSelectionChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: _t->onIngredientDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->addSelectedIngredients(); break;
        case 3: _t->addAllIngredients(); break;
        case 4: _t->addCustomItem(); break;
        case 5: _t->showItemContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 6: _t->setPriority((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< SLPriority(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ShoppingListWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ShoppingListWidget.data,
    qt_meta_data_ShoppingListWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ShoppingListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShoppingListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ShoppingListWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ShoppingListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
