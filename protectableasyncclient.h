#ifndef PROTECTABLEASYNCCLIENT_H
#define PROTECTABLEASYNCCLIENT_H

#include "asyncclient.h"
#include "connectoptions.h"
#include "disconnectoptions.h"

namespace linquitto {

class ProtectableAsyncClient: public AsyncClient
{
public:
    ProtectableAsyncClient();
    ~ProtectableAsyncClient();


    // AsyncClient interface
public:
    bool isConnected() const;
    QString getClientId() const;
    void setCallback(EventCallback &callback);
    void connect(const ConnectOptions &connOpt);
    void disconnect(const DisconnectOptions &disconnOpt);
    void publish(const QString &topic, const Message &message, ActionCallback &callback);
    void subscribe(const QString &topic, int qos, ActionCallback &callback);
    void unsubscribe(const QString &topic, ActionCallback &callback);

private:
    MQTTAsync m_handle;
};

} // end of namespace linquitto


#endif // PROTECTABLEASYNCCLIENT_H
