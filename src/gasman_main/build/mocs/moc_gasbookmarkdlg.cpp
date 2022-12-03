/****************************************************************************
** Meta object code from reading C++ file 'gasbookmarkdlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../gasbookmarkdlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasbookmarkdlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GasBookmarkDlg_t {
    QByteArrayData data[13];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GasBookmarkDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GasBookmarkDlg_t qt_meta_stringdata_GasBookmarkDlg = {
    {
QT_MOC_LITERAL(0, 0, 14), // "GasBookmarkDlg"
QT_MOC_LITERAL(1, 15, 4), // "exec"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11), // "addBookmark"
QT_MOC_LITERAL(4, 33, 9), // "addTarget"
QT_MOC_LITERAL(5, 43, 14), // "deleteBookmark"
QT_MOC_LITERAL(6, 58, 12), // "deleteTarget"
QT_MOC_LITERAL(7, 71, 18), // "deleteAllBookmarks"
QT_MOC_LITERAL(8, 90, 16), // "deleteAllTargets"
QT_MOC_LITERAL(9, 107, 13), // "saveBookmarks"
QT_MOC_LITERAL(10, 121, 15), // "defaultBookmark"
QT_MOC_LITERAL(11, 137, 13), // "defaultTarget"
QT_MOC_LITERAL(12, 151, 9) // "defaultOk"

    },
    "GasBookmarkDlg\0exec\0\0addBookmark\0"
    "addTarget\0deleteBookmark\0deleteTarget\0"
    "deleteAllBookmarks\0deleteAllTargets\0"
    "saveBookmarks\0defaultBookmark\0"
    "defaultTarget\0defaultOk"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GasBookmarkDlg[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    0,   71,    2, 0x0a /* Public */,
       5,    0,   72,    2, 0x0a /* Public */,
       6,    0,   73,    2, 0x0a /* Public */,
       7,    0,   74,    2, 0x0a /* Public */,
       8,    0,   75,    2, 0x0a /* Public */,
       9,    0,   76,    2, 0x0a /* Public */,
      10,    0,   77,    2, 0x0a /* Public */,
      11,    0,   78,    2, 0x0a /* Public */,
      12,    0,   79,    2, 0x0a /* Public */,

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

void GasBookmarkDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GasBookmarkDlg *>(_o);
        (void)_t;
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

QT_INIT_METAOBJECT const QMetaObject GasBookmarkDlg::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_GasBookmarkDlg.data,
    qt_meta_data_GasBookmarkDlg,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GasBookmarkDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasBookmarkDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GasBookmarkDlg.stringdata0))
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
