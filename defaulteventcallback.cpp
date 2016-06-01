#include "defaulteventcallback.h"

using namespace linquitto;

DefaultEventCallback::DefaultEventCallback(QObject *parent) :
    QObject(parent)
{}

void DefaultEventCallback::onMessageArrived(QString topic, QByteArray payload)
{
    emit messageArrived(topic, payload);
}

void DefaultEventCallback::onDeliveryComplete(int id)
{
    emit deliveryComplete(id);
}

void DefaultEventCallback::onConnectionLost(QString cause)
{
    emit connectionLost(cause);
}

