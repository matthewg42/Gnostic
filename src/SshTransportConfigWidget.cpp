#include "SshTransportConfigWidget.hpp"
#include "AbstractSshTransport.hpp"
#include "ui_SshTransportConfigWidget.h"

#include <QString>

SshTransportConfigWidget::SshTransportConfigWidget(Transport* t, QWidget *parent) :
		TransportConfigWidget(t, parent),
		ui(new Ui::SshTransportConfigWidget)
{
	ui->setupUi(this);
	AbstractSshTransport* sshTransport = static_cast<AbstractSshTransport*>(transport);
	ui->hostEdit->setText(sshTransport->getHost());
	ui->userEdit->setText(sshTransport->getUser());
	connect(ui->userEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setUser(QString)));
	connect(ui->userEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setHost(QString)));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->portSpin, SIGNAL(valueChanged(int)), sshTransport, SLOT(setPort(int)));
	connect(ui->portSpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));

	QString subType = transport->getType();
	subType.remove("Transport");

	ui->textBrowser->append(QString("This transport uses %1.").arg(subType));
}

SshTransportConfigWidget::~SshTransportConfigWidget()
{
	delete ui;
}
