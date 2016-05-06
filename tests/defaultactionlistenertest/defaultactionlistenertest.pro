#-------------------------------------------------
#
# Project created by QtCreator 2016-05-06T21:32:58
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_defaultactionlistenertest
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += c++11

TEMPLATE = app


SOURCES += tst_defaultactionlistenertest.cpp \
    ../../defaultactionlistener.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../defaultactionlistener.h

unix:!macx: LIBS += -L$$PWD/../../../paho.mqtt.c/build/output/ -lpaho-mqtt3a

INCLUDEPATH += $$PWD/../../../paho.mqtt.c/build/output
DEPENDPATH += $$PWD/../../../paho.mqtt.c/build/output

unix:!macx: LIBS += -L$$PWD/../../../paho.mqtt.cpp/lib/ -lmqttpp

INCLUDEPATH += $$PWD/../../../paho.mqtt.cpp/src
DEPENDPATH += $$PWD/../../../paho.mqtt.cpp/src
