/****************************************************************************
** Meta object code from reading C++ file 'gasgraphview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../gasgraphview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasgraphview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GasGraphView_t {
    QByteArrayData data[15];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GasGraphView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GasGraphView_t qt_meta_stringdata_GasGraphView = {
    {
QT_MOC_LITERAL(0, 0, 12), // "GasGraphView"
QT_MOC_LITERAL(1, 13, 11), // "ChangeScale"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "scale"
QT_MOC_LITERAL(4, 32, 13), // "onZoomChanged"
QT_MOC_LITERAL(5, 46, 9), // "onDrawFAT"
QT_MOC_LITERAL(6, 56, 5), // "state"
QT_MOC_LITERAL(7, 62, 9), // "onDrawMUS"
QT_MOC_LITERAL(8, 72, 9), // "onDrawVRG"
QT_MOC_LITERAL(9, 82, 9), // "onDrawALV"
QT_MOC_LITERAL(10, 92, 9), // "onDrawCKT"
QT_MOC_LITERAL(11, 102, 10), // "onCollapse"
QT_MOC_LITERAL(12, 113, 12), // "hideCollapse"
QT_MOC_LITERAL(13, 126, 11), // "scrollMoved"
QT_MOC_LITERAL(14, 138, 5) // "value"

    },
    "GasGraphView\0ChangeScale\0\0scale\0"
    "onZoomChanged\0onDrawFAT\0state\0onDrawMUS\0"
    "onDrawVRG\0onDrawALV\0onDrawCKT\0onCollapse\0"
    "hideCollapse\0scrollMoved\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GasGraphView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       4,    0,   67,    2, 0x0a /* Public */,
       5,    1,   68,    2, 0x0a /* Public */,
       7,    1,   71,    2, 0x0a /* Public */,
       8,    1,   74,    2, 0x0a /* Public */,
       9,    1,   77,    2, 0x0a /* Public */,
      10,    1,   80,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    0,   84,    2, 0x0a /* Public */,
      13,    1,   85,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void GasGraphView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GasGraphView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ChangeScale((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onZoomChanged(); break;
        case 2: _t->onDrawFAT((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onDrawMUS((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onDrawVRG((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onDrawALV((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onDrawCKT((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onCollapse(); break;
        case 8: _t->hideCollapse(); break;
        case 9: _t->scrollMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GasGraphView::staticMetaObject = { {
    QMetaObject::SuperData::link<GasViewCommon::staticMetaObject>(),
    qt_meta_stringdata_GasGraphView.data,
    qt_meta_data_GasGraphView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GasGraphView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasGraphView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GasGraphView.stringdata0))
        return static_cast<void*>(this);
    return GasViewCommon::qt_metacast(_clname);
}

int GasGraphView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GasViewCommon::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
