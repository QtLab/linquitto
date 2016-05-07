#include <QString>
#include <QtTest>
#include <QSignalSpy>

#include <mqtt/token.h>

#include "../../unsubscribeactionlistener.h"
#include "../../defaultactionlistener.h"

class DummyToken : public mqtt::itoken
{
public:
    explicit DummyToken();
    explicit DummyToken(std::__cxx11::string const &topic);

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

    std::vector<std::__cxx11::string> m_topics;
};

DummyToken::DummyToken() : m_topics()
{}

DummyToken::DummyToken(std::__cxx11::string const &topic) {
    m_topics.push_back(topic);
}

class Actionlistenertests : public QObject
{
    Q_OBJECT

public:
    Actionlistenertests();

private:
    void emitTests(QSignalSpy &spy, QString expectedTopic, const char *testname);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void emitUnsubscribeSuccessWithToken();
    void emitUnsubscribeSuccessWithoutToken();
    void emitUnsubscribeFailureWithToken();
    void emitUnsubscribeFailureWithoutToken();
    void emitDefaultActionSuccess();
    void emitDefaultActionFailure();
};

Actionlistenertests::Actionlistenertests()
{
}

void Actionlistenertests::initTestCase()
{

}

void Actionlistenertests::cleanupTestCase()
{
}

void Actionlistenertests::emitUnsubscribeSuccessWithToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::success);
    const DummyToken token("hello");
    listener.on_success(token);
    emitTests(spy, "hello", "emitUnsubscribeSuccessWithToken");
}

void Actionlistenertests::emitUnsubscribeSuccessWithoutToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::success);
    const DummyToken token;
    listener.on_success(token);
    emitTests(spy, "unknown", "emitUnsubscribeSuccessWithoutToken");
}

void Actionlistenertests::emitUnsubscribeFailureWithToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::failure);
    const DummyToken token("hello");
    listener.on_failure(token);
    emitTests(spy, "hello", "emitUnsubscribeFailureWithToken");
}

void Actionlistenertests::emitUnsubscribeFailureWithoutToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::failure);
    const DummyToken token;
    listener.on_failure(token);
    emitTests(spy, "unknown", "emitUnsubscribeFailureWithoutToken");
}

void Actionlistenertests::emitDefaultActionSuccess()
{
    DefaultActionListener listener("Test");
    QSignalSpy spy(&listener, SIGNAL(success()));
    const DummyToken token;
    listener.on_success(token);
    QVERIFY2(spy.count() == 1, "Success signal not emited.");
}

void Actionlistenertests::emitDefaultActionFailure()
{
    DefaultActionListener listener("Test");
    QSignalSpy spy(&listener, SIGNAL(failure()));
    const DummyToken token;
    listener.on_failure(token);
    QVERIFY2(spy.count() == 1, "Failure signal not emited.");
}

void Actionlistenertests::emitTests(QSignalSpy &spy, QString expectedTopic,
                                    const char *testname)
{
    QVERIFY2(spy.count() == 1, testname);
    auto arguments = spy.takeFirst();
    QVERIFY2(arguments.at(0).type() == QVariant::String, testname);
    QVERIFY2(arguments.at(0).toString() == expectedTopic, testname);
}

QTEST_APPLESS_MAIN(Actionlistenertests)

#include "tst_actionlistenertests.moc"
