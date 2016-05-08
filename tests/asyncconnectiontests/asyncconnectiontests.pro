#-------------------------------------------------
#
# Project created by QtCreator 2016-05-08T15:15:53
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_asyncconnectiontests
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += tst_asyncconnectiontests.cpp \
    ../../defaultactionlistener.cpp \
    ../../defaultcallback.cpp \
    ../../asyncconnection.cpp \
    ../../unsubscribeactionlistener.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

unix:!macx: LIBS += -L$$PWD/../../../paho.mqtt.c/build/output/ -lpaho-mqtt3a

INCLUDEPATH += $$PWD/../../../paho.mqtt.c/build/output
DEPENDPATH += $$PWD/../../../paho.mqtt.c/build/output

unix:!macx: LIBS += -L$$PWD/../../../paho.mqtt.cpp/lib/ -lmqttpp

INCLUDEPATH += $$PWD/../../../paho.mqtt.cpp/src
DEPENDPATH += $$PWD/../../../paho.mqtt.cpp/src

unix:!macx: LIBS += -L$$PWD/../../../paho.mqtt.c/build/output/ -lpaho-mqtt3as

INCLUDEPATH += $$PWD/../../../paho.mqtt.c/build/output
DEPENDPATH += $$PWD/../../../paho.mqtt.c/build/output

HEADERS += \
    ../../defaultactionlistener.h \
    ../../defaultcallback.h \
    ../../asyncconnection.h \
    ../../unsubscribeactionlistener.h
