#ifndef TOOLS
#define TOOLS

extern "C" {
#include <MQTTAsync.h>
}

namespace linquitto {

namespace tools {

/*!
 * \brief actionCallback_onSuccess will be called when a connection was established.
 * \param context is a instance of a class derived from ActionCallback.
 *
 * onSuccess encapsulates the static cast from a void pointer (context)
 * to a ActionCallback pointer.
 * MQTTAsync_succesData will not be processed, it stands only to provide
 * the expected MQTTAsync_onSuccess callback interface.
 */
void actionCallback_onSuccess(void *context, MQTTAsync_successData *);

/*!
 * \brief actionCallback_onFailure will be called when a connection can't be established.
 * \param context is a instance of a class derived from ActionCallback
 * \param data contains the error code and error message
 *
 * onFailure encapsulates the static cast from a void pointer (context)
 * to a ActionCallback pointer and transfers the data to
 * the ActionCallback instance.
 */
void actionCallback_onFailure(void *context, MQTTAsync_failureData *data);

void connectionLostCallback(void* context, char* cause);
int messageArrivedCallback(void *context,
                            char* topicName, int,
                            MQTTAsync_message* message);
void deliveryCompleteCallback(void *context, MQTTAsync_token token);

} // namespace tools

} // namespace linquitto

#endif // TOOLS

