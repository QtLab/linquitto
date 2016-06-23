#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
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
    void closeTab(int index);

private:
    void createConnection(QString name, QString broker, int port, bool sslEnabled);
    bool isUniqueTabName(QString name);

private:
    Ui::MainWindow *ui;
    QTimer timer;
};

#endif // MAINWINDOW_H
