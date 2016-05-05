#include "unsubscribeactionlistener.h"

#include <QDebug>
#include <mqtt/token.h>

UnsubscribeActionListener::UnsubscribeActionListener(QObject *parent) :
    QObject(parent)
{}

void UnsubscribeActionListener::on_failure(const mqtt::itoken &asyncActionToken)
{
    auto topics = asyncActionToken.get_topics();
    if(topics.empty()) {
        qDebug() << "Failed to unsubscribe to unknown topic";
        emit failure(QString("unknown"));
    } else {
        // take only the first topic
        QString topic(topics.at(0).c_str());
        emit failure(topic);
    }
}

void UnsubscribeActionListener::on_success(const mqtt::itoken &asyncActionToken)
{
    auto topics = asyncActionToken.get_topics();
    if(topics.empty()) {
        qDebug() << "Unsubscribed to unknown topic";
        emit success(QString("unknown"));
    } else {
        // take only the first topic
        QString topic(topics.at(0).c_str());
        emit success(topic);
    }
}

