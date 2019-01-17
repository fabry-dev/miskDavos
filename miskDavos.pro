#-------------------------------------------------
#
# Project created by QtCreator 2018-09-28T08:37:50
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = miskDavos
TEMPLATE = app

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += mpv

SOURCES += main.cpp \
    mpvwidget.cpp \
    circlebutton.cpp \
    module3.cpp \
    module2.cpp \
    module4.cpp \
    picbutton.cpp \
    customslider.cpp \
    module0.cpp \
    mainScreen.cpp


HEADERS  += \
    mpvwidget.h \
    circlebutton.h \
    module3.h \
    module2.h \
    module4.h \
    picbutton.h \
    customslider.h \
    module0.h \
    mainScreen.h


FORMS    +=

