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
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setDescription(QString)));
	connect(ui->userEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setUser(QString)));
	connect(ui->hostEdit, SIGNAL(textEdited(QString)), sshTransport, SLOT(setHost(QString)));


	QString subType = transport->getType();
	subType.remove("Transport");

	ui->textBrowser->append(QString("This transport uses %1.").arg(subType));
}

SshTransportConfigWidget::~SshTransportConfigWidget()
{
	delete ui;
}
