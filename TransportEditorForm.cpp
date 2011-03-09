#include "TransportEditorForm.hpp"
#include "ui_TransportEditorForm.h"

TransportEditorForm::TransportEditorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransportEditorForm)
{
    ui->setupUi(this);
}

TransportEditorForm::~TransportEditorForm()
{
    delete ui;
}
