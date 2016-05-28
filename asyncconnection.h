#ifndef ASYNCCONNECTION_H
#define ASYNCCONNECTION_H

#include <QObject>

#include <mqtt/async_client.h>

#include "defaultactionlistener.h"
#include "unsubscribeactionlistener.h"
#include "defaultcallback.h"
#include "protectableasyncclient.h"
#include "defaulteventcallback.h"
#include "defaultactioncallback.h"
#include "subscriptionactioncallback.h"

class AsyncConnection : public QObject
{
    Q_OBJECT
public:
     explicit AsyncConnection(std::unique_ptr<linquitto::ProtectableAsyncClient> client,
                              QObject *parent = 0);
    ~AsyncConnection();

    void connectWithServer();
    void disconnectFromServer();
    void publishMessage(QString const &topic, QString const &message);
    void subscribeToTopic(QString const &topic);
    void unsubscribeFromTopic(QString const &topic);
    bool isConnectedWithServer() const {return m_client->isConnected();}
    QString getClientId() const {return m_client->getClientId();}
    QString getBrokerUrl() const {return m_client->getBrokerUrl();}

signals:
    void connected();
    void connectFailed(const QString &cause);
    void disconnected();
    void disconnectFailed(const QString &cause);
    void published();
    void subscribed(const QString &topic);
    void unsubscribed(const QString &topic);
    void messageArrived(QString topic, QString message);
    void connectionLost(QString cause);

public slots:

private:
    /*
     * using polymorphism and dependency injection to make AsyncConnection
     * testable and configurable
     * m_client is now only dependend on an abstraction
     * (i.e.: linquitto::ProtectableAsyncClient)
     * the injection happens through the ctor
     */
    std::unique_ptr<linquitto::ProtectableAsyncClient> m_client;

    // Actionlistener
    linquitto::DefaultActionCallback m_connectActionCallback;
    linquitto::DefaultActionCallback m_disconnectActionCallback;
    linquitto::DefaultActionCallback m_publishActionCallback;
    linquitto::SubscriptionActionCallback m_subscribeActionCallback;
    linquitto::SubscriptionActionCallback m_unsubscribeActionCallback;

    // Callback
    linquitto::DefaultEventCallback m_callback;
};

#endif // ASYNCCONNECTION_H
