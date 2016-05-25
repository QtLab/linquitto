#include "disconnectoptions.h"

using namespace linquitto;

DisconnectOptions::DisconnectOptions() :
    m_opts(MQTTAsync_disconnectOptions_initializer)
{}

void DisconnectOptions::setContext(void *context)
{
    m_opts.context = context;
}

void DisconnectOptions::setTimeout(int timeout)
{
    m_opts.timeout = timeout;
}

void DisconnectOptions::setOnSuccessCallback(MQTTAsync_onSuccess *callback)
{
    m_opts.onSuccess = callback;
}

void DisconnectOptions::setOnFailureCallback(MQTTAsync_onFailure *callback)
{
    m_opts.onFailure = callback;
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

