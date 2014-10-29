#-------------------------------------------------
#
# Project created by QtCreator 2014-09-25T10:46:05
#
#-------------------------------------------------


VERSION = 0.0.2

CONFIG += use_opencv chiyo_framework

include(../paths.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChiyoPlugins
TEMPLATE = lib

DEFINES += CHIYOPLUGINS_LIBRARY

SOURCES += chiyoplugins.cpp \
    chiyopluginsloader.cpp \
    chiyoconverter.cpp

HEADERS += chiyoplugins.h\
        chiyoplugins_global.h \
    chiyopluginsloader.h \
    chiyoconverter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    ChiyoPlugins.qrc
