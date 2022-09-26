DESTDIR = $$MY_OUT_PATH
# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += shared
CONFIG += lib_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customwidgetplugin.cpp \
    gasgraph_test.cpp \
    main.cpp \
    gasgraph.cpp

HEADERS += \
    customwidgetplugin.h \
    gasgraph.h \
    gasgraph_test.h

FORMS += \
    gasgraph.ui \
    gasgraph_test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    gasgraph.xpm \
    gasgraph_plugin.json

TARGET = gasgraph
