#ifndef DEFAULTACTIONLISTENER_H
#define DEFAULTACTIONLISTENER_H

#include <QObject>
#include <mqtt/iaction_listener.h>

class DefaultActionListener : public QObject, public mqtt::iaction_listener
{
    Q_OBJECT
public:
    explicit DefaultActionListener(QString const &action, QObject *parent = 0);

signals:
    void failure();
    void success();

public slots:

    // iaction_listener interface
public:
    void on_failure(const mqtt::itoken &asyncActionToken);
    void on_success(const mqtt::itoken &asyncActionToken);

private:
    QString m_actionDescription;
};

#endif // DEFAULTACTIONLISTENER_H
