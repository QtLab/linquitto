#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include <QDebug>

#include "../../asyncconnection.h"

class FakeToken : public mqtt::itoken
{
public:
    explicit FakeToken();
    explicit FakeToken(std::__cxx11::string const &topic);

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

FakeToken::FakeToken() : m_topics()
{}

FakeToken::FakeToken(std::__cxx11::string const &topic) {
    m_topics.push_back(topic);
}

class TestClient: public mqtt::iasync_client
{
public:
    explicit TestClient() {}
    ~TestClient();

    // iasync_client interface
private:
    void remove_token(mqtt::itoken *);

public:
    mqtt::itoken_ptr connect() throw(mqtt::exception, mqtt::security_exception)
    {m_isConnected = true; return nullptr;}
    mqtt::itoken_ptr connect(mqtt::connect_options) throw(mqtt::exception, mqtt::security_exception)
    {m_isConnected = true; return nullptr;}
    mqtt::itoken_ptr connect(mqtt::connect_options options, void *, mqtt::iaction_listener &cb) throw(mqtt::exception, mqtt::security_exception);
    mqtt::itoken_ptr connect(void *, mqtt::iaction_listener &cb) throw(mqtt::exception, mqtt::security_exception);
    mqtt::itoken_ptr disconnect() throw(mqtt::exception);
    mqtt::itoken_ptr disconnect(long) throw(mqtt::exception) {return disconnect();}
    mqtt::itoken_ptr disconnect(long, void *, mqtt::iaction_listener &) throw(mqtt::exception) {return disconnect();}
    mqtt::itoken_ptr disconnect(void *, mqtt::iaction_listener &cb) throw(mqtt::exception);
    mqtt::idelivery_token_ptr get_pending_delivery_token(int) const {return nullptr;}
    std::vector<mqtt::idelivery_token_ptr> get_pending_delivery_tokens() const {return std::vector<mqtt::idelivery_token_ptr>();}
    std::__cxx11::string get_client_id() const { return 0; }
    std::__cxx11::string get_server_uri() const {return "unknown";}
    bool is_connected() const {return m_isConnected;}
    mqtt::idelivery_token_ptr publish(const std::__cxx11::string &, const void *, size_t, int, bool) throw(mqtt::exception) {return nullptr;}
    mqtt::idelivery_token_ptr publish(const std::__cxx11::string &, const void *, size_t, int, bool, void *, mqtt::iaction_listener &) throw(mqtt::exception) {return nullptr;}
    mqtt::idelivery_token_ptr publish(const std::__cxx11::string &, mqtt::message_ptr) throw(mqtt::exception) {return nullptr;}
    mqtt::idelivery_token_ptr publish(const std::__cxx11::string &topic, mqtt::message_ptr msg, void *userContext, mqtt::iaction_listener &cb) throw(mqtt::exception);
    void set_callback(mqtt::callback &cb) throw(mqtt::exception);
    mqtt::itoken_ptr subscribe(const topic_filter_collection &, const qos_collection &) throw(std::invalid_argument, mqtt::exception) {return nullptr;}
    mqtt::itoken_ptr subscribe(const topic_filter_collection &, const qos_collection &, void *, mqtt::iaction_listener &) throw(std::invalid_argument, mqtt::exception) {return nullptr;}
    mqtt::itoken_ptr subscribe(const std::__cxx11::string &, int) throw(mqtt::exception) {return nullptr;}
    mqtt::itoken_ptr subscribe(const std::__cxx11::string &topicFilter, int qos, void *userContext, mqtt::iaction_listener &callback) throw(mqtt::exception);
    mqtt::itoken_ptr unsubscribe(const std::__cxx11::string &) throw(mqtt::exception) {return nullptr;}
    mqtt::itoken_ptr unsubscribe(const topic_filter_collection &) throw(mqtt::exception) {return nullptr;}
    mqtt::itoken_ptr unsubscribe(const topic_filter_collection &, void *, mqtt::iaction_listener &) throw(mqtt::exception) {return nullptr;}
    mqtt::itoken_ptr unsubscribe(const std::__cxx11::string &topicFilter, void *userContext, mqtt::iaction_listener &callback) throw(mqtt::exception);

private:
    bool m_isConnected = false;
    mqtt::callback *m_callback;
    std::string m_topic;
    bool m_subscribed = false;
};

TestClient::~TestClient()
{}

void TestClient::remove_token(mqtt::itoken *)
{
    // Do nothing
}

mqtt::itoken_ptr TestClient::connect(mqtt::connect_options, void *,
                    mqtt::iaction_listener &cb) throw(mqtt::exception, mqtt::security_exception)
{
    FakeToken token("connect");
    m_isConnected = true;
    cb.on_success(token);
    return nullptr;
}

mqtt::itoken_ptr TestClient::connect(void *, mqtt::iaction_listener &cb) throw(mqtt::exception, mqtt::security_exception)
{
    FakeToken token("connect");
    m_isConnected = true;
    cb.on_success(token);
    return nullptr;
}

mqtt::itoken_ptr TestClient::disconnect() throw(mqtt::exception)
{
    m_isConnected = false;
    return nullptr;
}

mqtt::itoken_ptr TestClient::disconnect(void *, mqtt::iaction_listener &cb) throw(mqtt::exception)
{
    FakeToken token("disconnect");
    cb.on_success(token);
    return nullptr;
}

mqtt::idelivery_token_ptr TestClient::publish(const std::__cxx11::string &topic, mqtt::message_ptr msg, void *, mqtt::iaction_listener &cb) throw(mqtt::exception)
{
    FakeToken token(topic);
    cb.on_success(token);
    if(m_subscribed) {
        if(topic.compare(m_topic) == 0) {
            m_callback->message_arrived(topic, msg);
        }
    }
    return nullptr;
}

void TestClient::set_callback(mqtt::callback &cb) throw(mqtt::exception)
{
    //m_callback = mqtt::callback_ptr(&cb);
    m_callback = &cb;
}

mqtt::itoken_ptr TestClient::subscribe(const std::__cxx11::string &topicFilter, int, void *, mqtt::iaction_listener &callback) throw(mqtt::exception)
{
    m_topic = topicFilter;
    FakeToken token(topicFilter);
    m_subscribed = true;
    callback.on_success(token);
    return nullptr;
}

mqtt::itoken_ptr TestClient::unsubscribe(const std::__cxx11::string &topicFilter, void *, mqtt::iaction_listener &callback) throw(mqtt::exception)
{
    FakeToken token(topicFilter);
    m_subscribed = false;
    callback.on_success(token);
    return nullptr;
}

class Asyncconnectiontests : public QObject
{
    Q_OBJECT

public:
    Asyncconnectiontests();

private Q_SLOTS:
    void connectWithServer();
    void disconnectFromServer();
    void publishMessage();
    void subscribeToTopic();
    void subscribeAndPublish();
    void unsubscribeFromTopic();
};

Asyncconnectiontests::Asyncconnectiontests()
{
}

void Asyncconnectiontests::connectWithServer()
{
    AsyncConnection connection(std::move(
                                   std::unique_ptr<mqtt::iasync_client>(
                                             new TestClient())));
    QSignalSpy spy(&connection, &AsyncConnection::connected);
    connection.connectWithServer();

    QVERIFY2(spy.count() == 1, "Connect failure");
}

void Asyncconnectiontests::disconnectFromServer()
{
    AsyncConnection connection(std::move(
                                   std::unique_ptr<mqtt::iasync_client>(
                                             new TestClient())));
    QSignalSpy connectSpy(&connection, &AsyncConnection::connected);
    QSignalSpy disconnectSpy(&connection, &AsyncConnection::disconnected);
    connection.connectWithServer();
    QVERIFY2(connectSpy.count() == 1, "Connect failed!");
    connection.disconnectFromServer();
    QVERIFY2(disconnectSpy.count() == 1, "Disconnect failed!");
}

void Asyncconnectiontests::publishMessage()
{
    AsyncConnection connection(std::move(
                                   std::unique_ptr<mqtt::iasync_client>(
                                             new TestClient())));
    QSignalSpy connectSpy(&connection, &AsyncConnection::connected);
    QSignalSpy disconnectSpy(&connection, &AsyncConnection::disconnected);
    QSignalSpy publishSpy(&connection, &AsyncConnection::published);
    connection.connectWithServer();
    QVERIFY2(connectSpy.count() == 1, "Connect failed!");
    connection.publishMessage("Test","hello");
    QVERIFY2(publishSpy.count() == 1, "Publish failed!");
    connection.disconnectFromServer();
    QVERIFY2(disconnectSpy.count() == 1, "Disconnect failed!");
}

void Asyncconnectiontests::subscribeToTopic()
{
    AsyncConnection connection(std::move(
                                   std::unique_ptr<mqtt::iasync_client>(
                                             new TestClient())));
    QSignalSpy connectSpy(&connection, &AsyncConnection::connected);
    QSignalSpy disconnectSpy(&connection, &AsyncConnection::disconnected);
    QSignalSpy subscribeSpy(&connection, &AsyncConnection::subscribed);
    connection.connectWithServer();
    QVERIFY2(connectSpy.count() == 1, "Connect failed!");
    connection.subscribeToTopic("Test");
    QVERIFY2(subscribeSpy.count() == 1, "Subscribe failed!");
    connection.disconnectFromServer();
    QVERIFY2(disconnectSpy.count() == 1, "Disconnect failed!");
}

void Asyncconnectiontests::subscribeAndPublish()
{
    AsyncConnection connection(std::move(
                                   std::unique_ptr<mqtt::iasync_client>(
                                             new TestClient())));
    QSignalSpy connectSpy(&connection, &AsyncConnection::connected);
    QSignalSpy disconnectSpy(&connection, &AsyncConnection::disconnected);
    QSignalSpy subscribeSpy(&connection, &AsyncConnection::subscribed);
    QSignalSpy messageArrivedSpy(&connection, &AsyncConnection::messageArrived);

    connection.connectWithServer();
    QVERIFY2(connectSpy.count() == 1, "Connect failed!");

    connection.subscribeToTopic("Test");
    QVERIFY2(subscribeSpy.count() == 1, "Subscribe failed!");

    connection.publishMessage("Test","hello publish");
    QVERIFY2(messageArrivedSpy.count() == 1, "Received message not announced!");
    auto arguments = messageArrivedSpy.takeFirst();
    QString expectedTopic("Test");
    QString expectedMessage("hello publish");
    QVERIFY2(arguments.at(0).type() == QVariant::String, "First parameter is not a string!");
    QVERIFY2(arguments.at(0).toString() == expectedTopic, "Topic is different!");
    QVERIFY2(arguments.at(1).type() == QVariant::String, "Second parameter is not a string!");
    QVERIFY2(arguments.at(1).toString() == expectedMessage, "Message is different!");

    connection.disconnectFromServer();
    QVERIFY2(disconnectSpy.count() == 1, "Disconnect failed!");
}

void Asyncconnectiontests::unsubscribeFromTopic()
{
    AsyncConnection connection(std::move(
                                   std::unique_ptr<mqtt::iasync_client>(
                                             new TestClient())));
    QSignalSpy connectSpy(&connection, &AsyncConnection::connected);
    QSignalSpy disconnectSpy(&connection, &AsyncConnection::disconnected);
    QSignalSpy subscribeSpy(&connection, &AsyncConnection::subscribed);
    QSignalSpy unsubscribeSpy(&connection, &AsyncConnection::unsubscribed);

    connection.connectWithServer();
    QVERIFY2(connectSpy.count() == 1, "Connect failed!");

    connection.subscribeToTopic("Test");
    QVERIFY2(subscribeSpy.count() == 1, "Subscribe failed!");

    connection.unsubscribeFromTopic("Test");
    QVERIFY2(unsubscribeSpy.count() == 1, "Unsubscribe failed!");

    auto arguments = unsubscribeSpy.takeFirst();
    QString expectedTopic("Test");
    QVERIFY2(arguments.at(0).type() == QVariant::String, "First parameter is not a string!");
    QVERIFY2(arguments.at(0).toString() == expectedTopic, "Topic is different!");


    connection.disconnectFromServer();
    QVERIFY2(disconnectSpy.count() == 1, "Disconnect failed!");
}

QTEST_APPLESS_MAIN(Asyncconnectiontests)

#include "tst_asyncconnectiontests.moc"
