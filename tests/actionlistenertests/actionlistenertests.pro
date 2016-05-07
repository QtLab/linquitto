#-------------------------------------------------
#
# Project created by QtCreator 2016-05-07T14:01:36
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_actionlistenertests
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += tst_actionlistenertests.cpp \
    ../../unsubscribeactionlistener.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$PWD/../../../paho.mqtt.c/build/output/ -lpaho-mqtt3a

INCLUDEPATH += $$PWD/../../../paho.mqtt.c/build/output
DEPENDPATH += $$PWD/../../../paho.mqtt.c/build/output

unix:!macx: LIBS += -L$$PWD/../../../paho.mqtt.cpp/lib/ -lmqttpp

INCLUDEPATH += $$PWD/../../../paho.mqtt.cpp/src
DEPENDPATH += $$PWD/../../../paho.mqtt.cpp/src

HEADERS += \
    ../../unsubscribeactionlistener.h
