#ifndef ACTIONCALLBACK_H
#define ACTIONCALLBACK_H

#include <QString>

namespace linquitto {

class ActionCallback
{
public:
    ActionCallback();
    virtual ~ActionCallback() {}

    virtual void onSuccess() = 0;
    virtual void onFailure(int errorCode, QString errorMessage) = 0;
};

} // namespace linquitto

#endif // ACTIONCALLBACK_H
