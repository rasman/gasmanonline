INCLUDEPATH += . ..
WARNINGS += -Wall

UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs

TEMPLATE = app

QT += core gui svg xml multimedia printsupport dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 -Wall

INCLUDEPATH +=  ../gasgraph\
../gasanibutton\
../gasgauge\
../gasmchn

DEPENDPATH += \
../gasgraph\
../gasanibutton\
../gasgauge\
../gasmchn

LIBS += -L$$OUT_PWD/../gasanibutton/ -lgasanibutton\
-L$$OUT_PWD/../gasgauge/ -lgasgauge\
-L$$OUT_PWD/../gasgraph/ -lgasgraph\
-L$$OUT_PWD/../gasmchn/ -lgasmchn

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    assistant.cpp \
    gas.cpp \
    gasaboutdlg.cpp \
    gasagentdelegate.cpp \
    gasagentdlg.cpp \
    gasanes.cpp \
    gasapplication.cpp \
    gasbkpoint.cpp \
    gasbookmarkdlg.cpp \
    gaschildwindow.cpp \
    gascontrol.cpp \
    gascostdlg.cpp \
    gasdeldlg.cpp \
    gasdoc.cpp \
    gasdosedlg.cpp \
    gasglobal.cpp \
    gasgraphview.cpp \
    gaslicensemanager.cpp \
    gasmacstyle.cpp \
    gasmainwindow.cpp \
    gasoptionsdlg.cpp \
    gasoverlay.cpp \
    gaspanelview.cpp \
    gasparser.cpp \
    gaspatientdlg.cpp \
    gasprintpreview.cpp \
    gasprintseldlg.cpp \
    gasregisterdlg.cpp \
    gassample.cpp \
    gassplashscreen.cpp \
    gastabbar.cpp \
    gastabwidget.cpp \
    gasviewcommon.cpp \
    generator.cpp \
    main.cpp

HEADERS += \
    assistant.h \
    gas.h \
    gasaboutdlg.h \
    gasagentdelegate.h \
    gasagentdlg.h \
    gasanes.h \
    gasapplication.h \
    gasbkpoint.h \
    gasbookmarkdlg.h \
    gaschildwindow.h \
    gascontrol.h \
    gascostdlg.h \
    gasdeldlg.h \
    gasdoc.h \
    gasdosedlg.h \
    gasevent.h \
    gasglobal.h \
    gasgraphview.h \
    gaslicensemanager.h \
    gasmacstyle.h \
    gasmainwindow.h \
    gasoptionsdlg.h \
    gasoverlay.h \
    gaspanelview.h \
    gasparser.h \
    gaspatientdlg.h \
    gasprintpreview.h \
    gasprintseldlg.h \
    gasptrlist.h \
    gasregisterdlg.h \
    gassample.h \
    gassplashscreen.h \
    gastabbar.h \
    gastabwidget.h \
    gasviewcommon.h \
    generator.h \
    import_plugins.h \
    semaphoreholder.h

unix {
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/
    CONFIG += cryptp
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    gasaboutdlg.ui \
    gasagentdlg.ui \
    gasbkpoint.ui \
    gasbookmarkdlg.ui \
    gascdefaults.ui \
    gaschoicesound.ui \
    gascontrol.ui \
    gascostdlg.ui \
    gasdeldlg.ui \
    gasdosedlg.ui \
    gasgraphview.ui \
    gasoverlay.ui \
    gaspanelview.ui \
    gaspatient.ui \
    gaspdefaults.ui \
    gasprintselectdlg.ui \
    gasregisterdlg.ui \
    gassdefaults.ui \
    gasvdefaults.ui

TARGET = gasman

DISTFILES += \
    gasman.ini
