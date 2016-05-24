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
    connection.cpp \
    asyncconnection.cpp \
    defaultactionlistener.cpp \
    debugcallback.cpp \
    defaultcallback.cpp \
    unsubscribeactionlistener.cpp \
    createconnectiondialog.cpp \
    connectioncontent.cpp \
    asyncclient.cpp \
    connectoptions.cpp

HEADERS  += mainwindow.h \
    connection.h \
    asyncconnection.h \
    defaultactionlistener.h \
    debugcallback.h \
    defaultcallback.h \
    unsubscribeactionlistener.h \
    makeunique.h \
    createconnectiondialog.h \
    connectioncontent.h \
    asyncclient.h \
    connectoptions.h

FORMS    += mainwindow.ui \
    create_connection_dialog.ui \
    connectioncontent.ui


unix:!macx: LIBS += -L$$PWD/../paho.mqtt.cpp/lib/ -lmqttpp

INCLUDEPATH += $$PWD/../paho.mqtt.cpp/src
DEPENDPATH += $$PWD/../paho.mqtt.cpp/src

unix:!macx: LIBS += -L$$PWD/../paho.mqtt.c/build/output/ -lpaho-mqtt3as

INCLUDEPATH += $$PWD/../paho.mqtt.c/src
DEPENDPATH += $$PWD/../paho.mqtt.c/src
