#include "createconnectiondialog.h"
#include "ui_create_connection_dialog.h"

CreateConnectionDialog::CreateConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateConnectionDialog)
{
    ui->setupUi(this);

    connect(ui->sslCheckBox, &QCheckBox::stateChanged,
            this, &CreateConnectionDialog::onStateChanged);
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

bool CreateConnectionDialog::sslEnabled() const
{
    return ui->sslCheckBox->isChecked();
}

void CreateConnectionDialog::onStateChanged(int state)
{
    if(state == Qt::Checked) {
        ui->portSpinner->setValue(8883);
    } else {
        ui->portSpinner->setValue(1883);
    }
}

