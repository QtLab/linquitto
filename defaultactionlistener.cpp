#include "defaultactionlistener.h"
#include "mqtt/token.h"

#include <qdebug.h>

DefaultActionListener::DefaultActionListener(QString const &action,
                                             QObject *parent) :
    QObject(parent),
    m_actionDescription(action)
{}

void DefaultActionListener::on_failure(const mqtt::itoken &asyncActionToken)
{
    qDebug() << "on_failure: token "
             << asyncActionToken.get_message_id()
             << " has failed on" << m_actionDescription << "!";
    emit failure();
}

void DefaultActionListener::on_success(const mqtt::itoken &asyncActionToken)
{
    qDebug() << "DefaultActionListener::on_success";
    if(asyncActionToken.is_complete()) {
        qDebug() << "on_success: "
                 << m_actionDescription << " is complete.";
    } else {
        qDebug() << "on_success: "
                 << m_actionDescription << " is not complete yet.";
    }
    emit success();
}

