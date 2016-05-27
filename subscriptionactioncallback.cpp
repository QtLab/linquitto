#include "subscriptionactioncallback.h"

using namespace linquitto;

SubscriptionActionCallback::SubscriptionActionCallback(
        const QString &topic, QObject *parent) :
    QObject(parent),
    m_topic(topic)
{}


void SubscriptionActionCallback::onSuccess()
{
    emit success(m_topic);
}

void SubscriptionActionCallback::onFailure(int errorCode, QString errorMessage)
{
    emit failure(m_topic, errorCode, errorMessage);
}
