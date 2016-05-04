#include "defaultcallback.h"

#include <QDebug>

DefaultCallback::DefaultCallback(QObject *parent) : QObject(parent)
{

}

void DefaultCallback::connection_lost(const std::__cxx11::string &cause)
{
    qDebug() << "DefaultCallback::connection_lost: " << cause.c_str();
    emit connectionLost(QString(cause.c_str()));
}

void DefaultCallback::message_arrived(const std::__cxx11::string &topic, mqtt::message_ptr msg)
{
    qDebug() << "DefaultCallback::message_arrived: "
             << "Topic=" << topic.c_str()
             << "Payload=" << msg->get_payload().c_str();
    emit messageArrived(QString(topic.c_str()),
                        QString(msg->get_payload().c_str()));
}

void DefaultCallback::delivery_complete(mqtt::idelivery_token_ptr tok)
{
    qDebug() << "DefaultCallback::delivery_complete"
             << tok->get_message()->get_payload().c_str();
}

