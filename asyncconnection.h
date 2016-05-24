#ifndef ASYNCCONNECTION_H
#define ASYNCCONNECTION_H

#include <QObject>

#include <mqtt/async_client.h>

#include "defaultactionlistener.h"
#include "unsubscribeactionlistener.h"
#include "defaultcallback.h"
#include "iasync_client.h"

class AsyncConnection : public QObject
{
    Q_OBJECT
public:
     explicit AsyncConnection(std::unique_ptr<mqtt::iasync_client> client,
                              QObject *parent = 0);
    ~AsyncConnection();

    void connectWithServer();
    void disconnectFromServer();
    void publishMessage(QString const &topic, QString const &message);
    void subscribeToTopic(QString const &topic);
    void unsubscribeFromTopic(QString const &topic);
    bool isConnectedWithServer() const {return m_client->is_connected();}
    std::string getClientId() const {return m_client->get_client_id();}

signals:
    void connected();
    void disconnected();
    void published();
    void subscribed();
    void unsubscribed(QString topic);
    void messageArrived(QString topic, QString message);
    void connectionLost(QString cause);

public slots:

private:
    /*
     * using polymorphism and dependency injection to make AsyncConnection
     * testable and configurable
     * m_client is now only dependend on an abstraction
     * (i.e.: mqtt::iasync_client)
     * the injection happens through the ctor
     */
    std::unique_ptr<mqtt::iasync_client> m_client;

    // Actionlistener
    DefaultActionListener m_connectListener;
    DefaultActionListener m_disconnectListener;
    DefaultActionListener m_publishListener;
    DefaultActionListener m_subscribeListener;
    UnsubscribeActionListener m_unsubscribeListener;

    // Callback
    DefaultCallback m_callback;
};

#endif // ASYNCCONNECTION_H
