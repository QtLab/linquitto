#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <map>

#include "asyncconnection.h"

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
    void addLog(QString message);
    void onCreateConnection();
    void closeTab(int index);

private:
    void createConnection(QString name, QString broker, int port);
    bool isUniqueTabName(QString name);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
