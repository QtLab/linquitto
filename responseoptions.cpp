#include "responseoptions.h"
#include "actioncallback.h"
#include "tools.h"

using namespace linquitto;

ResponseOptions::ResponseOptions() :
    m_opts(MQTTAsync_responseOptions_initializer)
{
    m_opts.onSuccess = tools::actionCallback_onSuccess;
    m_opts.onFailure = tools::actionCallback_onFailure;
}

void ResponseOptions::setActionCallback(ActionCallback *callback)
{
    m_opts.context = callback;
}

MQTTAsync_responseOptions * ResponseOptions::getRawOptions()
{
    return &m_opts;
}

