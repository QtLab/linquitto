#include "connectoptions.h"
#include "actioncallback.h"
#include "tools.h"

#include <QDebug>

using namespace linquitto;

ConnectOptions::ConnectOptions() :
    m_opts(MQTTAsync_connectOptions_initializer)
{
    m_opts.onSuccess = tools::actionCallback_onSuccess;
    m_opts.onFailure = tools::actionCallback_onFailure;
}

ConnectOptions::~ConnectOptions()
{
    if(m_opts.serverURIcount > 0) {
        deleteURIBuffer();
    }
}

/*!
 * \brief ConnectOptions::getRawOptions
 * \return pointer to a MQTTAsync_connectionOption structure
 *
 * Never delete this pointer by yourself, it is a non owning pointer!
 */
const MQTTAsync_connectOptions *ConnectOptions::getRawOptions() const
{
    return &m_opts;
}

void ConnectOptions::setCleanSession(bool clean)
{
    m_opts.cleansession = clean;
    qDebug() << "cleansession =" << m_opts.cleansession;
}

void ConnectOptions::setKeepAlive(int seconds)
{
    m_opts.keepAliveInterval = seconds;
}

void ConnectOptions::setMaxInflight(int maxNumberOfMessages)
{
    m_opts.maxInflight = maxNumberOfMessages;
}

void ConnectOptions::setWillOptions(MQTTAsync_willOptions *willOptions)
{
    m_opts.will = willOptions;
}

void ConnectOptions::setUsername(const QString &username)
{
    m_username = username.toStdString();
    m_opts.username = m_username.c_str();
}

void ConnectOptions::setPassword(const QString &password)
{
    m_password = password.toStdString();
    m_opts.password = m_password.c_str();
}

void ConnectOptions::setSSLOptions(MQTTAsync_SSLOptions *sslOptions)
{
    m_opts.ssl = sslOptions;
}

void ConnectOptions::setActionCallback(ActionCallback *callback)
{
    m_opts.context = callback;
}

// TODO: should be thoroughly tested, possible buffer overrun or memory leak!
void ConnectOptions::setServerURIs(std::list<QString> &serverURIs)
{
    if(m_opts.serverURIcount > 0) {
        clearURIBuffer();
        deleteURIBuffer();
    }
    char** buffer = createURIBuffer(serverURIs.size());

    size_t index = 0;
    for(auto uri: serverURIs) {
        size_t bufferLength = strlen(uri.toStdString().c_str()) + 1;
        char * str = new char[bufferLength];
        strncpy(str, uri.toStdString().c_str(), bufferLength);
        buffer[index] = str;
        ++index;
    }
    m_opts.serverURIs = buffer;
    m_opts.serverURIcount = serverURIs.size();
}

char** ConnectOptions::createURIBuffer(size_t size)
{
    return new char*[size];
}

void ConnectOptions::clearURIBuffer()
{
    for(int i=0; i < m_opts.serverURIcount; ++i) {
        delete [] m_opts.serverURIs[i];
    }
}

void ConnectOptions::deleteURIBuffer()
{
    if(m_opts.serverURIcount > 0) {
        delete [] m_opts.serverURIs;
        m_opts.serverURIs = nullptr;
        m_opts.serverURIcount = 0;
    }
}
