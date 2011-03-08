#include "SshTransport.hpp"
#include "SshTransportConfigWidget.hpp"
#include "GnosticApp.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>

SshTransport::SshTransport(QObject *parent) :
		Transport(parent)
{
	proc = new QProcess(this);
}

SshTransport::~SshTransport()
{
	stopMonitor();
}

SshTransport::SshTransport(SshTransport& other, QObject* parent) :
		Transport(parent),
		host(other.host),
		user(other.user),
		authType(other.authType),
		keyFilePath(other.keyFilePath)
{
	proc = new QProcess(this);
}

TransportConfigWidget* SshTransport::getConfigWidget(QWidget* parent)
{
	return new SshTransportConfigWidget(this, parent);
}

bool SshTransport::testTransport()
{
	QProcess testProc(this);
	testProc.start("plink",
		       QStringList() << "-pw" << "pewpy"
		       << "-x" << "-a" << "-T"
		       << "-l" << user
		       << host
		       << "echo hello world");
	if (!testProc.waitForStarted())
	{
		qDebug() << "SshTransport::testTransport waitForStarted returned false";
		return false;
	}

	if (!testProc.waitForFinished())
	{
		qDebug() << "SshTransport::testTransport  waitForFinished returned false";
		return false;
	}

	QByteArray output = testProc.readAll();
	if (QString(output) != "hello world\n")
	{
		qWarning() << "SshTransport::testTransport - output was not what was expected:" << output;
		return false;
	}

	return true;
}

bool SshTransport::startMonitor(const QString& exec, const QStringList& args)
{
	// TODO after password prompt
//	this->setConnectionStatus(Transport::EstablishingConnection);

//	proc->start(exec, argsToCmd(QStringList() << args));
//	return proc->waitForStarted(10000);
}

void SshTransport::stopMonitor()
{
	if (proc)
		if (proc->state() != QProcess::NotRunning)
			proc->kill();
}

void SshTransport::saveTransport()
{
	qDebug() << "SshTransport::saveTransport" << id << description << host << user << authType << keyFilePath;
	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/type").arg(id), "SshTransport");
	settings->setValue(QString("%1/description").arg(id), description);
	settings->setValue(QString("%1/host").arg(id), host);
	settings->setValue(QString("%1/user").arg(id), user);
	if (authType==SshTransport::Password)
		settings->setValue(QString("%1/auth_type").arg(id), "password");
	else
		settings->setValue(QString("%1/auth_type").arg(id), "public_key");
}

const QString SshTransport::getUser()
{
	return user;
}

void SshTransport::setUser(const QString& u)
{
	qDebug() << "SshTransport::setUser" << u;
	user = u;
}

const QString SshTransport::getHost()
{
	return host;
}

void SshTransport::setHost(const QString& h)
{
	qDebug() << "SshTransport::sethost" << h;
	host = h;
}

const QString SshTransport::getKeyFilePath()
{
	return keyFilePath;
}

void SshTransport::setKeyFilePath(const QString& k)
{
	qDebug() << "SshTransport::setKeyFilePath" << k;
	keyFilePath= k;
}

SshTransport::SshAuthType SshTransport::getAuthType()
{
	return authType;
}

void SshTransport::setAuthType(SshAuthType t)
{
	qDebug() << "SshTransport::setAuthType" << t;
	authType = t;
}

void SshTransport::procStatusUpdate(QProcess::ProcessState newState)
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

void SshTransport::procReadIn()
{
	qDebug() << "SshTransport::procReadIn";
}

void SshTransport::procReadErr()
{
	qWarning() << "SshTransport::procReadErr:" << proc->readAllStandardError();
}

void SshTransport::procError(QProcess::ProcessError err)
{
	qWarning() << "SshTransport::procError detected error:" << err;
}

void SshTransport::procDone(int err)
{
	qWarning() << "SshTransport::procError detected error:" << err;
}

void SshTransport::makeProcess()
{
	proc = new QProcess(this);
	connect(proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(procError(QProcess::ProcessError)));
	connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(procReadErr()));
	connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(procReadIn()));
	connect(proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(procStatusUpdate(QProcess::ProcessState)));
	connect(proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
}

void SshTransport::dumpDebug()
{
	qDebug() << "SshTransport::dumpDebug id=" << id;
	qDebug() << "SshTransport::dumpDebug description=" << description;
	qDebug() << "SshTransport::dumpDebug host=" << host;
	qDebug() << "SshTransport::dumpDebug user=" << user;
	qDebug() << "SshTransport::dumpDebug authType=" << authType;
	qDebug() << "SshTransport::dumpDebug keyFilePath=" << keyFilePath;

}

