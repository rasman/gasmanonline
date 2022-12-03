/****************************************************************************
** Meta object code from reading C++ file 'gasdoc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../gasdoc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasdoc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GasDoc_t {
    QByteArrayData data[18];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GasDoc_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GasDoc_t qt_meta_stringdata_GasDoc = {
    {
QT_MOC_LITERAL(0, 0, 6), // "GasDoc"
QT_MOC_LITERAL(1, 7, 13), // "updateControl"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 11), // "updatePanel"
QT_MOC_LITERAL(4, 34, 12), // "agentChanged"
QT_MOC_LITERAL(5, 47, 4), // "nGas"
QT_MOC_LITERAL(6, 52, 5), // "agent"
QT_MOC_LITERAL(7, 58, 10), // "allCleared"
QT_MOC_LITERAL(8, 69, 8), // "modified"
QT_MOC_LITERAL(9, 78, 1), // "m"
QT_MOC_LITERAL(10, 80, 12), // "stateChanged"
QT_MOC_LITERAL(11, 93, 6), // "AState"
QT_MOC_LITERAL(12, 100, 5), // "state"
QT_MOC_LITERAL(13, 106, 12), // "titleChanged"
QT_MOC_LITERAL(14, 119, 8), // "newTitle"
QT_MOC_LITERAL(15, 128, 14), // "noViewsOfAgent"
QT_MOC_LITERAL(16, 143, 18), // "agentColorsChanged"
QT_MOC_LITERAL(17, 162, 4) // "idle"

    },
    "GasDoc\0updateControl\0\0updatePanel\0"
    "agentChanged\0nGas\0agent\0allCleared\0"
    "modified\0m\0stateChanged\0AState\0state\0"
    "titleChanged\0newTitle\0noViewsOfAgent\0"
    "agentColorsChanged\0idle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GasDoc[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    2,   71,    2, 0x06 /* Public */,
       7,    0,   76,    2, 0x06 /* Public */,
       8,    1,   77,    2, 0x06 /* Public */,
       8,    0,   80,    2, 0x26 /* Public | MethodCloned */,
      10,    1,   81,    2, 0x06 /* Public */,
      13,    1,   84,    2, 0x06 /* Public */,
      15,    1,   87,    2, 0x06 /* Public */,
      16,    0,   90,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void GasDoc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GasDoc *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateControl(); break;
        case 1: _t->updatePanel(); break;
        case 2: _t->agentChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->allCleared(); break;
        case 4: _t->modified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->modified(); break;
        case 6: _t->stateChanged((*reinterpret_cast< AState(*)>(_a[1]))); break;
        case 7: _t->titleChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->noViewsOfAgent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->agentColorsChanged(); break;
        case 10: _t->idle(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GasDoc::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::updateControl)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::updatePanel)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::agentChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::allCleared)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::modified)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)(AState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::stateChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::titleChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::noViewsOfAgent)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (GasDoc::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GasDoc::agentColorsChanged)) {
                *result = 9;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GasDoc::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_GasDoc.data,
    qt_meta_data_GasDoc,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GasDoc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasDoc::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GasDoc.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GasDoc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void GasDoc::updateControl()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GasDoc::updatePanel()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GasDoc::agentChanged(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GasDoc::allCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void GasDoc::modified(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 6
void GasDoc::stateChanged(AState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void GasDoc::titleChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void GasDoc::noViewsOfAgent(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void GasDoc::agentColorsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
