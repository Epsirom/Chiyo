#-------------------------------------------------
#
# Project created by QtCreator 2014-10-06T15:11:25
#
#-------------------------------------------------
VERSION = 0.0.2

CONFIG += use_opencv chiyo_plugin

include(../paths.pri)

QT       += widgets

TARGET = ChiyoImageBase
TEMPLATE = lib

DEFINES += CHIYOIMAGEBASE_LIBRARY

SOURCES += chiyoimagebase.cpp \
    vignette.cpp \
    white_balance.cpp \
    median.cpp \
    gaussianblur.cpp \
    saturation.cpp \
    liquefied.cpp \
    sharpen.cpp \
    liquefiedgraphicsview.cpp \
    lomo.cpp \
    beautiful.cpp \
    inpaintingthread.cpp \
    inpainting.cpp

HEADERS += chiyoimagebase.h\
        chiyoimagebase_global.h \
    vignette.h \
    white_balance.h \
    median.h \
    gaussianblur.h \
    saturation.h \
    liquefied.h \
    sharpen.h \
    liquefiedgraphicsview.h \
    lomo.h \
    beautiful.h \
    inpaintingthread.h \
    inpainting.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    ChiyoImageBase.qrc

FORMS += \
    vignette.ui \
    gaussianblur.ui \
    saturation.ui \
    liquefied.ui \
    lomo.ui \
    inpainting.ui
