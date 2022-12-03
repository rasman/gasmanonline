/****************************************************************************
** Meta object code from reading C++ file 'gasmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/gasman_main/gasmainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gasmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GasMainWindow_t {
    QByteArrayData data[101];
    char stringdata0[1203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GasMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GasMainWindow_t qt_meta_stringdata_GasMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GasMainWindow"
QT_MOC_LITERAL(1, 14, 7), // "newFile"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 18), // "subWindowActivated"
QT_MOC_LITERAL(4, 42, 14), // "QMdiSubWindow*"
QT_MOC_LITERAL(5, 57, 2), // "sw"
QT_MOC_LITERAL(6, 60, 16), // "updateAllActions"
QT_MOC_LITERAL(7, 77, 13), // "updateGasDocs"
QT_MOC_LITERAL(8, 91, 11), // "removeChild"
QT_MOC_LITERAL(9, 103, 3), // "obj"
QT_MOC_LITERAL(10, 107, 13), // "showHelpTopic"
QT_MOC_LITERAL(11, 121, 12), // "newViewOrAdd"
QT_MOC_LITERAL(12, 134, 15), // "groupSubWindows"
QT_MOC_LITERAL(13, 150, 12), // "pictureOnTop"
QT_MOC_LITERAL(14, 163, 10), // "graphOnTop"
QT_MOC_LITERAL(15, 174, 12), // "controlOnTop"
QT_MOC_LITERAL(16, 187, 5), // "onTop"
QT_MOC_LITERAL(17, 193, 11), // "GasViewType"
QT_MOC_LITERAL(18, 205, 8), // "viewType"
QT_MOC_LITERAL(19, 214, 15), // "editDescription"
QT_MOC_LITERAL(20, 230, 17), // "reparentChildDrop"
QT_MOC_LITERAL(21, 248, 11), // "QDropEvent*"
QT_MOC_LITERAL(22, 260, 5), // "event"
QT_MOC_LITERAL(23, 266, 13), // "GasTabWidget*"
QT_MOC_LITERAL(24, 280, 4), // "dest"
QT_MOC_LITERAL(25, 285, 13), // "reparentChild"
QT_MOC_LITERAL(26, 299, 15), // "GasChildWindow*"
QT_MOC_LITERAL(27, 315, 5), // "child"
QT_MOC_LITERAL(28, 321, 6), // "source"
QT_MOC_LITERAL(29, 328, 5), // "point"
QT_MOC_LITERAL(30, 334, 11), // "tabifyChild"
QT_MOC_LITERAL(31, 346, 15), // "updateActiveGas"
QT_MOC_LITERAL(32, 362, 8), // "openFile"
QT_MOC_LITERAL(33, 371, 8), // "saveFile"
QT_MOC_LITERAL(34, 380, 10), // "saveFileAs"
QT_MOC_LITERAL(35, 391, 9), // "closeFile"
QT_MOC_LITERAL(36, 401, 13), // "closeAllFiles"
QT_MOC_LITERAL(37, 415, 8), // "sendMail"
QT_MOC_LITERAL(38, 424, 11), // "aboutGasMan"
QT_MOC_LITERAL(39, 436, 5), // "print"
QT_MOC_LITERAL(40, 442, 9), // "pageSetup"
QT_MOC_LITERAL(41, 452, 7), // "newView"
QT_MOC_LITERAL(42, 460, 7), // "GasDoc*"
QT_MOC_LITERAL(43, 468, 3), // "doc"
QT_MOC_LITERAL(44, 472, 4), // "nGas"
QT_MOC_LITERAL(45, 477, 15), // "registerProduct"
QT_MOC_LITERAL(46, 493, 8), // "showHelp"
QT_MOC_LITERAL(47, 502, 12), // "showWorkbook"
QT_MOC_LITERAL(48, 515, 14), // "setShowToolbar"
QT_MOC_LITERAL(49, 530, 3), // "set"
QT_MOC_LITERAL(50, 534, 17), // "selectPrintOutput"
QT_MOC_LITERAL(51, 552, 17), // "setDefaultOptions"
QT_MOC_LITERAL(52, 570, 14), // "openRecentFile"
QT_MOC_LITERAL(53, 585, 9), // "removeDoc"
QT_MOC_LITERAL(54, 595, 14), // "createViewMenu"
QT_MOC_LITERAL(55, 610, 16), // "createWindowMenu"
QT_MOC_LITERAL(56, 627, 14), // "updateFileMenu"
QT_MOC_LITERAL(57, 642, 14), // "updateEditMenu"
QT_MOC_LITERAL(58, 657, 20), // "updateAnesthesiaMenu"
QT_MOC_LITERAL(59, 678, 17), // "updateSpecialMenu"
QT_MOC_LITERAL(60, 696, 11), // "updateTitle"
QT_MOC_LITERAL(61, 708, 14), // "updateViewMenu"
QT_MOC_LITERAL(62, 723, 16), // "updateWindowMenu"
QT_MOC_LITERAL(63, 740, 15), // "setActiveWindow"
QT_MOC_LITERAL(64, 756, 8), // "QWidget*"
QT_MOC_LITERAL(65, 765, 20), // "setActiveWindowIfNec"
QT_MOC_LITERAL(66, 786, 13), // "runSimulation"
QT_MOC_LITERAL(67, 800, 16), // "runAllSimulation"
QT_MOC_LITERAL(68, 817, 12), // "liquidInject"
QT_MOC_LITERAL(69, 830, 6), // "rewind"
QT_MOC_LITERAL(70, 837, 20), // "rewindAllSimulations"
QT_MOC_LITERAL(71, 858, 7), // "fastFwd"
QT_MOC_LITERAL(72, 866, 9), // "zeroTimer"
QT_MOC_LITERAL(73, 876, 8), // "clearAll"
QT_MOC_LITERAL(74, 885, 8), // "copyData"
QT_MOC_LITERAL(75, 894, 9), // "selectAll"
QT_MOC_LITERAL(76, 904, 13), // "copySelection"
QT_MOC_LITERAL(77, 918, 18), // "defaultAgentColors"
QT_MOC_LITERAL(78, 937, 8), // "showCost"
QT_MOC_LITERAL(79, 946, 6), // "showMl"
QT_MOC_LITERAL(80, 953, 12), // "hideNumerics"
QT_MOC_LITERAL(81, 966, 13), // "disableUptake"
QT_MOC_LITERAL(82, 980, 13), // "disableReturn"
QT_MOC_LITERAL(83, 994, 11), // "enableVapor"
QT_MOC_LITERAL(84, 1006, 15), // "enableScrolling"
QT_MOC_LITERAL(85, 1022, 11), // "setBookmark"
QT_MOC_LITERAL(86, 1034, 13), // "setBreakPoint"
QT_MOC_LITERAL(87, 1048, 13), // "changePatient"
QT_MOC_LITERAL(88, 1062, 7), // "overlay"
QT_MOC_LITERAL(89, 1070, 6), // "zoomin"
QT_MOC_LITERAL(90, 1077, 7), // "zoomout"
QT_MOC_LITERAL(91, 1085, 8), // "unitDose"
QT_MOC_LITERAL(92, 1094, 7), // "setCost"
QT_MOC_LITERAL(93, 1102, 9), // "adjustDel"
QT_MOC_LITERAL(94, 1112, 11), // "changeAgent"
QT_MOC_LITERAL(95, 1124, 18), // "windowStateChanged"
QT_MOC_LITERAL(96, 1143, 12), // "printPreview"
QT_MOC_LITERAL(97, 1156, 11), // "exportToPdf"
QT_MOC_LITERAL(98, 1168, 11), // "exportToXml"
QT_MOC_LITERAL(99, 1180, 12), // "exportToHtml"
QT_MOC_LITERAL(100, 1193, 9) // "transform"

    },
    "GasMainWindow\0newFile\0\0subWindowActivated\0"
    "QMdiSubWindow*\0sw\0updateAllActions\0"
    "updateGasDocs\0removeChild\0obj\0"
    "showHelpTopic\0newViewOrAdd\0groupSubWindows\0"
    "pictureOnTop\0graphOnTop\0controlOnTop\0"
    "onTop\0GasViewType\0viewType\0editDescription\0"
    "reparentChildDrop\0QDropEvent*\0event\0"
    "GasTabWidget*\0dest\0reparentChild\0"
    "GasChildWindow*\0child\0source\0point\0"
    "tabifyChild\0updateActiveGas\0openFile\0"
    "saveFile\0saveFileAs\0closeFile\0"
    "closeAllFiles\0sendMail\0aboutGasMan\0"
    "print\0pageSetup\0newView\0GasDoc*\0doc\0"
    "nGas\0registerProduct\0showHelp\0"
    "showWorkbook\0setShowToolbar\0set\0"
    "selectPrintOutput\0setDefaultOptions\0"
    "openRecentFile\0removeDoc\0createViewMenu\0"
    "createWindowMenu\0updateFileMenu\0"
    "updateEditMenu\0updateAnesthesiaMenu\0"
    "updateSpecialMenu\0updateTitle\0"
    "updateViewMenu\0updateWindowMenu\0"
    "setActiveWindow\0QWidget*\0setActiveWindowIfNec\0"
    "runSimulation\0runAllSimulation\0"
    "liquidInject\0rewind\0rewindAllSimulations\0"
    "fastFwd\0zeroTimer\0clearAll\0copyData\0"
    "selectAll\0copySelection\0defaultAgentColors\0"
    "showCost\0showMl\0hideNumerics\0disableUptake\0"
    "disableReturn\0enableVapor\0enableScrolling\0"
    "setBookmark\0setBreakPoint\0changePatient\0"
    "overlay\0zoomin\0zoomout\0unitDose\0setCost\0"
    "adjustDel\0changeAgent\0windowStateChanged\0"
    "printPreview\0exportToPdf\0exportToXml\0"
    "exportToHtml\0transform"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GasMainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      87,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  449,    2, 0x0a /* Public */,
       3,    1,  450,    2, 0x0a /* Public */,
       6,    0,  453,    2, 0x0a /* Public */,
       7,    0,  454,    2, 0x0a /* Public */,
       8,    1,  455,    2, 0x0a /* Public */,
      10,    0,  458,    2, 0x0a /* Public */,
      11,    0,  459,    2, 0x08 /* Private */,
      12,    0,  460,    2, 0x08 /* Private */,
      13,    0,  461,    2, 0x08 /* Private */,
      14,    0,  462,    2, 0x08 /* Private */,
      15,    0,  463,    2, 0x08 /* Private */,
      16,    1,  464,    2, 0x08 /* Private */,
      19,    0,  467,    2, 0x08 /* Private */,
      20,    2,  468,    2, 0x08 /* Private */,
      20,    1,  473,    2, 0x28 /* Private | MethodCloned */,
      25,    4,  476,    2, 0x08 /* Private */,
      25,    3,  485,    2, 0x28 /* Private | MethodCloned */,
      25,    2,  492,    2, 0x28 /* Private | MethodCloned */,
      30,    1,  497,    2, 0x08 /* Private */,
      31,    0,  500,    2, 0x08 /* Private */,
      32,    0,  501,    2, 0x08 /* Private */,
      33,    0,  502,    2, 0x08 /* Private */,
      34,    0,  503,    2, 0x08 /* Private */,
      35,    0,  504,    2, 0x08 /* Private */,
      36,    0,  505,    2, 0x08 /* Private */,
      37,    0,  506,    2, 0x08 /* Private */,
      38,    0,  507,    2, 0x08 /* Private */,
      39,    0,  508,    2, 0x08 /* Private */,
      40,    0,  509,    2, 0x08 /* Private */,
      41,    3,  510,    2, 0x08 /* Private */,
      41,    2,  517,    2, 0x28 /* Private | MethodCloned */,
      41,    1,  522,    2, 0x28 /* Private | MethodCloned */,
      41,    0,  525,    2, 0x28 /* Private | MethodCloned */,
      45,    0,  526,    2, 0x08 /* Private */,
      46,    0,  527,    2, 0x08 /* Private */,
      47,    0,  528,    2, 0x08 /* Private */,
      48,    1,  529,    2, 0x08 /* Private */,
      50,    0,  532,    2, 0x08 /* Private */,
      51,    0,  533,    2, 0x08 /* Private */,
      52,    0,  534,    2, 0x08 /* Private */,
      53,    1,  535,    2, 0x08 /* Private */,
      54,    0,  538,    2, 0x08 /* Private */,
      55,    0,  539,    2, 0x08 /* Private */,
      56,    0,  540,    2, 0x08 /* Private */,
      57,    0,  541,    2, 0x08 /* Private */,
      58,    0,  542,    2, 0x08 /* Private */,
      59,    0,  543,    2, 0x08 /* Private */,
      60,    0,  544,    2, 0x08 /* Private */,
      61,    0,  545,    2, 0x08 /* Private */,
      62,    0,  546,    2, 0x08 /* Private */,
      63,    1,  547,    2, 0x08 /* Private */,
      65,    1,  550,    2, 0x08 /* Private */,
      66,    0,  553,    2, 0x08 /* Private */,
      67,    0,  554,    2, 0x08 /* Private */,
      68,    0,  555,    2, 0x08 /* Private */,
      69,    0,  556,    2, 0x08 /* Private */,
      70,    0,  557,    2, 0x08 /* Private */,
      71,    0,  558,    2, 0x08 /* Private */,
      72,    0,  559,    2, 0x08 /* Private */,
      73,    0,  560,    2, 0x08 /* Private */,
      74,    0,  561,    2, 0x08 /* Private */,
      75,    0,  562,    2, 0x08 /* Private */,
      76,    0,  563,    2, 0x08 /* Private */,
      77,    0,  564,    2, 0x08 /* Private */,
      78,    1,  565,    2, 0x08 /* Private */,
      79,    1,  568,    2, 0x08 /* Private */,
      80,    1,  571,    2, 0x08 /* Private */,
      81,    1,  574,    2, 0x08 /* Private */,
      82,    1,  577,    2, 0x08 /* Private */,
      83,    1,  580,    2, 0x08 /* Private */,
      84,    1,  583,    2, 0x08 /* Private */,
      85,    0,  586,    2, 0x08 /* Private */,
      86,    0,  587,    2, 0x08 /* Private */,
      87,    0,  588,    2, 0x08 /* Private */,
      88,    0,  589,    2, 0x08 /* Private */,
      89,    0,  590,    2, 0x08 /* Private */,
      90,    0,  591,    2, 0x08 /* Private */,
      91,    0,  592,    2, 0x08 /* Private */,
      92,    0,  593,    2, 0x08 /* Private */,
      93,    0,  594,    2, 0x08 /* Private */,
      94,    0,  595,    2, 0x08 /* Private */,
      95,    0,  596,    2, 0x08 /* Private */,
      96,    0,  597,    2, 0x08 /* Private */,
      97,    0,  598,    2, 0x08 /* Private */,
      98,    0,  599,    2, 0x08 /* Private */,
      99,    0,  600,    2, 0x08 /* Private */,
     100,    0,  601,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Bool,
    QMetaType::Void, 0x80000000 | 21, 0x80000000 | 23,   22,   24,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 23, 0x80000000 | 23, QMetaType::QPoint,   27,   28,   24,   29,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 23, 0x80000000 | 23,   27,   28,   24,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 23,   27,   28,
    QMetaType::Void, 0x80000000 | 26,   27,
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
    QMetaType::Void, 0x80000000 | 42, QMetaType::Int, 0x80000000 | 17,   43,   44,   18,
    QMetaType::Void, 0x80000000 | 42, QMetaType::Int,   43,   44,
    QMetaType::Void, 0x80000000 | 42,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   49,
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
    QMetaType::Void, 0x80000000 | 64,   27,
    QMetaType::Void, 0x80000000 | 64,   27,
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
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::Bool,   49,
    QMetaType::Void, QMetaType::Bool,   49,
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

void GasMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GasMainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { bool _r = _t->newFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->subWindowActivated((*reinterpret_cast< QMdiSubWindow*(*)>(_a[1]))); break;
        case 2: _t->updateAllActions(); break;
        case 3: _t->updateGasDocs(); break;
        case 4: _t->removeChild((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 5: _t->showHelpTopic(); break;
        case 6: _t->newViewOrAdd(); break;
        case 7: _t->groupSubWindows(); break;
        case 8: _t->pictureOnTop(); break;
        case 9: _t->graphOnTop(); break;
        case 10: _t->controlOnTop(); break;
        case 11: _t->onTop((*reinterpret_cast< const GasViewType(*)>(_a[1]))); break;
        case 12: { bool _r = _t->editDescription();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->reparentChildDrop((*reinterpret_cast< QDropEvent*(*)>(_a[1])),(*reinterpret_cast< GasTabWidget*(*)>(_a[2]))); break;
        case 14: _t->reparentChildDrop((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 15: _t->reparentChild((*reinterpret_cast< GasChildWindow*(*)>(_a[1])),(*reinterpret_cast< GasTabWidget*(*)>(_a[2])),(*reinterpret_cast< GasTabWidget*(*)>(_a[3])),(*reinterpret_cast< const QPoint(*)>(_a[4]))); break;
        case 16: _t->reparentChild((*reinterpret_cast< GasChildWindow*(*)>(_a[1])),(*reinterpret_cast< GasTabWidget*(*)>(_a[2])),(*reinterpret_cast< GasTabWidget*(*)>(_a[3]))); break;
        case 17: _t->reparentChild((*reinterpret_cast< GasChildWindow*(*)>(_a[1])),(*reinterpret_cast< GasTabWidget*(*)>(_a[2]))); break;
        case 18: _t->tabifyChild((*reinterpret_cast< GasChildWindow*(*)>(_a[1]))); break;
        case 19: _t->updateActiveGas(); break;
        case 20: _t->openFile(); break;
        case 21: _t->saveFile(); break;
        case 22: _t->saveFileAs(); break;
        case 23: _t->closeFile(); break;
        case 24: _t->closeAllFiles(); break;
        case 25: _t->sendMail(); break;
        case 26: _t->aboutGasMan(); break;
        case 27: _t->print(); break;
        case 28: _t->pageSetup(); break;
        case 29: _t->newView((*reinterpret_cast< GasDoc*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< GasViewType(*)>(_a[3]))); break;
        case 30: _t->newView((*reinterpret_cast< GasDoc*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 31: _t->newView((*reinterpret_cast< GasDoc*(*)>(_a[1]))); break;
        case 32: _t->newView(); break;
        case 33: _t->registerProduct(); break;
        case 34: _t->showHelp(); break;
        case 35: _t->showWorkbook(); break;
        case 36: _t->setShowToolbar((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 37: _t->selectPrintOutput(); break;
        case 38: _t->setDefaultOptions(); break;
        case 39: _t->openRecentFile(); break;
        case 40: _t->removeDoc((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 41: _t->createViewMenu(); break;
        case 42: _t->createWindowMenu(); break;
        case 43: _t->updateFileMenu(); break;
        case 44: _t->updateEditMenu(); break;
        case 45: _t->updateAnesthesiaMenu(); break;
        case 46: _t->updateSpecialMenu(); break;
        case 47: _t->updateTitle(); break;
        case 48: _t->updateViewMenu(); break;
        case 49: _t->updateWindowMenu(); break;
        case 50: _t->setActiveWindow((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 51: _t->setActiveWindowIfNec((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 52: _t->runSimulation(); break;
        case 53: _t->runAllSimulation(); break;
        case 54: _t->liquidInject(); break;
        case 55: _t->rewind(); break;
        case 56: _t->rewindAllSimulations(); break;
        case 57: _t->fastFwd(); break;
        case 58: _t->zeroTimer(); break;
        case 59: _t->clearAll(); break;
        case 60: _t->copyData(); break;
        case 61: _t->selectAll(); break;
        case 62: _t->copySelection(); break;
        case 63: _t->defaultAgentColors(); break;
        case 64: _t->showCost((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 65: _t->showMl((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 66: _t->hideNumerics((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 67: _t->disableUptake((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 68: _t->disableReturn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 69: _t->enableVapor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 70: _t->enableScrolling((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 71: _t->setBookmark(); break;
        case 72: _t->setBreakPoint(); break;
        case 73: _t->changePatient(); break;
        case 74: _t->overlay(); break;
        case 75: _t->zoomin(); break;
        case 76: _t->zoomout(); break;
        case 77: _t->unitDose(); break;
        case 78: _t->setCost(); break;
        case 79: _t->adjustDel(); break;
        case 80: _t->changeAgent(); break;
        case 81: _t->windowStateChanged(); break;
        case 82: _t->printPreview(); break;
        case 83: _t->exportToPdf(); break;
        case 84: _t->exportToXml(); break;
        case 85: _t->exportToHtml(); break;
        case 86: _t->transform(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMdiSubWindow* >(); break;
            }
            break;
        case 50:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        case 51:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GasMainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_GasMainWindow.data,
    qt_meta_data_GasMainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GasMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GasMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GasMainWindow.stringdata0))
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
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 87)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 87;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
