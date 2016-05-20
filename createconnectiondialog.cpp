#include "createconnectiondialog.h"
#include "ui_create_connection_dialog.h"

CreateConnectionDialog::CreateConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateConnectionDialog)
{
    ui->setupUi(this);

}

QString CreateConnectionDialog::getBroker() const
{
    return ui->brokerEdit->text();
}

QString CreateConnectionDialog::getName() const
{
    return ui->nameEdit->text();
}

int CreateConnectionDialog::getPort() const
{
    return ui->portSpinner->value();
}

