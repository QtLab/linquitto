#include "tools.h"
#include "actioncallback.h"

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
        callback->onFailure(data->code, QString(data->message));
    } else {
        qDebug() << "context was empty.";
    }
}
