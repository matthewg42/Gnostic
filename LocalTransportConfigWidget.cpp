#include "LocalTransportConfigWidget.hpp"
#include "ui_LocalTransportConfigWidget.h"

LocalTransportConfigWidget::LocalTransportConfigWidget(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::LocalTransportConfigWidget)
{
	ui->setupUi(this);
}

LocalTransportConfigWidget::~LocalTransportConfigWidget()
{
	delete ui;
}
