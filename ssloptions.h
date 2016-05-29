#ifndef SSLOPTIONS_H
#define SSLOPTIONS_H

#include <QString>

extern "C" {
#include <MQTTAsync.h>
}

namespace linquitto {

class SSLOptions
{
public:
    SSLOptions();

    void setTrustStore(const QString &filePath);
    void setKeyStore(const QString &filePath);
    void setPrivateKey(const QString &filePath);
    void setPrivateKeyPassword(const QString &password);
    void setCipherSuites(const QString &cipherSuites);
    void enableServerCertAuth(bool enabled);

    MQTTAsync_SSLOptions *getRawOptions();

private:
    MQTTAsync_SSLOptions m_opts;
    std::string m_trustStoreFile;
    std::string m_keyStoreFile;
    std::string m_privateKeyFile;
    std::string m_privateKeyPassword;
    std::string m_cipherSuites;
};

} // namespace linquitto


#endif // SSLOPTIONS_H
