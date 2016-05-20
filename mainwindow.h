#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <map>

//#include "connection.h"
#include "asyncconnection.h"
#include "defaultactionlistener.h"
#include "debugcallback.h"

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

public slots:
    void switchConnection();
    void addLog(QString message);
    void connectionEstablished();
    void disconnected();
    void connectionHasPublished();
    void connectionHasSubscribed();
    void connectionHasUnsubscribed(QString topic);
    void connectionLost(QString cause);
    void messageArrived(QString topic, QString message);
    void onSubscribe();
    void onUnsubscribe();
    void onPublish();

private:
    Ui::MainWindow *ui;
    std::map<std::string, std::unique_ptr<AsyncConnection> > m_connections;
    DebugCallback m_debugCallback;
};

#endif // MAINWINDOW_H
