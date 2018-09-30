#-------------------------------------------------
#
# Project created by QtCreator 2018-09-28T08:37:50
#
#-------------------------------------------------

QT       += core gui serialport
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = falcon
TEMPLATE = app


SOURCES += main.cpp \
    slidewindow.cpp \
    slidevideo.cpp \
    videoplayer.cpp \
    serialwatcher.cpp \
    showrunner.cpp

HEADERS  += \
    slidewindow.h \
    slidevideo.h \
    videoplayer.h \
    serialwatcher.h \
    showrunner.h

FORMS    +=


LIBS += -lvlc
