#include "disconnectoptions.h"
#include "actioncallback.h"
#include "tools.h"

#include <QDebug>

using namespace linquitto;

DisconnectOptions::DisconnectOptions() :
    m_opts(MQTTAsync_disconnectOptions_initializer)
{
    m_opts.onSuccess = tools::actionCallback_onSuccess;
    m_opts.onFailure = tools::actionCallback_onFailure;
}

void DisconnectOptions::setTimeout(int timeout)
{
    m_opts.timeout = timeout;
}

void DisconnectOptions::setActionCallback(ActionCallback *callback)
{
    m_opts.context = callback;
}

/*!
 * \brief DisconnectOptions::getRawOptions
 * \return pointer to MQTTAsync_disconnectOptions structure
 *
 * Never delete this pointer by yourself, it is a non owning pointer!
 */
const MQTTAsync_disconnectOptions * DisconnectOptions::getRawOptions() const
{
    return &m_opts;
}

