/****************************************************************************
** Meta object code from reading C++ file 'customwidgetplugin.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gasgauge/customwidgetplugin.h"
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
struct qt_meta_tag_ZN14GasGaugePluginE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN14GasGaugePluginE = QtMocHelpers::stringData(
    "GasGaugePlugin"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN14GasGaugePluginE[] = {

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

Q_CONSTINIT const QMetaObject GasGaugePlugin::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN14GasGaugePluginE.offsetsAndSizes,
    qt_meta_data_ZN14GasGaugePluginE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN14GasGaugePluginE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GasGaugePlugin, std::true_type>
    >,
    nullptr
} };

void GasGaugePlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GasGaugePlugin *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *GasGaugePlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasGaugePlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN14GasGaugePluginE.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QDesignerCustomWidgetInterface"))
        return static_cast< QDesignerCustomWidgetInterface*>(this);
    if (!strcmp(_clname, "org.qt-project.QDesignerCustomWidgetInterface"))
        return static_cast< QDesignerCustomWidgetInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int GasGaugePlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaDataV2_GasGaugePlugin[] = {
    0xbf, 
    // "IID"
    0x02,  0x76,  'g',  'a',  's',  'm',  'a',  'n', 
    '.',  'g',  'a',  's',  'g',  'a',  'u',  'g', 
    'e',  '_',  'p',  'l',  'u',  'g',  'i',  'n', 
    // "className"
    0x03,  0x6e,  'G',  'a',  's',  'G',  'a',  'u', 
    'g',  'e',  'P',  'l',  'u',  'g',  'i',  'n', 
    // "MetaData"
    0x04,  0xa1,  0x64,  'K',  'e',  'y',  's',  0x81, 
    0x74,  'g',  'a',  's',  'g',  'a',  'u',  'g', 
    'e',  '_',  'p',  'l',  'u',  'g',  'i',  'n', 
    '.',  'j',  's',  'o',  'n', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN_V2(GasGaugePlugin, GasGaugePlugin, qt_pluginMetaDataV2_GasGaugePlugin)
#else
QT_PLUGIN_METADATA_SECTION
Q_CONSTINIT static constexpr unsigned char qt_pluginMetaData_GasGaugePlugin[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x76,  'g',  'a',  's',  'm',  'a',  'n', 
    '.',  'g',  'a',  's',  'g',  'a',  'u',  'g', 
    'e',  '_',  'p',  'l',  'u',  'g',  'i',  'n', 
    // "className"
    0x03,  0x6e,  'G',  'a',  's',  'G',  'a',  'u', 
    'g',  'e',  'P',  'l',  'u',  'g',  'i',  'n', 
    // "MetaData"
    0x04,  0xa1,  0x64,  'K',  'e',  'y',  's',  0x81, 
    0x74,  'g',  'a',  's',  'g',  'a',  'u',  'g', 
    'e',  '_',  'p',  'l',  'u',  'g',  'i',  'n', 
    '.',  'j',  's',  'o',  'n', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(GasGaugePlugin, GasGaugePlugin)
#endif  // QT_MOC_EXPORT_PLUGIN_V2

QT_WARNING_POP
