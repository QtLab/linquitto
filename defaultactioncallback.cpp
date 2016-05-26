#include "defaultactioncallback.h"

#include <QDebug>

using namespace linquitto;

DefaultActionCallback::DefaultActionCallback(const QString &name) :
    ActionCallback(),
    m_name(name)
{}

// runs in a non UI thread
void DefaultActionCallback::onSuccess()
{
    qDebug() << m_name << "was successfull.";
}

// runs in a non UI thread
void DefaultActionCallback::onFailure(int errorCode, QString errorMessage)
{
    qDebug() << m_name << "failed:" << "code =" << errorCode << errorMessage;
}

