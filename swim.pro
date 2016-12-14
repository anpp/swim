#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T18:38:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CFLAGS += -std=c++11
QMAKE_CXXFLAGS += -std=c++11

TARGET = swim
TEMPLATE = app

CONFIG += static debug

SOURCES += main.cpp\
        swimwindow.cpp \
    apwidget.cpp \
    settings.cpp

HEADERS  += swimwindow.h \
    apwidget.h \
    settings.h

RESOURCES += \
    icons.qrc
