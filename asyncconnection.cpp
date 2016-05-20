#include "asyncconnection.h"

#include <QDebug>

AsyncConnection::AsyncConnection(std::unique_ptr<mqtt::iasync_client> client,
                                 QObject *parent) :
    QObject(parent),
    m_client(std::move(client)),
    m_connectListener(QString("connecting")),
    m_disconnectListener(QString("disconnecting")),
    m_publishListener(QString("publishing")),
    m_subscribeListener(QString("subscribing")),
    m_unsubscribeListener(),
    m_callback()
{
    connect(&m_connectListener, SIGNAL(success()), this, SIGNAL(connected()));
    connect(&m_disconnectListener, SIGNAL(success()), this, SIGNAL(disconnected()));
    connect(&m_publishListener, SIGNAL(success()), this, SIGNAL(published()));
    connect(&m_subscribeListener, SIGNAL(success()), this, SIGNAL(subscribed()));
    connect(&m_unsubscribeListener, SIGNAL(success(QString)),
            this, SIGNAL(unsubscribed(QString)));
    connect(&m_callback, SIGNAL(connectionLost(QString)),
            this, SIGNAL(connectionLost(QString)));
    connect(&m_callback, SIGNAL(messageArrived(QString,QString)),
            this, SIGNAL(messageArrived(QString,QString)));
    m_client->set_callback(m_callback);
}

AsyncConnection::~AsyncConnection()
{
    qDebug() << QString(m_client->get_client_id().c_str()) + ": AsyncConnection dtor called.";
}

void AsyncConnection::connectWithServer()
{
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    if(m_client->is_connected()) {
        qDebug() << "AsyncConnection::connectWithServer: already connected!";
    } else {
        m_client->connect(connOpts, nullptr, m_connectListener);
    }
}

void AsyncConnection::disconnectFromServer()
{
    if(m_client->is_connected()) {
        m_client->disconnect(nullptr, m_disconnectListener);
    } else {
        qDebug() << "AsyncConnection::disconnectFromServer: not connected!";
    }
}

void AsyncConnection::publishMessage(const QString &topic, const QString &message)
{
    if(m_client->is_connected()) {
        qDebug() << "AsyncConnection::sendMessage: " << topic << message;
        mqtt::message_ptr pubmsg =
                std::make_shared<mqtt::message>(message.toStdString().c_str());
        m_client->publish(topic.toStdString(), pubmsg, nullptr, m_publishListener);
    } else {
        qDebug() << "AsyncConnection::sendMessage: failed, not connected!";
    }
}

void AsyncConnection::subscribeToTopic(const QString &topic)
{
    if(m_client->is_connected()) {
        qDebug() << "AsyncConnection::subscribeToTopic: " << topic;
        m_client->subscribe(topic.toStdString(), 1, nullptr, m_subscribeListener);
    } else {
        qDebug() << "AsyncConnection::subscribeToTopic: failed, not connected!";
    }
}

void AsyncConnection::unsubscribeFromTopic(const QString &topic)
{
    if(m_client->is_connected()) {
        qDebug() << "AsyncConnection::unsubscribeFromTopic: " << topic;
        m_client->unsubscribe(topic.toStdString(), nullptr, m_unsubscribeListener);
    } else {
        qDebug() << "AsyncConnection::unsubscribeFromTopic: failed, not connected!";
    }
}
