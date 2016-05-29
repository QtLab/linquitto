#ifndef CONNECTOPTIONS_H
#define CONNECTOPTIONS_H

extern "C" {
#include <MQTTAsync.h>
}

#include <QString>
#include <list>
#include "ssloptions.h"

namespace linquitto {

class ActionCallback;

class ConnectOptions
{
public:
    ConnectOptions();
    ~ConnectOptions();

    const MQTTAsync_connectOptions *getRawOptions() const;
    void setCleanSession(bool clean);
    void setKeepAlive(int seconds);
    void setMaxInflight(int maxNumberOfMessages);
    void setWillOptions(MQTTAsync_willOptions *willOptions);
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setSSLOptions(SSLOptions &sslOptions);
    void setActionCallback(ActionCallback *callback);
    void setServerURIs(std::list<QString> &serverURIs);

private:
    char** createURIBuffer(size_t size);
    void clearURIBuffer();
    void deleteURIBuffer();

private:
    MQTTAsync_connectOptions m_opts;
    std::string m_username;
    std::string m_password;
};

} // end of namespace linquitto


#endif // CONNECTOPTIONS_H
