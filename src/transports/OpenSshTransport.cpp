#include "OpenSshTransport.hpp"
#include "GnosticApp.hpp"
#include "PasswordDialog.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

OpenSshTransport::OpenSshTransport(QObject *parent) :
		AbstractSshTransport(parent)
{
}

OpenSshTransport::~OpenSshTransport()
{
	//qDebug() << "OpenSshTransport::~OpenSshTransport";
}

bool OpenSshTransport::test()
{
	//qDebug() << "OpenSshTransport::test";
	QProcess testProc(this);
	QString ret = establishConnection(testProc, "echo", QStringList() << "hello world");
	//qDebug() << "OpenSshTransport::test ret" << ret;
	if (!testProc.waitForFinished(5000))
		return false;
	return (testProc.exitStatus() == 0);
}

bool OpenSshTransport::start(const QString& exec, const QStringList& args)
{
	proc.disconnect(this);
	QString ret = establishConnection(proc, exec, args);
	if (ret.isEmpty())
		return false;

	//qDebug() << "OpenSshTransport::start process state after establishing is:" << proc.state();

	connect(&proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(procError(QProcess::ProcessError)));
	connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(procReadErr()));
	connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(procReadIn()));
	connect(&proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(procStatusUpdate(QProcess::ProcessState)));
	connect(&proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
	foreach (QString line, ret.split(QRegExp("[\\n\\r]+")))
	{
		line.remove(QRegExp("[\\r\\n]+"));
		emit(spewLine(line));
	}
	return true;
}

void OpenSshTransport::stop()
{
	if (proc.state() != QProcess::NotRunning)
		proc.kill();
}

const QString OpenSshTransport::saveSettings()
{
	//qDebug() << "OpenSshTransport::saveSettings";
	AbstractSshTransport::saveSettings();
	return id;
}

void OpenSshTransport::dumpDebug()
{
	//qDebug() << "OpenSshTransport::dumpDebug, calling AbstractSshTransport::dumpDebug...";
	AbstractSshTransport::dumpDebug();
	//qDebug() << "OpenSshTransport::dumpDebug ssh exe path is" << getOpenExePath();
}

QString OpenSshTransport::establishConnection(QProcess& proc, const QString& exe, const QStringList& args)
{
	//qDebug() << "OpenSshTransport::establishConnection" << exe << args;
	dumpDebug();
	proc.setProcessChannelMode(QProcess::MergedChannels);
	QStringList sshArgs;
	QString pass;

	// We don't need to prompt for the password - ssh-askpass will do that if
	// it is installed....  If not, hopefully the one we implemented will
	// be picked up.
	if (authType == OpenSshTransport::PublicKey)
	{
		QMessageBox mb;
		mb.setText("public key authentication not yet implemented, please use password for now");
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
		return QString();
	}

	sshArgs << QString("%1@%2").arg(user).arg(host) << exe << args;

	//qDebug() << "OpenSshTransport::establishConnection start(" << getOpenExePath() << sshArgs << ")";
	proc.start(getOpenExePath(), sshArgs);
	if (!proc.waitForStarted())
	{
		//qDebug() << "OpenSshTransport::establishConnection waitForStarted returned false";
		proc.kill();  // just in case it's taking too long but will still start at some point...
		return false;
	}

	bool done = false;
	while(!done)
	{
		if (!proc.waitForReadyRead(30000))
		{
			//qDebug() << "OpenSshTransport::establishConnection waitForReadyRead returned false";
			proc.kill();
			return QString();
		}

		QByteArray output = proc.readAll();
		//qDebug() << "OpenSshTransport::establishConnection read:" << output;
		if (QString(output).contains("Are you sure you want to continue connecting?"))
		{
			QMessageBox mb;
			mb.setText("This host has not been seen before.  Do you want to add it to the list of known hosts?");
			mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			mb.setDefaultButton(QMessageBox::Yes);
			if (mb.exec() == QMessageBox::Yes)
			{
				proc.write("yes\n");
			}
			else
			{
				proc.kill();
				return QString();
			}
		}
		else if (QString(output).contains("Permission denied")) // e.g. wrong password
		{
			qWarning() << "OpenSshTransport::test - access denied";
			proc.kill();
			return QString();
		}
		else
		{
			QString ret(output);
			ret.remove(QRegExp("[\\n\\r]+$"));
			return ret;
		}
	}
	return QString();
}

QString OpenSshTransport::getOpenExePath()
{
	return GnosticApp::getInstance().settings()->value("programs/ssh_path", "ssh").toString();
}

