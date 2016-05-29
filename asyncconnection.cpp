#include "asyncconnection.h"
#include "message.h"

#include <QDebug>


using namespace linquitto;
AsyncConnection::AsyncConnection(std::unique_ptr<ProtectableAsyncClient> client,
                                 QObject *parent) :
    QObject(parent),
    m_client(std::move(client)),
    m_connectActionCallback("connecting"),
    m_disconnectActionCallback("disconnecting"),
    m_publishActionCallback("publishing"),
    m_subscribeActionCallback("unknown"),
    m_unsubscribeActionCallback("unknown"),
    m_callback()
{
    connect(&m_connectActionCallback, &DefaultActionCallback::success,
            this, &AsyncConnection::connected);
    connect(&m_connectActionCallback, &DefaultActionCallback::failure,
            this, &AsyncConnection::connectFailed);

    connect(&m_disconnectActionCallback, &DefaultActionCallback::success,
            this, &AsyncConnection::disconnected);
    connect(&m_disconnectActionCallback, &DefaultActionCallback::failure,
            this, &AsyncConnection::disconnectFailed);

    connect(&m_publishActionCallback, &DefaultActionCallback::success,
            this, &AsyncConnection::published);
    connect(&m_publishActionCallback, &DefaultActionCallback::failure,
            this, &AsyncConnection::publishFailed);

    connect(&m_subscribeActionCallback, &SubscriptionActionCallback::success,
            this, &AsyncConnection::subscribed);
    connect(&m_subscribeActionCallback, &SubscriptionActionCallback::failure,
            this, &AsyncConnection::subscribeFailed);

    connect(&m_unsubscribeActionCallback, &SubscriptionActionCallback::success,
            this, &AsyncConnection::unsubscribed);
    connect(&m_unsubscribeActionCallback, &SubscriptionActionCallback::failure,
            this, &AsyncConnection::unsubscribeFailed);

    connect(&m_callback, &DefaultEventCallback::connectionLost,
            this, &AsyncConnection::connectionLost);
    connect(&m_callback, &DefaultEventCallback::messageArrived,
            this, &AsyncConnection::messageArrived);
    m_client->setCallback(m_callback);
}

AsyncConnection::~AsyncConnection()
{
    disconnectFromServer();
    qDebug() << m_client->getClientId() + ": AsyncConnection dtor called.";
}

void AsyncConnection::connectWithServer()
{
    linquitto::ConnectOptions connOptions;
    linquitto::SSLOptions sslOptions;
    if(m_client->sslEnabled()) {
        sslOptions.setTrustStore("/etc/ssl/certs/mosquitto.crt");
        connOptions.setSSLOptions(sslOptions);
    }
    connOptions.setActionCallback(&m_connectActionCallback);
    if(m_client->isConnected()) {
        qDebug() << "AsyncConnection::connectWithServer: already connected!";
    } else {
        m_client->connect(connOptions);
    }
}

void AsyncConnection::disconnectFromServer()
{
    linquitto::DisconnectOptions disconnOptions;
    disconnOptions.setActionCallback(&m_disconnectActionCallback);
    if(m_client->isConnected()) {
        m_client->disconnect(disconnOptions);
    } else {
        qDebug() << "AsyncConnection::disconnectFromServer: not connected!";
    }
}

void AsyncConnection::publishMessage(const QString &topic, const QString &message)
{
    if(m_client->isConnected()) {
        qDebug() << "AsyncConnection::sendMessage: " << topic << message;
        ResponseOptions responseOptions;
        responseOptions.setActionCallback(&m_publishActionCallback);
        Message msg;
        msg.setPayload(message);
        msg.setQualityOfService(1);
        msg.setRetained(false);
        m_client->publish(topic, msg, responseOptions);
    } else {
        qDebug() << "AsyncConnection::sendMessage: failed, not connected!";
    }
}

void AsyncConnection::subscribeToTopic(const QString &topic)
{
    if(m_client->isConnected()) {
        qDebug() << "AsyncConnection::subscribeToTopic: " << topic;
        ResponseOptions responseOptions;
        m_subscribeActionCallback.setTopic(topic);
        responseOptions.setActionCallback(&m_subscribeActionCallback);
        m_client->subscribe(topic, 1, responseOptions);
    } else {
        qDebug() << "AsyncConnection::subscribeToTopic: failed, not connected!";
    }
}

void AsyncConnection::unsubscribeFromTopic(const QString &topic)
{
    if(m_client->isConnected()) {
        qDebug() << "AsyncConnection::unsubscribeFromTopic: " << topic;
        linquitto::ResponseOptions responseOptions;
        m_unsubscribeActionCallback.setTopic(topic);
        responseOptions.setActionCallback(&m_unsubscribeActionCallback);
        m_client->unsubscribe(topic, responseOptions);
    } else {
        qDebug() << "AsyncConnection::unsubscribeFromTopic: failed, not connected!";
    }
}
