#-------------------------------------------------
#
# Project created by QtCreator 2015-10-29T21:55:10
#
#-------------------------------------------------

QT       += core gui webkitwidgets avwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mlsplayer
TEMPLATE = app


SOURCES += main.cpp\
        player.cpp \
    ownapplication.cpp \
    downloadmg.cpp \
    downloadone.cpp \
    getufromc.cpp \
    getone.cpp \
    imgload.cpp \
    dllist.cpp \
    dlset.cpp \
    hebing.cpp \
    opendialog.cpp \
    pianku.cpp \
    shoucang.cpp \
    zhuanjidl.cpp \
    huancun.cpp \
    mywebpage.cpp \
    gettime.cpp

HEADERS  += player.h \
    ownapplication.h \
    downloadmg.h \
    downloadone.h \
    getone.h \
    getufromc.h \
    imgload.h \
    dllist.h \
    dlset.h \
    hebing.h \
    opendialog.h \
    pianku.h \
    shoucang.h \
    zhuanjidl.h \
    huancun.h \
    mywebpage.h \
    gettime.h

FORMS    += player.ui \
    dllist.ui \
    dlset.ui \
    hebing.ui \
    opendialog.ui \
    pianku.ui \
    shoucang.ui \
    zhuanjidl.ui

RESOURCES += \
    res.qrc
