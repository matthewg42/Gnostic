#include "SshTransportConfigWidget.hpp"
#include "SshTransport.hpp"
#include "GnosticApp.hpp"
#include "ui_SshTransportConfigWidget.h"

#include <QFileDialog>

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
	ui->keyFileEdit->setText(sshTransport->getKeyFilePath());
	if (sshTransport->getAuthType()==SshTransport::Password)
		ui->passwordRadio->setChecked(true);
	else
		ui->publicRadio->setChecked(true);

	connect(ui->passwordRadio, SIGNAL(clicked()), this, SLOT(authTypeChanged()));
	connect(ui->passwordRadio, SIGNAL(clicked()), this, SLOT(emitChanged()));
	connect(ui->publicRadio, SIGNAL(clicked()), this, SLOT(authTypeChanged()));
	connect(ui->publicRadio, SIGNAL(clicked()), this, SLOT(emitChanged()));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setHost(QString)));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->userEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setUser(QString)));
	connect(ui->userEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->keyFileEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setKeyFilePath(QString)));
	connect(ui->keyFileEdit, SIGNAL(textEdited(QString)), this, SLOT(emitChanged()));
	connect(ui->keyFileBrowse, SIGNAL(clicked()), this, SLOT(chooseKeyPath()));

	authTypeChanged();
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

void SshTransportConfigWidget::chooseKeyPath()
{
	QFileDialog fd;
	QString path = fd.getOpenFileName(
			this,
			"Select a Putty private key file to use with this transport",
			GnosticApp::getInstance().getConfigDir(),
			"Putty private key files (*.ppk)");

	if (path != sshTransport->getKeyFilePath())
	{
		sshTransport->setKeyFilePath(path);
		ui->keyFileEdit->setText(sshTransport->getKeyFilePath());
		emitChanged();
	}
}
