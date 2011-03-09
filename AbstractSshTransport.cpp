#include "AbstractSshTransport.hpp"
#include "GnosticApp.hpp"
#include "PasswordDialog.hpp"
#include "SshTransportConfigWidget.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

AbstractSshTransport::AbstractSshTransport(QObject *parent) :
		Transport(parent),
		authType(AbstractSshTransport::Password),
		proc(this)
{
}

AbstractSshTransport::~AbstractSshTransport()
{
	qDebug() << "AbstractSshTransport::~AbstractSshTransport";
	if (proc.state() != QProcess::NotRunning)
		proc.kill();
}

TransportConfigWidget* AbstractSshTransport::getConfigWidget(QWidget* parent)
{
	return new SshTransportConfigWidget(this, parent);
}

const QString& AbstractSshTransport::saveTransport()
{
	qDebug() << "AbstractSshTransport::saveTransport";
	Transport::saveTransport();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/host").arg(id), host);
	settings->setValue(QString("%1/user").arg(id), user);
	settings->setValue(QString("%1/key_file_path").arg(id), "");
	if (authType==AbstractSshTransport::Password)
		settings->setValue(QString("%1/auth_type").arg(id), "password");
	else
		settings->setValue(QString("%1/auth_type").arg(id), "public_key");
	return id;
}

bool AbstractSshTransport::loadSettings(const QString& section)
{
	qDebug() << "AbstractSshTransport::loadSettings";
	if (!Transport::loadSettings(section))
		return false;

	QSettings* settings = GnosticApp::getInstance().settings();
	host = settings->value(QString("%1/host").arg(section)).toString();
	user = settings->value(QString("%1/user").arg(section)).toString();
	keyFilePath = GnosticApp::getInstance().settings()->value(QString("%1/key_file_path").arg(section)).toString();
	if (settings->value(QString("%1/auth_type").arg(section), "password").toString() == "password")
		authType=AbstractSshTransport::Password;
	else
		authType=AbstractSshTransport::PublicKey;

	return true;
}

const QString AbstractSshTransport::getUser()
{
	return user;
}

void AbstractSshTransport::setUser(const QString& u)
{
	qDebug() << "AbstractSshTransport::setUser" << u;
	user = u;
}

const QString AbstractSshTransport::getHost()
{
	return host;
}

void AbstractSshTransport::setHost(const QString& h)
{
	qDebug() << "AbstractSshTransport::sethost" << h;
	host = h;
}

const QString AbstractSshTransport::getKeyFilePath()
{
	return keyFilePath;
}

void AbstractSshTransport::setKeyFilePath(const QString& k)
{
	qDebug() << "AbstractSshTransport::setKeyFilePath" << k;
	keyFilePath= k;
}

AbstractSshTransport::SshAuthType AbstractSshTransport::getAuthType()
{
	return authType;
}

void AbstractSshTransport::setAuthType(SshAuthType t)
{
	qDebug() << "AbstractSshTransport::setAuthType" << t;
	authType = t;
}

void AbstractSshTransport::procStatusUpdate(QProcess::ProcessState newState)
{
	switch (newState)
	{
	case QProcess::NotRunning:
		setConnectionStatus(Transport::Disconnected);
		break;
	case QProcess::Starting:
		setConnectionStatus(Transport::EstablishingConnection);
		break;
	case QProcess::Running:
		setConnectionStatus(Transport::Connected);
		break;
	}
}

void AbstractSshTransport::procReadIn()
{
	qDebug() << "AbstractSshTransport::procReadIn";
	while(1)
	{
		QByteArray in = proc.readLine();
		if (in.isEmpty())
			break;
		else
		{
			QString line(in);
			line.remove(QRegExp("[\\n\\r]+$"));
			emit(receivedLine(line));
		}
	}
}

void AbstractSshTransport::procReadErr()
{
	qWarning() << "AbstractSshTransport::procReadErr:" << proc.readAllStandardError();
}

void AbstractSshTransport::procError(QProcess::ProcessError err)
{
	qWarning() << "AbstractSshTransport::procError detected error:" << err;
}

void AbstractSshTransport::procDone(int err)
{
	qWarning() << "AbstractSshTransport::procError detected error:" << err;
}

void AbstractSshTransport::dumpDebug()
{
	qDebug() << "AbstractSshTransport::dumpDebug, calling Transport::dumpDebug...";
	Transport::dumpDebug();
	qDebug() << "AbstractSshTransport::dumpDebug host=" << host;
	qDebug() << "AbstractSshTransport::dumpDebug user=" << user;
	qDebug() << "AbstractSshTransport::dumpDebug authType=" << authType;
	qDebug() << "AbstractSshTransport::dumpDebug keyFilePath=" << keyFilePath;
}
