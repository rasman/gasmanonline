/****************************************************************************
** Meta object code from reading C++ file 'gasbookmarkdlg.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gasman_main/gasbookmarkdlg.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasbookmarkdlg.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14GasBookmarkDlgE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN14GasBookmarkDlgE = QtMocHelpers::stringData(
    "GasBookmarkDlg",
    "exec",
    "",
    "addBookmark",
    "addTarget",
    "deleteBookmark",
    "deleteTarget",
    "deleteAllBookmarks",
    "deleteAllTargets",
    "saveBookmarks",
    "defaultBookmark",
    "defaultTarget",
    "defaultOk"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN14GasBookmarkDlgE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x0a,    1 /* Public */,
       3,    0,   81,    2, 0x0a,    2 /* Public */,
       4,    0,   82,    2, 0x0a,    3 /* Public */,
       5,    0,   83,    2, 0x0a,    4 /* Public */,
       6,    0,   84,    2, 0x0a,    5 /* Public */,
       7,    0,   85,    2, 0x0a,    6 /* Public */,
       8,    0,   86,    2, 0x0a,    7 /* Public */,
       9,    0,   87,    2, 0x0a,    8 /* Public */,
      10,    0,   88,    2, 0x0a,    9 /* Public */,
      11,    0,   89,    2, 0x0a,   10 /* Public */,
      12,    0,   90,    2, 0x0a,   11 /* Public */,

 // slots: parameters
    QMetaType::Int,
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

Q_CONSTINIT const QMetaObject GasBookmarkDlg::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ZN14GasBookmarkDlgE.offsetsAndSizes,
    qt_meta_data_ZN14GasBookmarkDlgE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN14GasBookmarkDlgE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GasBookmarkDlg, std::true_type>,
        // method 'exec'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'addBookmark'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addTarget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteBookmark'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteTarget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteAllBookmarks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'deleteAllTargets'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveBookmarks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'defaultBookmark'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'defaultTarget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'defaultOk'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void GasBookmarkDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GasBookmarkDlg *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = _t->exec();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->addBookmark(); break;
        case 2: _t->addTarget(); break;
        case 3: _t->deleteBookmark(); break;
        case 4: _t->deleteTarget(); break;
        case 5: _t->deleteAllBookmarks(); break;
        case 6: _t->deleteAllTargets(); break;
        case 7: _t->saveBookmarks(); break;
        case 8: _t->defaultBookmark(); break;
        case 9: _t->defaultTarget(); break;
        case 10: _t->defaultOk(); break;
        default: ;
        }
    }
}

const QMetaObject *GasBookmarkDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasBookmarkDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN14GasBookmarkDlgE.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int GasBookmarkDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
