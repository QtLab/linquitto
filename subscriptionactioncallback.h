#ifndef SUBSCRIPTIONACTIONCALLBACK_H
#define SUBSCRIPTIONACTIONCALLBACK_H

#include <QObject>
#include "actioncallback.h"

namespace linquitto {

class SubscriptionActionCallback : public QObject, public ActionCallback
{
    Q_OBJECT
public:
    explicit SubscriptionActionCallback(const QString &topic, QObject *parent = 0);
    void setTopic(const QString &topic) {m_topic = topic;}

signals:
    void success(const QString &topic);
    void failure(const QString &topic, int errorCode, const QString &errorMessage);

public slots:

private:
    QString m_topic;

    // ActionCallback interface
public:
    void onSuccess() override;
    void onFailure(int errorCode, QString errorMessage) override;
};

} // namespace linquitto

#endif // SUBSCRIPTIONACTIONCALLBACK_H
