#include "PlinkSshTransport.hpp"
#include "GnosticApp.hpp"
#include "PasswordDialog.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

PlinkSshTransport::PlinkSshTransport(QObject *parent) :
		Transport(parent),
		proc(this)
{
}

PlinkSshTransport::~PlinkSshTransport()
{
	proc.kill();
}

bool PlinkSshTransport::testTransport()
{
	qDebug() << "PlinkSshTransport::testTransport";
	QProcess testProc(this);
	QString ret = establishConnection(testProc, "echo", QStringList() << "hello world");
	qDebug() << "PlinkSshTransport::testTransport ret" << ret;
	if (!testProc.waitForFinished(5000))
		return false;
	return (testProc.exitStatus() == 0);

}

bool PlinkSshTransport::start(const QString& exec, const QStringList& args)
{
	proc.disconnect(this);
	QString ret = establishConnection(proc, exec, args);
	if (ret.isEmpty())
		return false;
	connect(&proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(procError(QProcess::ProcessError)));
	connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(procReadErr()));
	connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(procReadIn()));
	connect(&proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(procStatusUpdate(QProcess::ProcessState)));
	connect(&proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
	foreach (QString line, ret.split(QRegExp("[\\n\\r]+")))
	{
		emit(receivedLine(line));
	}
	return true;
}

void PlinkSshTransport::stop()
{
	if (proc.state() != QProcess::NotRunning)
		proc.kill();
}

const QString& PlinkSshTransport::saveTransport()
{
	qDebug() << "PlinkSshTransport::saveTransport";
	Transport::saveTransport();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/host").arg(id), host);
	settings->setValue(QString("%1/user").arg(id), user);
	if (authType==PlinkSshTransport::Password)
		settings->setValue(QString("%1/auth_type").arg(id), "password");
	else
		settings->setValue(QString("%1/auth_type").arg(id), "public_key");
	settings->setValue(QString("%1/key_file_path").arg(id), "");
	return id;
}

const QString PlinkSshTransport::getUser()
{
	return user;
}

void PlinkSshTransport::setUser(const QString& u)
{
	qDebug() << "PlinkSshTransport::setUser" << u;
	user = u;
}

const QString PlinkSshTransport::getHost()
{
	return host;
}

void PlinkSshTransport::setHost(const QString& h)
{
	qDebug() << "PlinkSshTransport::sethost" << h;
	host = h;
}

const QString PlinkSshTransport::getKeyFilePath()
{
	return keyFilePath;
}

void PlinkSshTransport::setKeyFilePath(const QString& k)
{
	qDebug() << "PlinkSshTransport::setKeyFilePath" << k;
	keyFilePath= k;
}

PlinkSshTransport::SshAuthType PlinkSshTransport::getAuthType()
{
	return authType;
}

void PlinkSshTransport::setAuthType(SshAuthType t)
{
	qDebug() << "PlinkSshTransport::setAuthType" << t;
	authType = t;
}

void PlinkSshTransport::procStatusUpdate(QProcess::ProcessState newState)
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

void PlinkSshTransport::procReadIn()
{
	qDebug() << "PlinkSshTransport::procReadIn";
}

void PlinkSshTransport::procReadErr()
{
	qWarning() << "PlinkSshTransport::procReadErr:" << proc.readAllStandardError();
}

void PlinkSshTransport::procError(QProcess::ProcessError err)
{
	qWarning() << "PlinkSshTransport::procError detected error:" << err;
}

void PlinkSshTransport::procDone(int err)
{
	qWarning() << "PlinkSshTransport::procError detected error:" << err;
}

void PlinkSshTransport::dumpDebug()
{
	qDebug() << "PlinkSshTransport::dumpDebug id=" << id;
	qDebug() << "PlinkSshTransport::dumpDebug description=" << description;
	qDebug() << "PlinkSshTransport::dumpDebug host=" << host;
	qDebug() << "PlinkSshTransport::dumpDebug user=" << user;
	qDebug() << "PlinkSshTransport::dumpDebug authType=" << authType;
	qDebug() << "PlinkSshTransport::dumpDebug keyFilePath=" << keyFilePath;

}

QString PlinkSshTransport::establishConnection(QProcess& proc, const QString& exe, const QStringList& args)
{
	qDebug() << "PlinkSshTransport::establishConnection" << exe << args;
	dumpDebug();
	proc.setProcessChannelMode(QProcess::MergedChannels);
	QString plinkExe = GnosticApp::getInstance().settings()->value("Programs/plink_path", "plink").toString();
	QStringList plinkArgs;

	if (authType == PlinkSshTransport::Password)
	{
		qDebug() << "PlinkSshTransport::establishConnection prompting for password, authType" << authType;
		QString pass;
		PasswordDialog d(QString("password for %1@%2").arg(user).arg(host));
		d.exec();
		pass = d.getPassword();
		plinkArgs << "-pw" << pass;
	}
	else
	{
		QMessageBox mb;
		mb.setText("public key authentication not yet implemented, please use password for now");
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
		return QString();
	}

	plinkArgs << "-x" << "-a" << "-T" << "-l" << user << host << exe << args;

	qDebug() << "PlinkSshTransport::establishConnection start(" << plinkExe << plinkArgs << ")";
	proc.start(plinkExe, plinkArgs);
	if (!proc.waitForStarted())
	{
		qDebug() << "PlinkSshTransport::establishConnection waitForStarted returned false";
		proc.kill();  // just in case it's taking too long but will still start at some point...
		return false;
	}

	bool done = false;
	while(!done)
	{
		if (!proc.waitForReadyRead(8000))
		{
			qDebug() << "PlinkSshTransport::establishConnection waitForReadyRead returned false";
			proc.kill();
			return QString();
		}

		QByteArray output = proc.readAll();
		qDebug() << "PlinkSshTransport::establishConnection read:" << output;
		if (QString(output).contains("Store key in cache?"))
		{
			QMessageBox mb;
			mb.setText("This host has not been seen before.  Do you want to add it to the list of known hosts?");
			mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			mb.setDefaultButton(QMessageBox::Yes);
			if (mb.exec() == QMessageBox::Yes)
			{
				proc.write("y\n");
			}
			else
			{
				proc.kill();
				return QString();
			}
		}
		else if (QString(output).contains("Access denied")) // e.g. wrong password
		{
			qWarning() << "PlinkSshTransport::testTransport - access denied";
			proc.kill();
			return QString();
		}
		else if (QString(output).contains("password:"))
		{
			// we don't handle this - we assume the command line password is OK
			// TODO: pass the password here instead as it is more secure (process table
			// keeps command line options and can be seen with ps)
			qWarning() << "PlinkSshTransport::testTransport - it wants a password";
			proc.kill();
			return QString();
		}
		else
		{
			return QString(output);
		}
	}
	return QString();
}
