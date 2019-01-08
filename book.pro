#-------------------------------------------------
#
# Project created by QtCreator 2018-09-28T08:37:50
#
#-------------------------------------------------

QT       += core gui serialport
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = book
TEMPLATE = app

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += mpv

SOURCES += main.cpp \
    serialwatcher.cpp \
    videowindow.cpp \
    mpvwidget.cpp


HEADERS  += \
    serialwatcher.h \
    videowindow.h \
    mpvwidget.h


FORMS    +=

