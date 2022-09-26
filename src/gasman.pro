MY_OUT_PATH = ../compiled


TEMPLATE = subdirs


QMAKE_CXXFLAGS += -rpath=./gasmchn
QMAKE_CXXFLAGS += -rpath=./gasgraph
QMAKE_CXXFLAGS += -rpath=./gasgauge
QMAKE_CXXFLAGS += -rpath=./gasanibutton

TRANSLATIONS += \
    language/gasman_en.ts \
    language/gasman_fr.ts \
    language/gasman_ru.ts \
    language/gasman_zh_CN.ts

CONFIG += lrelease
CONFIG += embed_translations

SUBDIRS +=  \
    gasanibutton \
    gasgauge \
    gasgraph \
    gasmchn

CONFIG += ordered

SUBDIRS += gasman_main

gasman_main.depends =gasanibutton \
gasgauge \
gasgraph \
gasmchn



