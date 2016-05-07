#include <QString>
#include <QtTest>
#include <QSignalSpy>

#include <mqtt/token.h>

#include "../../defaultactionlistener.h"

class DummyToken : public mqtt::itoken
{
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

class Defaultactionlistenertest : public QObject
{
    Q_OBJECT

public:
    Defaultactionlistenertest();

private Q_SLOTS:
    void emitFailure();
    void emitSuccess();
};

Defaultactionlistenertest::Defaultactionlistenertest()
{
}

void Defaultactionlistenertest::emitFailure()
{
    DefaultActionListener listener("Test");
    QSignalSpy spy(&listener, SIGNAL(failure()));
    const DummyToken token;
    listener.on_failure(token);
    QVERIFY2(spy.count() == 1, "Failure signal not emited.");
}

void Defaultactionlistenertest::emitSuccess()
{
    DefaultActionListener listener("Test");
    QSignalSpy spy(&listener, SIGNAL(success()));
    const DummyToken token;
    listener.on_success(token);
    QVERIFY2(spy.count() == 1, "Success signal not emited.");
}

QTEST_APPLESS_MAIN(Defaultactionlistenertest)

#include "tst_defaultactionlistenertest.moc"
