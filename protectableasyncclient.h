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
    ProtectableAsyncClient(const QString &broker, const QString &name);
    ~ProtectableAsyncClient();

    // AsyncClient interface
public:
    bool isConnected() const override;
    QString getClientId() const override;
    QString getBrokerUrl() const override;
    bool sslEnabled() const override;
    void setCallback(EventCallback &callback) override;
    void connect(const ConnectOptions &connOpt) override;
    void disconnect(const DisconnectOptions &disconnOpt) override;
    void publish(const QString &topic, const Message &message, ResponseOptions &responseOpt) override;
    void subscribe(const QString &topic, int qos,  ResponseOptions &responseOpt) override;
    void unsubscribe(const QString &topic,  ResponseOptions &responseOpt) override;

private:
    MQTTAsync m_handle;
    QString m_brokerUrl;
    QString m_clientID;
};

} // end of namespace linquitto


#endif // PROTECTABLEASYNCCLIENT_H
