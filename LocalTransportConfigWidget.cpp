#include "LocalTransportConfigWidget.hpp"
#include "ui_LocalTransportConfigWidget.h"
#include "LocalTransport.hpp"

LocalTransportConfigWidget::LocalTransportConfigWidget(Transport* t, QWidget *parent) :
    TransportConfigWidget(t, parent),
    ui(new Ui::LocalTransportConfigWidget)
{
    ui->setupUi(this);
    connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), transport, SLOT(setDescription(QString)));
}

LocalTransportConfigWidget::~LocalTransportConfigWidget()
{
    delete ui;
}
