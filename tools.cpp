#include "tools.h"
#include "actioncallback.h"
#include "eventcallback.h"

#include <QDebug>

using namespace linquitto;

void linquitto::tools::actionCallback_onSuccess(void *context, MQTTAsync_successData *)
{
    if(context != nullptr) {
        ActionCallback *callback = static_cast<ActionCallback*>(context);
        callback->onSuccess();
    } else {
        qDebug() << "context was empty.";
    }
}

void linquitto::tools::actionCallback_onFailure(void *context, MQTTAsync_failureData *data)
{
    if(context != nullptr) {
        ActionCallback *callback = static_cast<ActionCallback*>(context);
        if(data != nullptr) {
            callback->onFailure(data->code, QString(data->message));
        } else {
            callback->onFailure(-1, "Unknown error!");
        }
    } else {
        qDebug() << "context was empty.";
    }
}

void tools::connectionLostCallback(void *context, char *cause)
{
    if(context != nullptr) {
        EventCallback *callback = static_cast<EventCallback*>(context);
        QString causeMsg;
        if(cause != nullptr) {
            causeMsg = cause;
        } else {
            causeMsg = "unknown";
        }
        callback->onConnectionLost(causeMsg);
    } else {
        qDebug() << "context was empty.";
    }
}

int tools::messageArrivedCallback(void *context,
                                  char *topicName, int,
                                  MQTTAsync_message *message)
{
    if(context != nullptr) {
        EventCallback *callback = static_cast<EventCallback*>(context);
        QString topic;
        if(topicName != nullptr) {
            topic = topicName;
        } else {
            topic = "unknown";
        }
        QByteArray payload;
        if(message != nullptr) {
            payload = static_cast<char*>(message->payload);
        }
        QString msg(payload);
        callback->onMessageArrived(topic, msg);
    } else {
        qDebug() << "context was empty.";
    }
    MQTTAsync_free(topicName);
    MQTTAsync_freeMessage(&message);
    return true;
}

void tools::deliveryCompleteCallback(void *context, MQTTAsync_token token)
{
    if(context != nullptr) {
        EventCallback *callback = static_cast<EventCallback*>(context);
        callback->onDeliveryComplete(token);
    } else {
        qDebug() << "context was empty.";
    }

}
