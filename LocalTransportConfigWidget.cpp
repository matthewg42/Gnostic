#include "LocalTransportConfigWidget.hpp"
#include "LocalTransport.hpp"
#include "ui_LocalTransportConfigWidget.h"

LocalTransportConfigWidget::LocalTransportConfigWidget(LocalTransport* lt, QWidget* parent) :
		QWidget(parent),
		ui(new Ui::LocalTransportConfigWidget)
{
	ui->setupUi(this);
	connect(ui->shellPathEdit, SIGNAL(textEdited(QString)), lt, SLOT(setShellPath(QString)));

	ui->shellPathEdit->setText(lt->shellPath);
}

LocalTransportConfigWidget::~LocalTransportConfigWidget()
{
	delete ui;
}
