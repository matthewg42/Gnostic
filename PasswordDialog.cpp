#include "PasswordDialog.hpp"
#include "ui_PasswordDialog.h"

PasswordDialog::PasswordDialog(const QString& message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    this->setWindowModality(Qt::WindowModal);
    ui->setupUi(this);
    ui->pwLabel->setText(message);
    connect(ui->pwEdit, SIGNAL(returnPressed()), this, SLOT(setPassword()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(setPassword()));
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::setPassword()
{
    pass = ui->pwEdit->text();
    this->done(0);
}
