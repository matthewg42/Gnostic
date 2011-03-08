#include "SshTransport.hpp"
#include "SshTransportConfigWidget.hpp"
#include "GnosticApp.hpp"
#include "PasswordDialog.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

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
	qDebug() << "SshTransport::testTransport";
	QProcess testProc(this);
	testProc.setProcessChannelMode(QProcess::MergedChannels);
	QString exe = "plink"; // TODO: get this from global setting in GnosticApp
	QStringList args;

	if (authType == SshTransport::Password)
	{
		qDebug() << "SshTransport::testTransport prompting for password, authType" << authType;
		QString pass;
		PasswordDialog d(QString("password for %1@%2").arg(user).arg(host));
		d.exec();
		pass = d.getPassword();
		args << "-pw" << pass;
	}
	else
	{
		QMessageBox mb;
		mb.setText("public key authentication not yet implemented, please use password for now");
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
		return false;
	}

	args << "-x" << "-a" << "-T" << "-l" << user << host << "echo hello world";

	testProc.start(exe, args);
	if (!testProc.waitForStarted())
	{
		qDebug() << "SshTransport::testTransport waitForStarted returned false";
		return false;
	}

	bool done = false;
	bool ok = false;

	while(!done)
	{
		if (!testProc.waitForReadyRead(10000))
		{
			qDebug() << "SshTransport::testTransport waitForReadyRead returned false";
			testProc.kill();
			return false;
		}

		QByteArray output = testProc.readAll();
		qDebug() << "SshTransport::testTransport read:" << output;
		if (QString(output).contains("hello world\n"))
		{
			qWarning() << "SshTransport::testTransport - got the hello world that we wanted";
			done = true;
			ok = true;
		}
		else if (QString(output).contains("Store key in cache?"))
		{
			QMessageBox mb;
			mb.setText("This host has not been seen before.  Do you want to add it to the list of known hosts?");
			mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			mb.setDefaultButton(QMessageBox::Yes);
			if (mb.exec() == QMessageBox::Yes)
			{
				testProc.write("y\n");
			}
			else
			{
				testProc.kill();
				done = true;
			}
		}
		else if (QString(output).contains("Access denied")) // e.g. wrong password
		{
			qWarning() << "SshTransport::testTransport - access denied";
			done = true;
			ok = false;
		}
		else if (QString(output).contains("password:"))
		{
			qWarning() << "SshTransport::testTransport - it wants a password";
			done = true;
			ok = false;
		}
		else
		{
			qWarning() << "SshTransport::testTransport - some unexpected stuff:" << output;
			done = true;
			ok = false;
		}
	}

	if (testProc.state()!=QProcess::NotRunning)
		testProc.kill();

	return ok;
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
	settings->setValue(QString("%1/key_file_path").arg(id), "");
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

