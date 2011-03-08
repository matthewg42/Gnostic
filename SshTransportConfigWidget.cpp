#include "SshTransportConfigWidget.hpp"
#include "SshTransport.hpp"
#include "ui_SshTransportConfigWidget.h"

SshTransportConfigWidget::SshTransportConfigWidget(SshTransport* st, QWidget *parent) :
		TransportConfigWidget(parent),
		ui(new Ui::SshTransportConfigWidget)
{
	sshTransport = st;
	ui->setupUi(this);
	ui->idEdit->setText(sshTransport ->getId());
	ui->descriptionEdit->setText(sshTransport->getDescription());
	ui->hostEdit->setText(sshTransport->getHost());
	ui->userEdit->setText(sshTransport->getUser());
	if (sshTransport->getAuthType()==SshTransport::Password)
		ui->passwordRadio->setChecked(true);
	else
		ui->publicRadio->setChecked(true);

	ui->keyFileEdit->setText(sshTransport->getKeyFilePath());

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setHost(QString)));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->userEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setUser(QString)));
	connect(ui->userEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->keyFileEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setKeyFilePath(QString)));
	connect(ui->keyFileEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->passwordRadio, SIGNAL(clicked()), this, SLOT(authTypeChanged()));
	connect(ui->passwordRadio, SIGNAL(clicked()), this, SLOT(emitChanged()));
	connect(ui->publicRadio, SIGNAL(clicked()), this, SLOT(authTypeChanged()));
	connect(ui->publicRadio, SIGNAL(clicked()), this, SLOT(emitChanged()));

	ui->passwordRadio->setChecked(true);
}

void SshTransportConfigWidget::authTypeChanged()
{
	if (ui->passwordRadio->isChecked())
	{
		sshTransport->setAuthType(SshTransport::Password);
	}
	else if (ui->publicRadio->isChecked())
	{
		sshTransport->setAuthType(SshTransport::PublicKey);
	}

	ui->keyFileBrowse->setEnabled(ui->publicRadio->isChecked());
	ui->keyFileEdit->setEnabled(ui->publicRadio->isChecked());
}

SshTransportConfigWidget::~SshTransportConfigWidget()
{
	delete ui;
}
