#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
}

ErrorDialog::ErrorDialog(QWidget *parent, QString mensagem) :
    QDialog(parent),
    ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
    ui->labelMensagem->setText(mensagem);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::on_buttonBox_accepted()
{
    deleteLater();
}
