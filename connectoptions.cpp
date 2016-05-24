#include "connectoptions.h"

using namespace linquitto;

ConnectOptions::ConnectOptions() :
    m_opts(MQTTAsync_connectOptions_initializer)
{}

MQTTAsync_connectOptions *ConnectOptions::getOptions()
{
    return &m_opts;
}

