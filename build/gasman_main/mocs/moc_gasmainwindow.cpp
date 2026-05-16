/****************************************************************************
** Meta object code from reading C++ file 'gasmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gasman_main/gasmainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasmainwindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13GasMainWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN13GasMainWindowE = QtMocHelpers::stringData(
    "GasMainWindow",
    "newFile",
    "",
    "subWindowActivated",
    "QMdiSubWindow*",
    "sw",
    "updateAllActions",
    "updateGasDocs",
    "removeChild",
    "obj",
    "showHelpTopic",
    "newViewOrAdd",
    "openFromContent",
    "fileName",
    "fileContent",
    "groupSubWindows",
    "pictureOnTop",
    "graphOnTop",
    "controlOnTop",
    "onTop",
    "GasViewType",
    "viewType",
    "editDescription",
    "reparentChildDrop",
    "QDropEvent*",
    "event",
    "GasTabWidget*",
    "dest",
    "reparentChild",
    "GasChildWindow*",
    "child",
    "source",
    "point",
    "tabifyChild",
    "updateActiveGas",
    "openFile",
    "saveFile",
    "saveFileAs",
    "closeFile",
    "closeAllFiles",
    "sendMail",
    "aboutGasMan",
    "print",
    "newView",
    "GasDoc*",
    "doc",
    "nGas",
    "showHelp",
    "showWorkbook",
    "setShowToolbar",
    "set",
    "selectPrintOutput",
    "setDefaultOptions",
    "openRecentFile",
    "removeDoc",
    "createViewMenu",
    "createWindowMenu",
    "updateFileMenu",
    "updateEditMenu",
    "updateAnesthesiaMenu",
    "updateSpecialMenu",
    "updateTitle",
    "updateViewMenu",
    "updateWindowMenu",
    "setActiveWindow",
    "QWidget*",
    "setActiveWindowIfNec",
    "runSimulation",
    "runAllSimulation",
    "liquidInject",
    "rewind",
    "rewindAllSimulations",
    "fastFwd",
    "zeroTimer",
    "clearAll",
    "copyData",
    "selectAll",
    "copySelection",
    "defaultAgentColors",
    "showCost",
    "showMl",
    "hideNumerics",
    "disableUptake",
    "disableReturn",
    "enableVapor",
    "enableScrolling",
    "setBookmark",
    "setBreakPoint",
    "changePatient",
    "overlay",
    "zoomin",
    "zoomout",
    "unitDose",
    "setCost",
    "adjustDel",
    "changeAgent",
    "windowStateChanged",
    "printPreview",
    "exportToPdf",
    "exportToXml",
    "exportToJson",
    "exportToHtml",
    "transform"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN13GasMainWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      87,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  536,    2, 0x0a,    1 /* Public */,
       3,    1,  537,    2, 0x0a,    2 /* Public */,
       6,    0,  540,    2, 0x0a,    4 /* Public */,
       7,    0,  541,    2, 0x0a,    5 /* Public */,
       8,    1,  542,    2, 0x0a,    6 /* Public */,
      10,    0,  545,    2, 0x0a,    8 /* Public */,
      11,    0,  546,    2, 0x08,    9 /* Private */,
      12,    2,  547,    2, 0x08,   10 /* Private */,
      15,    0,  552,    2, 0x08,   13 /* Private */,
      16,    0,  553,    2, 0x08,   14 /* Private */,
      17,    0,  554,    2, 0x08,   15 /* Private */,
      18,    0,  555,    2, 0x08,   16 /* Private */,
      19,    1,  556,    2, 0x08,   17 /* Private */,
      22,    0,  559,    2, 0x08,   19 /* Private */,
      23,    2,  560,    2, 0x08,   20 /* Private */,
      23,    1,  565,    2, 0x28,   23 /* Private | MethodCloned */,
      28,    4,  568,    2, 0x08,   25 /* Private */,
      28,    3,  577,    2, 0x28,   30 /* Private | MethodCloned */,
      28,    2,  584,    2, 0x28,   34 /* Private | MethodCloned */,
      33,    1,  589,    2, 0x08,   37 /* Private */,
      34,    0,  592,    2, 0x08,   39 /* Private */,
      35,    0,  593,    2, 0x08,   40 /* Private */,
      36,    0,  594,    2, 0x08,   41 /* Private */,
      37,    0,  595,    2, 0x08,   42 /* Private */,
      38,    0,  596,    2, 0x08,   43 /* Private */,
      39,    0,  597,    2, 0x08,   44 /* Private */,
      40,    0,  598,    2, 0x08,   45 /* Private */,
      41,    0,  599,    2, 0x08,   46 /* Private */,
      42,    0,  600,    2, 0x08,   47 /* Private */,
      43,    3,  601,    2, 0x08,   48 /* Private */,
      43,    2,  608,    2, 0x28,   52 /* Private | MethodCloned */,
      43,    1,  613,    2, 0x28,   55 /* Private | MethodCloned */,
      43,    0,  616,    2, 0x28,   57 /* Private | MethodCloned */,
      47,    0,  617,    2, 0x08,   58 /* Private */,
      48,    0,  618,    2, 0x08,   59 /* Private */,
      49,    1,  619,    2, 0x08,   60 /* Private */,
      51,    0,  622,    2, 0x08,   62 /* Private */,
      52,    0,  623,    2, 0x08,   63 /* Private */,
      53,    0,  624,    2, 0x08,   64 /* Private */,
      54,    1,  625,    2, 0x08,   65 /* Private */,
      55,    0,  628,    2, 0x08,   67 /* Private */,
      56,    0,  629,    2, 0x08,   68 /* Private */,
      57,    0,  630,    2, 0x08,   69 /* Private */,
      58,    0,  631,    2, 0x08,   70 /* Private */,
      59,    0,  632,    2, 0x08,   71 /* Private */,
      60,    0,  633,    2, 0x08,   72 /* Private */,
      61,    0,  634,    2, 0x08,   73 /* Private */,
      62,    0,  635,    2, 0x08,   74 /* Private */,
      63,    0,  636,    2, 0x08,   75 /* Private */,
      64,    1,  637,    2, 0x08,   76 /* Private */,
      66,    1,  640,    2, 0x08,   78 /* Private */,
      67,    0,  643,    2, 0x08,   80 /* Private */,
      68,    0,  644,    2, 0x08,   81 /* Private */,
      69,    0,  645,    2, 0x08,   82 /* Private */,
      70,    0,  646,    2, 0x08,   83 /* Private */,
      71,    0,  647,    2, 0x08,   84 /* Private */,
      72,    0,  648,    2, 0x08,   85 /* Private */,
      73,    0,  649,    2, 0x08,   86 /* Private */,
      74,    0,  650,    2, 0x08,   87 /* Private */,
      75,    0,  651,    2, 0x08,   88 /* Private */,
      76,    0,  652,    2, 0x08,   89 /* Private */,
      77,    0,  653,    2, 0x08,   90 /* Private */,
      78,    0,  654,    2, 0x08,   91 /* Private */,
      79,    1,  655,    2, 0x08,   92 /* Private */,
      80,    1,  658,    2, 0x08,   94 /* Private */,
      81,    1,  661,    2, 0x08,   96 /* Private */,
      82,    1,  664,    2, 0x08,   98 /* Private */,
      83,    1,  667,    2, 0x08,  100 /* Private */,
      84,    1,  670,    2, 0x08,  102 /* Private */,
      85,    1,  673,    2, 0x08,  104 /* Private */,
      86,    0,  676,    2, 0x08,  106 /* Private */,
      87,    0,  677,    2, 0x08,  107 /* Private */,
      88,    0,  678,    2, 0x08,  108 /* Private */,
      89,    0,  679,    2, 0x08,  109 /* Private */,
      90,    0,  680,    2, 0x08,  110 /* Private */,
      91,    0,  681,    2, 0x08,  111 /* Private */,
      92,    0,  682,    2, 0x08,  112 /* Private */,
      93,    0,  683,    2, 0x08,  113 /* Private */,
      94,    0,  684,    2, 0x08,  114 /* Private */,
      95,    0,  685,    2, 0x08,  115 /* Private */,
      96,    0,  686,    2, 0x08,  116 /* Private */,
      97,    0,  687,    2, 0x08,  117 /* Private */,
      98,    0,  688,    2, 0x08,  118 /* Private */,
      99,    0,  689,    2, 0x08,  119 /* Private */,
     100,    0,  690,    2, 0x08,  120 /* Private */,
     101,    0,  691,    2, 0x08,  121 /* Private */,
     102,    0,  692,    2, 0x08,  122 /* Private */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,   13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 24, 0x80000000 | 26,   25,   27,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, 0x80000000 | 29, 0x80000000 | 26, 0x80000000 | 26, QMetaType::QPoint,   30,   31,   27,   32,
    QMetaType::Void, 0x80000000 | 29, 0x80000000 | 26, 0x80000000 | 26,   30,   31,   27,
    QMetaType::Void, 0x80000000 | 29, 0x80000000 | 26,   30,   31,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 44, QMetaType::Int, 0x80000000 | 20,   45,   46,   21,
    QMetaType::Void, 0x80000000 | 44, QMetaType::Int,   45,   46,
    QMetaType::Void, 0x80000000 | 44,   45,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 65,   30,
    QMetaType::Void, 0x80000000 | 65,   30,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void, QMetaType::Bool,   50,
    QMetaType::Void, QMetaType::Bool,   50,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject GasMainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN13GasMainWindowE.offsetsAndSizes,
    qt_meta_data_ZN13GasMainWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN13GasMainWindowE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GasMainWindow, std::true_type>,
        // method 'newFile'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'subWindowActivated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMdiSubWindow *, std::false_type>,
        // method 'updateAllActions'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateGasDocs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeChild'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QObject *, std::false_type>,
        // method 'showHelpTopic'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newViewOrAdd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openFromContent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        // method 'groupSubWindows'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pictureOnTop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'graphOnTop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'controlOnTop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const GasViewType, std::false_type>,
        // method 'editDescription'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'reparentChildDrop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QDropEvent *, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasTabWidget *, std::false_type>,
        // method 'reparentChildDrop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QDropEvent *, std::false_type>,
        // method 'reparentChild'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasChildWindow *, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasTabWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasTabWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'reparentChild'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasChildWindow *, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasTabWidget *, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasTabWidget *, std::false_type>,
        // method 'reparentChild'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasChildWindow *, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasTabWidget *, std::false_type>,
        // method 'tabifyChild'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasChildWindow *, std::false_type>,
        // method 'updateActiveGas'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveFileAs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'closeFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'closeAllFiles'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendMail'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'aboutGasMan'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'print'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasDoc *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasViewType, std::false_type>,
        // method 'newView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasDoc *, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'newView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<GasDoc *, std::false_type>,
        // method 'newView'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showHelp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showWorkbook'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setShowToolbar'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'selectPrintOutput'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setDefaultOptions'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openRecentFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeDoc'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QObject *, std::false_type>,
        // method 'createViewMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createWindowMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateFileMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateEditMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateAnesthesiaMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSpecialMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateTitle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateViewMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateWindowMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setActiveWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        // method 'setActiveWindowIfNec'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>,
        // method 'runSimulation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'runAllSimulation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'liquidInject'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rewind'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rewindAllSimulations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fastFwd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zeroTimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'clearAll'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'copyData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectAll'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'copySelection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'defaultAgentColors'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showCost'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'showMl'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'hideNumerics'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'disableUptake'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'disableReturn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'enableVapor'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'enableScrolling'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setBookmark'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setBreakPoint'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changePatient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'overlay'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zoomin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zoomout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'unitDose'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setCost'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'adjustDel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changeAgent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'windowStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'printPreview'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportToPdf'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportToXml'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportToJson'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'exportToHtml'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'transform'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void GasMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GasMainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = _t->newFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->subWindowActivated((*reinterpret_cast< std::add_pointer_t<QMdiSubWindow*>>(_a[1]))); break;
        case 2: _t->updateAllActions(); break;
        case 3: _t->updateGasDocs(); break;
        case 4: _t->removeChild((*reinterpret_cast< std::add_pointer_t<QObject*>>(_a[1]))); break;
        case 5: _t->showHelpTopic(); break;
        case 6: _t->newViewOrAdd(); break;
        case 7: _t->openFromContent((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[2]))); break;
        case 8: _t->groupSubWindows(); break;
        case 9: _t->pictureOnTop(); break;
        case 10: _t->graphOnTop(); break;
        case 11: _t->controlOnTop(); break;
        case 12: _t->onTop((*reinterpret_cast< std::add_pointer_t<GasViewType>>(_a[1]))); break;
        case 13: { bool _r = _t->editDescription();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->reparentChildDrop((*reinterpret_cast< std::add_pointer_t<QDropEvent*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GasTabWidget*>>(_a[2]))); break;
        case 15: _t->reparentChildDrop((*reinterpret_cast< std::add_pointer_t<QDropEvent*>>(_a[1]))); break;
        case 16: _t->reparentChild((*reinterpret_cast< std::add_pointer_t<GasChildWindow*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GasTabWidget*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<GasTabWidget*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[4]))); break;
        case 17: _t->reparentChild((*reinterpret_cast< std::add_pointer_t<GasChildWindow*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GasTabWidget*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<GasTabWidget*>>(_a[3]))); break;
        case 18: _t->reparentChild((*reinterpret_cast< std::add_pointer_t<GasChildWindow*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GasTabWidget*>>(_a[2]))); break;
        case 19: _t->tabifyChild((*reinterpret_cast< std::add_pointer_t<GasChildWindow*>>(_a[1]))); break;
        case 20: _t->updateActiveGas(); break;
        case 21: _t->openFile(); break;
        case 22: _t->saveFile(); break;
        case 23: _t->saveFileAs(); break;
        case 24: _t->closeFile(); break;
        case 25: _t->closeAllFiles(); break;
        case 26: _t->sendMail(); break;
        case 27: _t->aboutGasMan(); break;
        case 28: _t->print(); break;
        case 29: _t->newView((*reinterpret_cast< std::add_pointer_t<GasDoc*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<GasViewType>>(_a[3]))); break;
        case 30: _t->newView((*reinterpret_cast< std::add_pointer_t<GasDoc*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 31: _t->newView((*reinterpret_cast< std::add_pointer_t<GasDoc*>>(_a[1]))); break;
        case 32: _t->newView(); break;
        case 33: _t->showHelp(); break;
        case 34: _t->showWorkbook(); break;
        case 35: _t->setShowToolbar((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 36: _t->selectPrintOutput(); break;
        case 37: _t->setDefaultOptions(); break;
        case 38: _t->openRecentFile(); break;
        case 39: _t->removeDoc((*reinterpret_cast< std::add_pointer_t<QObject*>>(_a[1]))); break;
        case 40: _t->createViewMenu(); break;
        case 41: _t->createWindowMenu(); break;
        case 42: _t->updateFileMenu(); break;
        case 43: _t->updateEditMenu(); break;
        case 44: _t->updateAnesthesiaMenu(); break;
        case 45: _t->updateSpecialMenu(); break;
        case 46: _t->updateTitle(); break;
        case 47: _t->updateViewMenu(); break;
        case 48: _t->updateWindowMenu(); break;
        case 49: _t->setActiveWindow((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 50: _t->setActiveWindowIfNec((*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[1]))); break;
        case 51: _t->runSimulation(); break;
        case 52: _t->runAllSimulation(); break;
        case 53: _t->liquidInject(); break;
        case 54: _t->rewind(); break;
        case 55: _t->rewindAllSimulations(); break;
        case 56: _t->fastFwd(); break;
        case 57: _t->zeroTimer(); break;
        case 58: _t->clearAll(); break;
        case 59: _t->copyData(); break;
        case 60: _t->selectAll(); break;
        case 61: _t->copySelection(); break;
        case 62: _t->defaultAgentColors(); break;
        case 63: _t->showCost((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 64: _t->showMl((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 65: _t->hideNumerics((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 66: _t->disableUptake((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 67: _t->disableReturn((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 68: _t->enableVapor((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 69: _t->enableScrolling((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 70: _t->setBookmark(); break;
        case 71: _t->setBreakPoint(); break;
        case 72: _t->changePatient(); break;
        case 73: _t->overlay(); break;
        case 74: _t->zoomin(); break;
        case 75: _t->zoomout(); break;
        case 76: _t->unitDose(); break;
        case 77: _t->setCost(); break;
        case 78: _t->adjustDel(); break;
        case 79: _t->changeAgent(); break;
        case 80: _t->windowStateChanged(); break;
        case 81: _t->printPreview(); break;
        case 82: _t->exportToPdf(); break;
        case 83: _t->exportToXml(); break;
        case 84: _t->exportToJson(); break;
        case 85: _t->exportToHtml(); break;
        case 86: _t->transform(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QMdiSubWindow* >(); break;
            }
            break;
        case 49:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        case 50:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *GasMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN13GasMainWindowE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int GasMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 87)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 87;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 87)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 87;
    }
    return _id;
}
QT_WARNING_POP
