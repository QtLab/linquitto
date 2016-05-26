#ifndef DEFAULTACTIONCALLBACK_H
#define DEFAULTACTIONCALLBACK_H

#include <QObject>
#include "actioncallback.h"

namespace linquitto {

class DefaultActionCallback : public QObject, public ActionCallback
{
    Q_OBJECT

public:
    explicit DefaultActionCallback(const QString &name, QObject *parent = 0);

signals:
    void success();
    void failure(int errorCode, const QString &errorMessage);

    // ActionCallback interface
public:
    void onSuccess();
    void onFailure(int errorCode, QString errorMessage);

private:
    QString m_name;
};

} // namespace linquitto



#endif // DEFAULTACTIONCALLBACK_H
