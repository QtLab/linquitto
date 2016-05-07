#include <QString>
#include <QtTest>
#include <QSignalSpy>

#include <mqtt/message.h>
#include <mqtt/token.h>

#include "../../defaultcallback.h"

class DummyDeliveryToken : public mqtt::idelivery_token
{
public:
    explicit DummyDeliveryToken(std::string message);

    // idelivery_token interface
public:
    mqtt::message_ptr get_message() const;

    // itoken interface
public:
    mqtt::iaction_listener *get_action_callback() const {return nullptr;}
    mqtt::iasync_client *get_client() const {return nullptr;}
    int get_message_id() const {return 0;}
    const std::vector<std::__cxx11::string> &get_topics() const {return m_topics;}
    void *get_user_context() const {return nullptr;}
    bool is_complete() const {return true;}
    void set_action_callback(mqtt::iaction_listener &) {}
    void set_user_context(void *) {}
    void wait_for_completion() {}
    void wait_for_completion(long ) {}

private:
    std::vector<std::__cxx11::string> m_topics;
    std::__cxx11::string m_message;
};

DummyDeliveryToken::DummyDeliveryToken(std::__cxx11::string message):
    m_topics(),
    m_message(message)
{}

mqtt::message_ptr DummyDeliveryToken::get_message() const
{
    const std::string message("hallo delivery");
    mqtt::message::ptr_t msg =
            std::make_shared<mqtt::message>(message);
    return msg;
}

class Callbacktests : public QObject
{
    Q_OBJECT

public:
    Callbacktests();

private Q_SLOTS:
    void defaultConnectionLost();
    void defaultMessageArrived();
    void defaultDeliveryComplete();
};

Callbacktests::Callbacktests()
{
}

void Callbacktests::defaultConnectionLost()
{
    DefaultCallback callback;
    QSignalSpy spy(&callback, &DefaultCallback::connectionLost);
    std::string cause("Server not available");
    callback.connection_lost(cause);
    QVERIFY(spy.count() == 1);
    auto arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::String);
    QVERIFY(arguments.at(0).toString() == "Server not available");
}

void Callbacktests::defaultMessageArrived()
{
    DefaultCallback callback;
    QSignalSpy spy(&callback, &DefaultCallback::messageArrived);
    const std::string topic("test");
    const std::string message = "hello test";
    mqtt::message::ptr_t msg =
            std::make_shared<mqtt::message>(message);
    callback.message_arrived(topic, msg);
    QVERIFY(spy.count() == 1);
    auto arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::String);
    QVERIFY(arguments.at(0).toString() == "test");
    QVERIFY(arguments.at(1).type() == QVariant::String);
    QVERIFY(arguments.at(1).toString() == "hello test");
}

void Callbacktests::defaultDeliveryComplete()
{
    DefaultCallback callback;
    QSignalSpy spy(&callback, &DefaultCallback::deliveryComplete);
    auto token_ptr = std::shared_ptr<DummyDeliveryToken>(
                new DummyDeliveryToken("hallo delivery"));
    callback.delivery_complete(token_ptr);
    QVERIFY(spy.count() == 1);
}


QTEST_APPLESS_MAIN(Callbacktests)

#include "tst_callbacktests.moc"
