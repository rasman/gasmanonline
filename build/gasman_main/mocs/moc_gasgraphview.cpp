/****************************************************************************
** Meta object code from reading C++ file 'gasgraphview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gasman_main/gasgraphview.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasgraphview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12GasGraphViewE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN12GasGraphViewE = QtMocHelpers::stringData(
    "GasGraphView",
    "ChangeScale",
    "",
    "scale",
    "onZoomChanged",
    "onDrawFAT",
    "state",
    "onDrawMUS",
    "onDrawVRG",
    "onDrawALV",
    "onDrawCKT",
    "onCollapse",
    "hideCollapse",
    "scrollMoved",
    "value"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN12GasGraphViewE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x0a,    1 /* Public */,
       4,    0,   77,    2, 0x0a,    3 /* Public */,
       5,    1,   78,    2, 0x0a,    4 /* Public */,
       7,    1,   81,    2, 0x0a,    6 /* Public */,
       8,    1,   84,    2, 0x0a,    8 /* Public */,
       9,    1,   87,    2, 0x0a,   10 /* Public */,
      10,    1,   90,    2, 0x0a,   12 /* Public */,
      11,    0,   93,    2, 0x0a,   14 /* Public */,
      12,    0,   94,    2, 0x0a,   15 /* Public */,
      13,    1,   95,    2, 0x08,   16 /* Private */,

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

Q_CONSTINIT const QMetaObject GasGraphView::staticMetaObject = { {
    QMetaObject::SuperData::link<GasViewCommon::staticMetaObject>(),
    qt_meta_stringdata_ZN12GasGraphViewE.offsetsAndSizes,
    qt_meta_data_ZN12GasGraphViewE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN12GasGraphViewE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GasGraphView, std::true_type>,
        // method 'ChangeScale'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onZoomChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDrawFAT'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDrawMUS'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDrawVRG'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDrawALV'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDrawCKT'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onCollapse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'hideCollapse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'scrollMoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void GasGraphView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GasGraphView *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ChangeScale((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->onZoomChanged(); break;
        case 2: _t->onDrawFAT((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->onDrawMUS((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onDrawVRG((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onDrawALV((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onDrawCKT((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onCollapse(); break;
        case 8: _t->hideCollapse(); break;
        case 9: _t->scrollMoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *GasGraphView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasGraphView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN12GasGraphViewE.stringdata0))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
