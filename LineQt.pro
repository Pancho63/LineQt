#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T14:17:24
#
#-------------------------------------------------

QT       += core gui
QT         += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LineQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    oscpkt.hh

FORMS    += mainwindow.ui
