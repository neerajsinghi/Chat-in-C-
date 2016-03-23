#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T21:50:52
#
#-------------------------------------------------

QT       += core gui
 QT      += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChaatApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    privateclient.cpp \
    groupclient.cpp

HEADERS  += mainwindow.h \
    privateclient.h \
    groupclient.h

FORMS    += mainwindow.ui \
    privateclient.ui \
    groupclient.ui
