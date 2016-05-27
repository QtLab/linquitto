#ifndef CONNECTIONCONTENT_H
#define CONNECTIONCONTENT_H

#include <QWidget>

#include "asyncconnection.h"
#include "protectableasyncclient.h"

namespace Ui {
class ConnectionContent;
}

class ConnectionContent : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionContent(QWidget *parent = 0);
    ConnectionContent(QString &brokerUrl, QString &clientName, QWidget *parent = 0);
    ~ConnectionContent();

signals:
    void log(QString);

public slots:
    void onConnect();
    void onDisconnect();
    void onPublish();
    void onSubscribe();
    void onUnsubscribe();
    void connectionEstablished();
    void disconnected();
    void connectionHasPublished();
    void connectionHasSubscribed(const QString &topic);
    void connectionHasUnsubscribed(const QString &topic);
    void connectionLost(QString cause);
    void messageArrived(QString topic, QString message);

private:
    void connectSignals();
    bool notAlreadySubscribed(const QString &topic) const;

private:
    Ui::ConnectionContent *ui;
    AsyncConnection m_connection;
};

#endif // CONNECTIONCONTENT_H
