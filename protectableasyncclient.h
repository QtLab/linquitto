#ifndef PROTECTABLEASYNCCLIENT_H
#define PROTECTABLEASYNCCLIENT_H

#include "asyncclient.h"
#include "connectoptions.h"
#include "disconnectoptions.h"
#include "responseoptions.h"

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
    void publish(const QString &topic, const Message &message, ResponseOptions &responseOpt);
    void subscribe(const QString &topic, int qos,  ResponseOptions &responseOpt);
    void unsubscribe(const QString &topic,  ResponseOptions &responseOpt);

private:

private:
    MQTTAsync m_handle;
};

} // end of namespace linquitto


#endif // PROTECTABLEASYNCCLIENT_H
