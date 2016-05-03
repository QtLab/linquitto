#include "asyncconnection.h"

#include <QDebug>

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENTID("AsyncPublisher");

AsyncConnection::AsyncConnection(QObject *parent) :
    QObject(parent),
    m_client(ADDRESS, CLIENTID),
    m_connectListener(QString("connecting")),
    m_disconnectListener(QString("disconnecting"))
{
    connect(&m_connectListener, SIGNAL(success()), this, SIGNAL(connected()));
    connect(&m_disconnectListener, SIGNAL(success()), this, SIGNAL(disconnected()));
}

void AsyncConnection::connectWithServer()
{
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);
    if(m_client.is_connected()) {
        qDebug() << "AsyncConnection::connectWithServer: already connected!";
    } else {
        m_client.connect(connOpts, nullptr, m_connectListener);
    }
}

void AsyncConnection::disconnectFromServer()
{
    if(m_client.is_connected()) {
        m_client.disconnect(nullptr, m_disconnectListener);
    } else {
        qDebug() << "AsyncConnection::disconnectFromServer: not connected!";
    }
}

void AsyncConnection::sendMessage(const QString &message)
{
    qDebug() << "AsyncConnection::sendMessage: " << message;
}
