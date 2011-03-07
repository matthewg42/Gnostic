#include "LocalTransportConfigWidget.hpp"
#include "LocalTransport.hpp"
#include "ui_LocalTransportConfigWidget.h"

LocalTransportConfigWidget::LocalTransportConfigWidget(LocalTransport* lt, QWidget* parent) :
		TransportConfigWidget(parent),
		ui(new Ui::LocalTransportConfigWidget)
{
	ui->setupUi(this);
	ui->idEdit->setText(lt->getId());
	ui->descriptionEdit->setText(lt->getDescription());
	ui->shellPathEdit->setText(lt->shellPath);
	connect(ui->shellPathEdit, SIGNAL(textEdited(QString)), lt, SLOT(setShellPath(QString)));
	connect(ui->shellPathEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), lt, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
}

LocalTransportConfigWidget::~LocalTransportConfigWidget()
{
	delete ui;
}
