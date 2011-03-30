#include "AbstractSshTransport.hpp"
#include "GnosticApp.hpp"
#include "PasswordDialog.hpp"
#include "SshTransportConfigWidget.hpp"
#include "RemoteCommandConfigWidget.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

AbstractSshTransport::AbstractSshTransport(QObject *parent) :
		Transport(parent),
		port(22),
		authType(AbstractSshTransport::Password),
		proc(this)
{
}

AbstractSshTransport::~AbstractSshTransport()
{
	//qDebug() << "AbstractSshTransport::~AbstractSshTransport";
	if (proc.state() != QProcess::NotRunning)
		proc.kill();
}

TransportConfigWidget* AbstractSshTransport::getConfigWidget(QWidget* parent)
{
	if (!configWidget)
		configWidget = new SshTransportConfigWidget(this, parent);

	return configWidget;
}

RemoteCommandConfigWidget* AbstractSshTransport::getCommandWidget(QWidget* parent)
{
	if (!commandWidget)
		commandWidget = new RemoteCommandConfigWidget(this, parent);

	return commandWidget;
}

const QString AbstractSshTransport::saveSettings()
{
	//qDebug() << "AbstractSshTransport::saveSettings";
	Transport::saveSettings();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/host").arg(id), host);
	settings->setValue(QString("%1/user").arg(id), user);
	settings->setValue(QString("%1/port").arg(id), port);
	settings->setValue(QString("%1/key_file_path").arg(id), "");
	if (authType==AbstractSshTransport::Password)
		settings->setValue(QString("%1/auth_type").arg(id), "password");
	else
		settings->setValue(QString("%1/auth_type").arg(id), "public_key");

        return id;
}

bool AbstractSshTransport::loadSettings(const QString& section)
{
	//qDebug() << "AbstractSshTransport::loadSettings" << section;
	if (!Transport::loadSettings(section))
		return false;

	QSettings* settings = GnosticApp::getInstance().settings();
	host = settings->value(QString("%1/host").arg(section)).toString();
	user = settings->value(QString("%1/user").arg(section), 22).toString();
	port = settings->value(QString("%1/port").arg(section), false).toInt();
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
	user = u;
	autogenDescription();
}

const QString AbstractSshTransport::getHost()
{
	return host;	
}

void AbstractSshTransport::setHost(const QString& h)
{
	host = h;
	autogenDescription();
}

const QString AbstractSshTransport::getKeyFilePath()
{
	return keyFilePath;
}

void AbstractSshTransport::setKeyFilePath(const QString& k)
{
	keyFilePath = k;
}

int AbstractSshTransport::getPort()
{
	return port;
}

void AbstractSshTransport::setPort(int p)
{
	port = p;
}

AbstractSshTransport::SshAuthType AbstractSshTransport::getAuthType()
{
	return authType;
}

void AbstractSshTransport::setAuthType(SshAuthType t)
{
	//qDebug() << "AbstractSshTransport::setAuthType" << t;
	authType = t;
}

void AbstractSshTransport::autogenDescription()
{
	QString t = getType();
	t.remove("SshTransport");

	if (port!=22)
		setDescription(QString("%1@%2:%3 (%4)").arg(user).arg(host).arg(port).arg(t));
	else
		setDescription(QString("%1@%2 (%3)").arg(user).arg(host).arg(t));
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
			emit(spewLine(line));
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
	//qDebug() << "AbstractSshTransport::dumpDebug, calling Transport::dumpDebug...";
	Transport::dumpDebug();
	//qDebug() << "AbstractSshTransport::dumpDebug host=" << host;
	//qDebug() << "AbstractSshTransport::dumpDebug user=" << user;
	//qDebug() << "AbstractSshTransport::dumpDebug authType=" << authType;
	//qDebug() << "AbstractSshTransport::dumpDebug keyFilePath=" << keyFilePath;
}
