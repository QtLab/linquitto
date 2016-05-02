#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "connection.h"

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

private:
    Ui::MainWindow *ui;
    bool m_connected;
    Connection m_connection;
};

#endif // MAINWINDOW_H
