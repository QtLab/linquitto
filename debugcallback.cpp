#include "debugcallback.h"

#include <QDebug>

DebugCallback::DebugCallback(QObject *parent) : QObject(parent)
{

}

void DebugCallback::connection_lost(const std::__cxx11::string &cause)
{
    qDebug() << "DebugCallback::connection_lost: " << cause.c_str();
}

void DebugCallback::message_arrived(const std::__cxx11::string &topic, mqtt::message_ptr msg)
{
    qDebug() << "DebugCallback::message_arrived: "
             << "Topic=" << topic.c_str()
             << "Payload=" << msg->get_payload().c_str();
}

void DebugCallback::delivery_complete(mqtt::idelivery_token_ptr tok)
{
    qDebug() << "DebugCallback::delivery_complete"
             << tok->get_message()->get_payload().c_str();
}

