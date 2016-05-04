#ifndef ASYNCCONNECTION_H
#define ASYNCCONNECTION_H

#include <QObject>

#include <mqtt/async_client.h>

#include "defaultactionlistener.h"

class AsyncConnection : public QObject
{
    Q_OBJECT
public:
     explicit AsyncConnection(QObject *parent = 0);
    ~AsyncConnection() {}

    void connectWithServer();
    void disconnectFromServer();
    void sendMessage(QString const &message);
    bool isConnectedWithServer() const {return m_client.is_connected();}

signals:
    void connected();
    void disconnected();

public slots:

private:
    mqtt::async_client m_client;
    DefaultActionListener m_connectListener;
    DefaultActionListener m_disconnectListener;

};

#endif // ASYNCCONNECTION_H
