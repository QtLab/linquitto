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
    void publishMessage(QString const &topic, QString const &message);
    void subscribeToTopic(QString const &topic);
    bool isConnectedWithServer() const {return m_client.is_connected();}

signals:
    void connected();
    void disconnected();
    void published();
    void subscribed();

public slots:

private:
    mqtt::async_client m_client;
    DefaultActionListener m_connectListener;
    DefaultActionListener m_disconnectListener;
    DefaultActionListener m_publishListener;
    DefaultActionListener m_subscribeListener;
};

#endif // ASYNCCONNECTION_H
