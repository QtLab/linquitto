#include <QString>
#include <QtTest>
#include <QSignalSpy>

#include <mqtt/token.h>

#include "../../unsubscribeactionlistener.h"

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
    void emitTests(QSignalSpy &spy, QString expectedTopic);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void emitSuccessWithToken();
    void emitSuccessWithoutToken();
    void emitFailureWithToken();
    void emitFailureWithoutToken();
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

void Actionlistenertests::emitSuccessWithToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::success);
    const DummyToken token("hello");
    listener.on_success(token);
    emitTests(spy, "hello");
}

void Actionlistenertests::emitSuccessWithoutToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::success);
    const DummyToken token;
    listener.on_success(token);
    emitTests(spy, "unknown");
}

void Actionlistenertests::emitFailureWithToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::failure);
    const DummyToken token("hello");
    listener.on_failure(token);
    emitTests(spy, "hello");
}

void Actionlistenertests::emitFailureWithoutToken()
{
    UnsubscribeActionListener listener;
    QSignalSpy spy(&listener, &UnsubscribeActionListener::failure);
    const DummyToken token;
    listener.on_failure(token);
    emitTests(spy, "unknown");
}

void Actionlistenertests::emitTests(QSignalSpy &spy, QString expectedTopic)
{
    QCOMPARE(spy.count(), 1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).type(), QVariant::String);
    QCOMPARE(arguments.at(0).toString(), expectedTopic);
}

QTEST_APPLESS_MAIN(Actionlistenertests)

#include "tst_actionlistenertests.moc"
