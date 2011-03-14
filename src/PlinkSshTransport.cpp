#include "PlinkSshTransport.hpp"
#include "GnosticApp.hpp"
#include "PasswordDialog.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

PlinkSshTransport::PlinkSshTransport(QObject *parent) :
		AbstractSshTransport(parent)
{
}

PlinkSshTransport::~PlinkSshTransport()
{
	qDebug() << "PlinkSshTransport::~PlinkSshTransport";
}

bool PlinkSshTransport::test()
{
	qDebug() << "PlinkSshTransport::test";
	QProcess testProc(this);
	QString ret = establishConnection(testProc, "echo", QStringList() << "hello world");
	qDebug() << "PlinkSshTransport::test ret" << ret;
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

	qDebug() << "PlinkSshTransport::start process state after establishing is:" << proc.state();

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

void PlinkSshTransport::stop()
{
	if (proc.state() != QProcess::NotRunning)
		proc.kill();
}

const QString PlinkSshTransport::saveSettings()
{
	qDebug() << "PlinkSshTransport::saveSettings";
	AbstractSshTransport::saveSettings();
	return id;
}

void PlinkSshTransport::dumpDebug()
{
	qDebug() << "PlinkSshTransport::dumpDebug, calling AbstractSshTransport::dumpDebug...";
	AbstractSshTransport::dumpDebug();
	qDebug() << "PlinkSshTransport::dumpDebug plink exe path is" << getPlinkExePath();
}

QString PlinkSshTransport::establishConnection(QProcess& proc, const QString& exe, const QStringList& args)
{
	qDebug() << "PlinkSshTransport::establishConnection" << exe << args;
	dumpDebug();
	proc.setProcessChannelMode(QProcess::MergedChannels);
	QStringList plinkArgs;

        int startTimeout = GnosticApp::getInstance().settings()->value("plink/start_timeout", 5000).toInt();
        int connectTimeout = GnosticApp::getInstance().settings()->value("plink/connect_timeout", 10000).toInt();

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

	// Don't make a habit of printing the password, even to debug...
	// qDebug() << "PlinkSshTransport::establishConnection start(" << getPlinkExePath() << plinkArgs << ")";
	proc.start(getPlinkExePath(), plinkArgs);
        if (!proc.waitForStarted(startTimeout))
	{
		qDebug() << "PlinkSshTransport::establishConnection waitForStarted returned false";
		proc.kill();  // just in case it's taking too long but will still start at some point...
		return false;
	}

	bool done = false;
	while(!done)
	{
                if (!proc.waitForReadyRead(connectTimeout))
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
			qWarning() << "PlinkSshTransport::test - access denied";
			proc.kill();
			return QString();
		}
		else if (QString(output).contains("password:"))
		{
			// we don't handle this - we assume the command line password is OK
			// TODO: pass the password here instead as it is more secure (process table
			// keeps command line options and can be seen with ps)
			qWarning() << "PlinkSshTransport::test - it wants a password";
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

QString PlinkSshTransport::getPlinkExePath()
{
	return GnosticApp::getInstance().settings()->value("paths/plink", "plink").toString();
}
