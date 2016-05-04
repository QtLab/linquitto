#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

//#include "connection.h"
#include "asyncconnection.h"
#include "defaultactionlistener.h"
#include "debugcallback.h"

namespace Ui {
class MainWindow;
}

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
    void connectionLost(QString cause);
    void messageArrived(QString topic, QString message);
    void onSubscribe();
    void onPublish();

private:
    Ui::MainWindow *ui;
    AsyncConnection m_connection;
    //DefaultActionListener m_defaultListener;
    DebugCallback m_debugCallback;
};

#endif // MAINWINDOW_H
