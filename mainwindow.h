#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <map>

#include "asyncconnection.h"
#include "protectableasyncclient.h"
#include "defaultactioncallback.h"

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
    void closeTab(int index);

private:
    void createConnection(QString name, QString broker, int port);
    bool isUniqueTabName(QString name);

private:
    Ui::MainWindow *ui;
    linquitto::ProtectableAsyncClient m_pclient;
    linquitto::DefaultActionCallback m_connectActionCallback;
    linquitto::DefaultActionCallback m_disconnectActionCallback;
};

#endif // MAINWINDOW_H
