#include "connection.h"

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENTID("SyncPublisher");

Connection::Connection(QObject *parent) : QObject(parent),
    m_client(ADDRESS, CLIENTID)
{
    m_client.set_callback(*this);
}

void Connection::connectWithServer()
{
    if(m_client.is_connected()) {
        emit logIt("Already connected!");
    } else {
        emit logIt("Connecting...");
        mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(20);
        connOpts.set_clean_session(true);
        m_client.connect(connOpts);
        if(m_client.is_connected()) {
            emit logIt("Connected.");
            sendMessageToServer("Testing the connection.");
        } else {
            emit logIt("Not yet connected.");
        }
    }

}

void Connection::disconnectFromServer()
{
    if(m_client.is_connected()) {
        emit logIt("Disconnecting...");
        m_client.disconnect();
        if(m_client.is_connected()) {
            emit logIt("Not yet disconnected.");
        } else {
            emit logIt("Disconnected.");
        }
    } else {
        emit logIt("Was not connected!");
    }
}

void Connection::sendMessageToServer(QString message)
{
    const std::string topic = "hello";
    mqtt::message_ptr msg = std::make_shared<mqtt::message>(message.toStdString());
    msg->set_qos(1);
    m_client.publish(topic, msg);
}

void Connection::connection_lost(const std::__cxx11::string &cause)
{
    emit connectionLost(QString(cause.c_str()));
    emit logIt(QString(cause.c_str()));
}

void Connection::message_arrived(const std::__cxx11::string &topic, mqtt::message_ptr msg)
{
    emit messageArrived(QString(topic.c_str()),
                        QString(msg->get_payload().c_str()));
    emit logIt("Message arrived.");

}

void Connection::delivery_complete(mqtt::idelivery_token_ptr tok)
{
    emit deliveryComplete(QString(tok->get_message()->get_payload().c_str()));
    emit logIt("Message delivered.");
}

