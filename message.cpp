#include "message.h"

#include <QString>

using namespace linquitto;

Message::Message() :
    m_msg(MQTTAsync_message_initializer)
{}

void Message::setPayload(const QString &payload)
{
    m_payload.clear();
    m_payload.append(payload);
    m_msg.payload = m_payload.data();
    m_msg.payloadlen = m_payload.size() + 1;
}

void Message::setQualityOfService(int qos)
{
    m_msg.qos = qos;
}

void Message::setRetained(bool retained)
{
    m_msg.retained = retained;
}

const MQTTAsync_message *Message::getRawMessage() const
{
    return &m_msg;
}

