/****************************************************************************
** Meta object code from reading C++ file 'gastabwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/gasman_main/gastabwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gastabwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GasTabWidget_t {
    QByteArrayData data[14];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GasTabWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GasTabWidget_t qt_meta_stringdata_GasTabWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "GasTabWidget"
QT_MOC_LITERAL(1, 13, 17), // "reparentChildDrop"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 11), // "QDropEvent*"
QT_MOC_LITERAL(4, 44, 5), // "event"
QT_MOC_LITERAL(5, 50, 13), // "GasTabWidget*"
QT_MOC_LITERAL(6, 64, 9), // "tabWidget"
QT_MOC_LITERAL(7, 74, 18), // "updateTabTextColor"
QT_MOC_LITERAL(8, 93, 10), // "handleDrag"
QT_MOC_LITERAL(9, 104, 11), // "tryCloseTab"
QT_MOC_LITERAL(10, 116, 9), // "updateTab"
QT_MOC_LITERAL(11, 126, 5), // "title"
QT_MOC_LITERAL(12, 132, 16), // "removeTabOnClose"
QT_MOC_LITERAL(13, 149, 17) // "currentTabChanged"

    },
    "GasTabWidget\0reparentChildDrop\0\0"
    "QDropEvent*\0event\0GasTabWidget*\0"
    "tabWidget\0updateTabTextColor\0handleDrag\0"
    "tryCloseTab\0updateTab\0title\0"
    "removeTabOnClose\0currentTabChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GasTabWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   54,    2, 0x0a /* Public */,
       8,    1,   55,    2, 0x08 /* Private */,
       9,    0,   58,    2, 0x08 /* Private */,
      10,    1,   59,    2, 0x08 /* Private */,
      12,    0,   62,    2, 0x08 /* Private */,
      13,    0,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GasTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GasTabWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->reparentChildDrop((*reinterpret_cast< QDropEvent*(*)>(_a[1])),(*reinterpret_cast< GasTabWidget*(*)>(_a[2]))); break;
        case 1: _t->updateTabTextColor(); break;
        case 2: _t->handleDrag((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->tryCloseTab(); break;
        case 4: _t->updateTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->removeTabOnClose(); break;
        case 6: _t->currentTabChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GasTabWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GasTabWidget::*)(QDropEvent * , GasTabWidget * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasTabWidget::reparentChildDrop)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GasTabWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QTabWidget::staticMetaObject>(),
    qt_meta_stringdata_GasTabWidget.data,
    qt_meta_data_GasTabWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GasTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GasTabWidget.stringdata0))
        return static_cast<void*>(this);
    return QTabWidget::qt_metacast(_clname);
}

int GasTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void GasTabWidget::reparentChildDrop(QDropEvent * _t1, GasTabWidget * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
