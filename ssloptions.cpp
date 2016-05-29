#include "ssloptions.h"

using namespace linquitto;

SSLOptions::SSLOptions() :
    m_opts(MQTTAsync_SSLOptions_initializer)
{}

void SSLOptions::setTrustStore(const QString &filePath)
{
    m_trustStoreFile = filePath.toStdString();
    m_opts.trustStore = m_trustStoreFile.c_str();
}

void SSLOptions::setKeyStore(const QString &filePath)
{
    m_keyStoreFile = filePath.toStdString();
    m_opts.keyStore = m_keyStoreFile.c_str();
}

void SSLOptions::setPrivateKey(const QString &filePath)
{
    m_privateKeyFile = filePath.toStdString();
    m_opts.privateKey = m_privateKeyFile.c_str();
}

void SSLOptions::setPrivateKeyPassword(const QString &password)
{
    m_privateKeyPassword = password.toStdString();
    m_opts.privateKeyPassword = m_privateKeyPassword.c_str();
}

void SSLOptions::setCipherSuites(const QString &cipherSuites)
{
    m_cipherSuites = cipherSuites.toStdString();
    m_opts.enabledCipherSuites = m_cipherSuites.c_str();
}

void SSLOptions::enableServerCertAuth(bool enabled)
{
    m_opts.enableServerCertAuth = enabled;
}


/*!
 * \brief SSLOptions::getRawOptions
 * \return pointer to MQTTAsync_SSLOptions structure
 *
 * Never delete this pointer by yourself, it is a non owning pointer!
 */
MQTTAsync_SSLOptions *SSLOptions::getRawOptions()
{
    return &m_opts;
}
