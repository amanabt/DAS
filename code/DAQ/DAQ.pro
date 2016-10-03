#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T21:44:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DAQ
TEMPLATE = app


SOURCES += main.cpp\
        WC_mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += WC_mainwindow.h \
    qcustomplot.h

FORMS    += UI_mainwindow.ui
