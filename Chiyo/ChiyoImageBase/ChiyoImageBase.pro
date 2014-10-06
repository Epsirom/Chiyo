#-------------------------------------------------
#
# Project created by QtCreator 2014-10-06T15:11:25
#
#-------------------------------------------------
VERSION = 0.0.1

CONFIG += use_opencv chiyo_plugin

include(../paths.pri)

QT       += widgets

TARGET = ChiyoImageBase
TEMPLATE = lib

DEFINES += CHIYOIMAGEBASE_LIBRARY

SOURCES += chiyoimagebase.cpp

HEADERS += chiyoimagebase.h\
        chiyoimagebase_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    ChiyoImageBase.qrc
