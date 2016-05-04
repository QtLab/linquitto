#ifndef DEBUGCALLBACK_H
#define DEBUGCALLBACK_H

#include <QObject>

#include "mqtt/callback.h"

class DebugCallback : public QObject, public mqtt::callback
{
    Q_OBJECT
public:
    explicit DebugCallback(QObject *parent = 0);

signals:

public slots:

    // callback interface
public:
    void connection_lost(const std::__cxx11::string &cause);
    void message_arrived(const std::__cxx11::string &topic, mqtt::message_ptr msg);
    void delivery_complete(mqtt::idelivery_token_ptr tok);
};

#endif // DEBUGCALLBACK_H
