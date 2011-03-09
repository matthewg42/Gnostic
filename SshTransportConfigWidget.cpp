#include "SshTransportConfigWidget.hpp"
#include "AbstractSshTransport.hpp"
#include "ui_SshTransportConfigWidget.h"

SshTransportConfigWidget::SshTransportConfigWidget(Transport* t, QWidget *parent) :
		TransportConfigWidget(t, parent),
		ui(new Ui::SshTransportConfigWidget)
{
	ui->setupUi(this);
	AbstractSshTransport* sshTransport = static_cast<AbstractSshTransport*>(transport);
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setDescription(QString)));
	connect(ui->userEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setUser(QString)));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setHost(QString)));
}

SshTransportConfigWidget::~SshTransportConfigWidget()
{
	delete ui;
}
