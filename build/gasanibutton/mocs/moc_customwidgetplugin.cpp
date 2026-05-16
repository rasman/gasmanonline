/****************************************************************************
** Meta object code from reading C++ file 'customwidgetplugin.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gasanibutton/customwidgetplugin.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customwidgetplugin.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18GasAniButtonPluginE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN18GasAniButtonPluginE = QtMocHelpers::stringData(
    "GasAniButtonPlugin"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN18GasAniButtonPluginE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject GasAniButtonPlugin::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN18GasAniButtonPluginE.offsetsAndSizes,
    qt_meta_data_ZN18GasAniButtonPluginE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN18GasAniButtonPluginE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GasAniButtonPlugin, std::true_type>
    >,
    nullptr
} };

void GasAniButtonPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GasAniButtonPlugin *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *GasAniButtonPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasAniButtonPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN18GasAniButtonPluginE.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QDesignerCustomWidgetInterface"))
        return static_cast< QDesignerCustomWidgetInterface*>(this);
    if (!strcmp(_clname, "org.qt-project.QDesignerCustomWidgetInterface"))
        return static_cast< QDesignerCustomWidgetInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int GasAniButtonPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaDataV2_GasAniButtonPlugin[] = {
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1a,  'g',  'a',  's',  'm',  'a', 
    'n',  '.',  'g',  'a',  's',  'a',  'n',  'i', 
    'b',  'u',  't',  't',  'o',  'n',  '_',  'p', 
    'l',  'u',  'g',  'i',  'n', 
    // "className"
    0x03,  0x72,  'G',  'a',  's',  'A',  'n',  'i', 
    'B',  'u',  't',  't',  'o',  'n',  'P',  'l', 
    'u',  'g',  'i',  'n', 
    // "MetaData"
    0x04,  0xa1,  0x64,  'K',  'e',  'y',  's',  0x81, 
    0x73,  'g',  'a',  's',  'a',  'n',  'i',  'b', 
    'u',  't',  't',  'o',  'n',  '_',  'p',  'l', 
    'u',  'g',  'i',  'n', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN_V2(GasAniButtonPlugin, GasAniButtonPlugin, qt_pluginMetaDataV2_GasAniButtonPlugin)
#else
QT_PLUGIN_METADATA_SECTION
Q_CONSTINIT static constexpr unsigned char qt_pluginMetaData_GasAniButtonPlugin[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1a,  'g',  'a',  's',  'm',  'a', 
    'n',  '.',  'g',  'a',  's',  'a',  'n',  'i', 
    'b',  'u',  't',  't',  'o',  'n',  '_',  'p', 
    'l',  'u',  'g',  'i',  'n', 
    // "className"
    0x03,  0x72,  'G',  'a',  's',  'A',  'n',  'i', 
    'B',  'u',  't',  't',  'o',  'n',  'P',  'l', 
    'u',  'g',  'i',  'n', 
    // "MetaData"
    0x04,  0xa1,  0x64,  'K',  'e',  'y',  's',  0x81, 
    0x73,  'g',  'a',  's',  'a',  'n',  'i',  'b', 
    'u',  't',  't',  'o',  'n',  '_',  'p',  'l', 
    'u',  'g',  'i',  'n', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(GasAniButtonPlugin, GasAniButtonPlugin)
#endif  // QT_MOC_EXPORT_PLUGIN_V2

QT_WARNING_POP
