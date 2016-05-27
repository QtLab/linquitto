#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <map>

#include "asyncconnection.h"
#include "protectableasyncclient.h"
#include "defaultactioncallback.h"
#include "defaulteventcallback.h"

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class builds and connects the UI
 * User interaction is dispatched from here to the appropriate methods
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    static void staticOnConnectSuccess(void *context, MQTTAsync_successData* data);

public slots:
    void addLog(QString message);
    void onCreateConnection();
    void onTestConnection();
    void onTestPublish();
    void onTestSubscribe();
    void onTestUnsubscribe();
    void closeTab(int index);

    void onSuccess();
    void onFailure(int errorCode, const QString &errorMessage);
    void onMessageArrived(const QString &topic, const QString &message);
    void onDeliveryComplete(int id);
    void onConnectionLost(const QString &cause);

private:
    void createConnection(QString name, QString broker, int port);
    bool isUniqueTabName(QString name);

private:
    Ui::MainWindow *ui;
    linquitto::ProtectableAsyncClient m_pclient;
    linquitto::DefaultActionCallback m_connectActionCallback;
    linquitto::DefaultActionCallback m_disconnectActionCallback;
    linquitto::DefaultActionCallback m_publishActionCallback;
    linquitto::DefaultActionCallback m_subscribeActionCallback;
    linquitto::DefaultActionCallback m_unsubscribeActionCallback;
    linquitto::DefaultEventCallback m_eventCallback;
};

#endif // MAINWINDOW_H
