/****************************************************************************
** Meta object code from reading C++ file 'gascontrol.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gasman_main/gascontrol.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gascontrol.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10GasControlE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10GasControlE = QtMocHelpers::stringData(
    "GasControl",
    "activateChild",
    "",
    "QWidget*",
    "widget",
    "addAgentView",
    "GasDoc*",
    "doc",
    "nGas",
    "GasViewType",
    "viewType",
    "activateChildWindows",
    "GasChildren",
    "children",
    "removeChildWindow",
    "obj",
    "updateControlVals",
    "setWeight",
    "weight",
    "setCircuit",
    "circuit",
    "setSpeed",
    "speed",
    "setState",
    "AState",
    "theState",
    "setTitle",
    "title",
    "setAgent",
    "agent",
    "changeAgent",
    "agentColorsChanged",
    "clearAllRequested",
    "updateAgentWidget",
    "QTableWidgetItem*",
    "item",
    "updateWeight",
    "updateCircuit",
    "updateSpeed",
    "pauseSim",
    "newViewRequested",
    "deleteAgentRequested",
    "bringToFrontRequested",
    "removeAgent",
    "gasNum",
    "moveUpRequested",
    "moveDownRequested",
    "swapRows",
    "old",
    "notSoOld",
    "updateAgentWidgetState",
    "enable",
    "setCost",
    "runSim",
    "onAFAPButton",
    "onSelectChanged",
    "currentRow"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10GasControlE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  200,    2, 0x06,    1 /* Public */,
       5,    3,  203,    2, 0x06,    3 /* Public */,
      11,    1,  210,    2, 0x06,    7 /* Public */,
      14,    1,  213,    2, 0x06,    9 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      16,    0,  216,    2, 0x0a,   11 /* Public */,
      17,    1,  217,    2, 0x0a,   12 /* Public */,
      19,    1,  220,    2, 0x0a,   14 /* Public */,
      21,    1,  223,    2, 0x0a,   16 /* Public */,
      23,    1,  226,    2, 0x0a,   18 /* Public */,
      26,    1,  229,    2, 0x0a,   20 /* Public */,
      28,    2,  232,    2, 0x0a,   22 /* Public */,
      30,    2,  237,    2, 0x08,   25 /* Private */,
      31,    0,  242,    2, 0x08,   28 /* Private */,
      32,    0,  243,    2, 0x08,   29 /* Private */,
      33,    1,  244,    2, 0x08,   30 /* Private */,
      36,    0,  247,    2, 0x08,   32 /* Private */,
      37,    1,  248,    2, 0x08,   33 /* Private */,
      38,    1,  251,    2, 0x08,   35 /* Private */,
      39,    0,  254,    2, 0x08,   37 /* Private */,
      40,    0,  255,    2, 0x08,   38 /* Private */,
      41,    0,  256,    2, 0x08,   39 /* Private */,
      42,    0,  257,    2, 0x08,   40 /* Private */,
      43,    1,  258,    2, 0x08,   41 /* Private */,
      45,    0,  261,    2, 0x08,   43 /* Private */,
      46,    0,  262,    2, 0x08,   44 /* Private */,
      47,    3,  263,    2, 0x08,   45 /* Private */,
      50,    1,  270,    2, 0x08,   49 /* Private */,
      52,    0,  273,    2, 0x08,   51 /* Private */,
      53,    0,  274,    2, 0x08,   52 /* Private */,
      54,    0,  275,    2, 0x08,   53 /* Private */,
      55,    4,  276,    2, 0x08,   54 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int, 0x80000000 | 9,    7,    8,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::QObjectStar,   15,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    8,   29,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    8,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 34,   35,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   44,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 34,   48,   49,   35,
    QMetaType::Void, QMetaType::Bool,   51,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   56,    2,    2,    2,

       0        // eod
};

Q_CONSTINIT const QMetaObject GasControl::staticMetaObject = { {
    QMetaObject::SuperData::link<GasViewCommon::staticMetaObject>(),
    qt_meta_stringdata_ZN10GasControlE.offsetsAndSizes,
    qt_meta_data_ZN10GasControlE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10GasControlE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GasControl, std::true_type>,
        // method 'activateChild'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        // method 'addAgentView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasDoc *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasViewType, std::false_type>,
        // method 'activateChildWindows'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GasChildren &, std::false_type>,
        // method 'removeChildWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QObject *, std::false_type>,
        // method 'updateControlVals'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setWeight'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'setCircuit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'setSpeed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'setState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const AState &, std::false_type>,
        // method 'setTitle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'setAgent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'changeAgent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'agentColorsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearAllRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateAgentWidget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTableWidgetItem *, std::false_type>,
        // method 'updateWeight'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateCircuit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateSpeed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'pauseSim'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newViewRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteAgentRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'bringToFrontRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeAgent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'moveUpRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'moveDownRequested'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'swapRows'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTableWidgetItem *, std::false_type>,
        // method 'updateAgentWidgetState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setCost'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runSim'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAFAPButton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSelectChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void GasControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GasControl *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->activateChild((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 1: _t->addAgentView((*reinterpret_cast< std::add_pointer_t<GasDoc*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<GasViewType>>(_a[3]))); break;
        case 2: _t->activateChildWindows((*reinterpret_cast< std::add_pointer_t<GasChildren>>(_a[1]))); break;
        case 3: _t->removeChildWindow((*reinterpret_cast< std::add_pointer_t<QObject*>>(_a[1]))); break;
        case 4: _t->updateControlVals(); break;
        case 5: _t->setWeight((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->setCircuit((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->setSpeed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->setState((*reinterpret_cast< std::add_pointer_t<AState>>(_a[1]))); break;
        case 9: _t->setTitle((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->setAgent((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->changeAgent((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 12: _t->agentColorsChanged(); break;
        case 13: _t->clearAllRequested(); break;
        case 14: _t->updateAgentWidget((*reinterpret_cast< std::add_pointer_t<QTableWidgetItem*>>(_a[1]))); break;
        case 15: _t->updateWeight(); break;
        case 16: _t->updateCircuit((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->updateSpeed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->pauseSim(); break;
        case 19: _t->newViewRequested(); break;
        case 20: _t->deleteAgentRequested(); break;
        case 21: _t->bringToFrontRequested(); break;
        case 22: _t->removeAgent((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 23: _t->moveUpRequested(); break;
        case 24: _t->moveDownRequested(); break;
        case 25: _t->swapRows((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QTableWidgetItem*>>(_a[3]))); break;
        case 26: _t->updateAgentWidgetState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 27: _t->setCost(); break;
        case 28: _t->runSim(); break;
        case 29: _t->onAFAPButton(); break;
        case 30: _t->onSelectChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (GasControl::*)(QWidget * );
            if (_q_method_type _q_method = &GasControl::activateChild; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (GasControl::*)(GasDoc * , int , GasViewType );
            if (_q_method_type _q_method = &GasControl::addAgentView; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (GasControl::*)(const GasChildren & );
            if (_q_method_type _q_method = &GasControl::activateChildWindows; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (GasControl::*)(QObject * );
            if (_q_method_type _q_method = &GasControl::removeChildWindow; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *GasControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10GasControlE.stringdata0))
        return static_cast<void*>(this);
    return GasViewCommon::qt_metacast(_clname);
}

int GasControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GasViewCommon::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void GasControl::activateChild(QWidget * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GasControl::addAgentView(GasDoc * _t1, int _t2, GasViewType _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GasControl::activateChildWindows(const GasChildren & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GasControl::removeChildWindow(QObject * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
