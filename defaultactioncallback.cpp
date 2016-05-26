#include "defaultactioncallback.h"

#include <QDebug>

using namespace linquitto;

DefaultActionCallback::DefaultActionCallback(const QString &name, QObject *parent) :
    QObject(parent),
    ActionCallback(),
    m_name(name)
{}

// runs in a non UI thread
void DefaultActionCallback::onSuccess()
{
    emit success();
    qDebug() << m_name << "was successfull.";
}

// runs in a non UI thread
void DefaultActionCallback::onFailure(int errorCode, QString errorMessage)
{
    emit failure(errorCode, errorMessage);
    qDebug() << m_name << "failed:" << "code =" << errorCode << errorMessage;
}

