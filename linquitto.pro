#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T11:32:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = linquitto
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    asyncconnection.cpp \
    createconnectiondialog.cpp \
    connectioncontent.cpp \
    connectoptions.cpp \
    protectableasyncclient.cpp \
    disconnectoptions.cpp \
    defaultactioncallback.cpp \
    tools.cpp \
    message.cpp \
    responseoptions.cpp \
    defaulteventcallback.cpp \
    subscriptionactioncallback.cpp

HEADERS  += mainwindow.h \
    asyncconnection.h \
    makeunique.h \
    createconnectiondialog.h \
    connectioncontent.h \
    asyncclient.h \
    connectoptions.h \
    protectableasyncclient.h \
    disconnectoptions.h \
    actioncallback.h \
    defaultactioncallback.h \
    tools.h \
    message.h \
    responseoptions.h \
    eventcallback.h \
    defaulteventcallback.h \
    subscriptionactioncallback.h

FORMS    += mainwindow.ui \
    create_connection_dialog.ui \
    connectioncontent.ui

unix:!macx: LIBS += -L$$PWD/../paho.mqtt.c/build/output/ -lpaho-mqtt3as

INCLUDEPATH += $$PWD/../paho.mqtt.c/src
DEPENDPATH += $$PWD/../paho.mqtt.c/src
