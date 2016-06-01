#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>

extern "C" {
#include <MQTTAsync.h>
}

namespace linquitto {

class Message
{
public:
    Message();

    void setPayload(const QString &payload);
    void setPayload(const QByteArray &payload);
    void setQualityOfService(int qos);
    void setRetained(bool retained);

    const MQTTAsync_message *getRawMessage() const;

private:
    MQTTAsync_message m_msg;
    QByteArray m_payload;
};

} // namespace linquitto

#endif // MESSAGE_H
