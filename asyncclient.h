#ifndef ASYNCCLIENT_H
#define ASYNCCLIENT_H

#include <QString>

namespace linquitto {

class ConnectOptions;
class DisconnectOptions;
class EventCallback;
class ActionCallback;
class Message;

class AsyncClient
{
public:
    explicit AsyncClient();

    virtual ~AsyncClient() {}

    virtual bool isConnected() const = 0;
    virtual QString getClientId() const = 0;

    virtual void setCallback(EventCallback &callback) = 0;

    virtual void connect(const ConnectOptions &connOpt) = 0;
    virtual void disconnect(const DisconnectOptions &disconnOpt) = 0;

    virtual void publish(const QString &topic, const Message &message, ActionCallback &callback) = 0;
    virtual void subscribe(const QString &topic, int qos, ActionCallback &callback) = 0;
    virtual void unsubscribe(const QString &topic, ActionCallback &callback) = 0;

};

} // end of namespace linquitto

#endif // ASYNCCLIENT_H
