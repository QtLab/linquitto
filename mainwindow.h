#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "mqtt/client.h"

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

private:
    Ui::MainWindow *ui;
    bool m_connected;
    std::unique_ptr<mqtt::client> m_client;
};

#endif // MAINWINDOW_H
