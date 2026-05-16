/****************************************************************************
** Meta object code from reading C++ file 'gasgauge.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gasgauge/gasgauge.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasgauge.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8GasGaugeE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN8GasGaugeE = QtMocHelpers::stringData(
    "GasGauge",
    "FireGasgaugeStart",
    "",
    "FireGasgaugeChange",
    "Value",
    "FireGasgaugeStop",
    "Max",
    "TickEvery",
    "LTickEvery",
    "BarColor"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN8GasGaugeE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       5,   37, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    6 /* Public */,
       3,    1,   33,    2, 0x06,    7 /* Public */,
       5,    0,   36,    2, 0x06,    9 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Short,    4,
    QMetaType::Void,

 // properties: name, type, flags, notifyId, revision
       6, QMetaType::Int, 0x00015003, uint(-1), 0,
       4, QMetaType::Int, 0x00015003, uint(-1), 0,
       7, QMetaType::Int, 0x00015003, uint(-1), 0,
       8, QMetaType::Int, 0x00015003, uint(-1), 0,
       9, QMetaType::QColor, 0x00015003, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject GasGauge::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN8GasGaugeE.offsetsAndSizes,
    qt_meta_data_ZN8GasGaugeE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN8GasGaugeE_t,
        // property 'Max'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'Value'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'TickEvery'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'LTickEvery'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'BarColor'
        QtPrivate::TypeAndForceComplete<QColor, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GasGauge, std::true_type>,
        // method 'FireGasgaugeStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'FireGasgaugeChange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<short, std::false_type>,
        // method 'FireGasgaugeStop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void GasGauge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GasGauge *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->FireGasgaugeStart(); break;
        case 1: _t->FireGasgaugeChange((*reinterpret_cast< std::add_pointer_t<short>>(_a[1]))); break;
        case 2: _t->FireGasgaugeStop(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (GasGauge::*)();
            if (_q_method_type _q_method = &GasGauge::FireGasgaugeStart; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (GasGauge::*)(short );
            if (_q_method_type _q_method = &GasGauge::FireGasgaugeChange; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (GasGauge::*)();
            if (_q_method_type _q_method = &GasGauge::FireGasgaugeStop; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->GetMax(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->GetValue(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->GetTickEvery(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->GetLTickEvery(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = _t->GetBarColor(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->SetMax(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->SetValue(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->SetTickEvery(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->SetLTickEvery(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->SetBarColor(*reinterpret_cast< QColor*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *GasGauge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasGauge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN8GasGaugeE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GasGauge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void GasGauge::FireGasgaugeStart()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GasGauge::FireGasgaugeChange(short _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GasGauge::FireGasgaugeStop()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
