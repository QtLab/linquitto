#include "protectableasyncclient.h"
#include "message.h"
#include "tools.h"

#include <QDebug>

using namespace linquitto;

ProtectableAsyncClient::ProtectableAsyncClient():
    ProtectableAsyncClient("tcp://localhost:1883","ProtectableAsyncTest")
{}

ProtectableAsyncClient::ProtectableAsyncClient(const QString &broker,
                                               const QString &name) :
    m_handle(nullptr),
    m_brokerUrl(broker),
    m_clientID(name)
{
    int ret = MQTTAsync_create(&m_handle,
                               broker.toStdString().c_str(),
                               name.toStdString().c_str(),
                               MQTTCLIENT_PERSISTENCE_NONE,
                               nullptr);
    if(ret != MQTTASYNC_SUCCESS) {
        throw "ProtectableAsyncClient creation failed!";
    }
}

ProtectableAsyncClient::~ProtectableAsyncClient()
{
    MQTTAsync_destroy(&m_handle);
    qDebug() << "ProtectableAsyncClient dtor.";
}

bool ProtectableAsyncClient::isConnected() const
{
    if(m_handle != nullptr) {
        return MQTTAsync_isConnected(m_handle) == 1;
    } else {
        return false;
    }
}

QString ProtectableAsyncClient::getClientId() const
{
    return m_clientID;
}

QString ProtectableAsyncClient::getBrokerUrl() const
{
    return m_brokerUrl;
}

void ProtectableAsyncClient::setCallback(EventCallback &callback)
{
    MQTTAsync_setCallbacks(m_handle, &callback,
                           tools::connectionLostCallback,
                           tools::messageArrivedCallback,
                           tools::deliveryCompleteCallback);
}

void ProtectableAsyncClient::connect(const ConnectOptions &connOpt)
{
    int code = MQTTAsync_connect(m_handle, connOpt.getRawOptions());
    if(code > 0) {
        qDebug() << "Can't connect! An error occured. [" << code << "]";
    }
}

void ProtectableAsyncClient::disconnect(const DisconnectOptions &disconnOpt)
{
    MQTTAsync_disconnect(m_handle, disconnOpt.getRawOptions());
}

void ProtectableAsyncClient::publish(const QString &topic, const Message &message, ResponseOptions &responseOpt)
{
    std::string destTopic = topic.toStdString();
    MQTTAsync_sendMessage(m_handle,
                          destTopic.c_str(),
                          message.getRawMessage(),
                          responseOpt.getRawOptions());
}

void ProtectableAsyncClient::subscribe(const QString &topic, int qos, ResponseOptions &responseOpt)
{
    std::string destTopic = topic.toStdString();
    MQTTAsync_subscribe(m_handle, destTopic.c_str(), qos, responseOpt.getRawOptions());
}

void ProtectableAsyncClient::unsubscribe(const QString &topic, ResponseOptions &responseOpt)
{
    std::string destTopic = topic.toStdString();
    MQTTAsync_unsubscribe(m_handle, destTopic.c_str(), responseOpt.getRawOptions());
}

