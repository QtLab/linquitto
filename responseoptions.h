#ifndef RESPONSEOPTIONS_H
#define RESPONSEOPTIONS_H

extern "C" {
#include <MQTTAsync.h>
}

namespace linquitto {

class ActionCallback;

class ResponseOptions
{
public:
    ResponseOptions();

    void setActionCallback(ActionCallback *callback);
    MQTTAsync_responseOptions * getRawOptions();

private:
    MQTTAsync_responseOptions m_opts;

};

} // namespace linquitto

#endif // RESPONSEOPTIONS_H
