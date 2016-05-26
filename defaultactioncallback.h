#ifndef DEFAULTACTIONCALLBACK_H
#define DEFAULTACTIONCALLBACK_H

#include "actioncallback.h"

namespace linquitto {

class DefaultActionCallback : public ActionCallback
{
public:
    explicit DefaultActionCallback(const QString &name);

    // ActionCallback interface
public:
    void onSuccess();
    void onFailure(int errorCode, QString errorMessage);

private:
    QString m_name;
};

} // namespace linquitto



#endif // DEFAULTACTIONCALLBACK_H
