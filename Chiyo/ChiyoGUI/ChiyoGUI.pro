#-------------------------------------------------
#
# Project created by QtCreator 2014-09-24T14:51:43
#
#-------------------------------------------------

VERSION = 0.0.1

CONFIG += use_opencv
#CONFIG += release qscintilla2

include(../paths.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chiyo
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    chiyographics.cpp \
    chiyoeditor.cpp \
    chiyoconsole.cpp \
    chiyocontrols.cpp \
    chiyohistory.cpp \
    chiyogui_global.cpp \
    chiyoloader.cpp

HEADERS  += mainwindow.h \
    chiyographics.h \
    chiyoeditor.h \
    chiyoconsole.h \
    chiyocontrols.h \
    chiyohistory.h \
    chiyogui_global.h \
    chiyoloader.h

FORMS    += mainwindow.ui \
    chiyoeditor.ui \
    chiyoconsole.ui \
    chiyocontrols.ui \
    chiyohistory.ui \
    chiyoloader.ui

RESOURCES += \
    ChiyoGUI.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ChiyoPlugins/release/ -lChiyoPlugins
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ChiyoPlugins/debug/ -lChiyoPlugins
else:unix: LIBS += -L$$OUT_PWD/../ChiyoPlugins/ -lChiyoPlugins

INCLUDEPATH += $$PWD/../ChiyoPlugins
DEPENDPATH += $$PWD/../ChiyoPlugins

unix {
    #QMAKE_POST_LINK += install_name_tool -change libqscintilla2.11.dylib $$[QT_INSTALL_LIBS]/libqscintilla2.11.dylib $(TARGET);
    QMAKE_POST_LINK += install_name_tool -change libChiyoPlugins.0.dylib $$OUT_PWD/../ChiyoPlugins/libChiyoPlugins.0.dylib $(TARGET);
}
