#ifndef CREATECONNECTIONDIALOG_H
#define CREATECONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class CreateConnectionDialog;
}

class CreateConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateConnectionDialog(QWidget *parent = 0);
    QString getBroker() const;
    QString getName() const;
    int getPort() const;
    bool sslEnabled() const;

signals:

public slots:

private slots:
    void onStateChanged(int state);

private:
    Ui::CreateConnectionDialog *ui;
};

#endif // CREATECONNECTIONDIALOG_H
