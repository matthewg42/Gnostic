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
	proc.setProcessChannelMode(QProcess::SeparateChannels);

	connect(&proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(procError(QProcess::ProcessError)));
	connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(procReadErr()));
	connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(procReadIn()));
	connect(&proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(procStatusUpdate(QProcess::ProcessState)));
	connect(&proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
}

PlinkSshTransport::~PlinkSshTransport()
{
	qDebug() << "PlinkSshTransport::~PlinkSshTransport";
}

bool PlinkSshTransport::test()
{
	qDebug() << "PlinkSshTransport::test";
	QMessageBox mb;
	mb.setText("TODO");
	return false;
}


bool PlinkSshTransport::start(const QString& exec, const QStringList& args)
{
	qDebug() << "PlinkSshTransport::start" << exec << args;

	if (proc.state() != QProcess::NotRunning)
	{
		qWarning() << "PlinkSshTransport::start failure - process already running";
		return false;
	}

	dumpDebug();

	QStringList plinkArgs;

	if (authType == PlinkSshTransport::Password)
	{
		qDebug() << "PlinkSshTransport::establishConnection prompting for password, authType" << authType;
		PasswordDialog d(QString("password for %1@%2").arg(user).arg(host));
		d.exec();
		savedPassword = d.getPassword();
		plinkArgs << "-pw" << savedPassword;
	}
	else
	{
		QMessageBox mb;
		mb.setText("public key authentication not yet implemented, please use password for now");
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
		return false;
	}

	plinkArgs << "-x" << "-a" << "-l" << user << host << exec << args;

	// Don't make a habit of printing the password, even to debug...
	// qDebug() << "PlinkSshTransport::establishConnection start(" << getPlinkExePath() << plinkArgs << ")";
	setConnectionStatus(Transport::EstablishingConnection);
	proc.start(getPlinkExePath(), plinkArgs);
	return true;
}

void PlinkSshTransport::stop()
{
	if (proc.state() != QProcess::NotRunning)
	{
		// send control-c
		char c[2];
		c[0] = 3;
		c[1] = '\0';
		proc.write(c);

		// and kill process just in case
		proc.kill();
	}
}

const QString PlinkSshTransport::saveSettings()
{
	qDebug() << "PlinkSshTransport::saveSettings";
	AbstractSshTransport::saveSettings();
        GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Transport);
	return id;
}

void PlinkSshTransport::dumpDebug()
{
	qDebug() << "PlinkSshTransport::dumpDebug, calling AbstractSshTransport::dumpDebug...";
	AbstractSshTransport::dumpDebug();
	qDebug() << "PlinkSshTransport::dumpDebug plink exe path is" << getPlinkExePath();
}

void PlinkSshTransport::procReadIn()
{
	qDebug() << "PlinkSshTransport::procReadIn";
	QByteArray input = proc.readAll();
	if (getConnectionStatus() == Transport::EstablishingConnection)
	{
		// Handle password prompt, prompt for new hosts and so on...
		handleConnectionInput(QString(input));
	}
	else
	{
		handleRegularInput(QString(input));
	}
}

void PlinkSshTransport::procReadErr()
{
	QByteArray input = proc.readAllStandardError();
	qWarning() << "PlinkSshTransport::procReadErr:" << getConnectionStatus() << input;
	handleConnectionInput(QString(input));

}

void PlinkSshTransport::handleConnectionInput(const QString& in)
{
	QString input(in);
	if (input.contains("Store key in cache?"))
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
			stop();
			return;
		}
	}
	else if (input.contains("Access denied")) // e.g. wrong password
	{
		QMessageBox mb;
		mb.setText(QString("Transport \"%1\" failed to connect - access denied (probably wrong password)").arg(getDescription()));
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
		qWarning() << "PlinkSshTransport::test - access denied";
		stop();
		return;
	}
	else if (input.contains("password:"))
	{
		// we don't handle this - we assume the command line password is OK
		// TODO: pass the password here instead as it is more secure (process table
		// keeps command line options and can be seen with ps)
		qWarning() << "PlinkSshTransport::test - it wants a password";
	}
	else
	{
		handleRegularInput(input);
	}
}

void PlinkSshTransport::handleRegularInput(const QString& in)
{
	qDebug() << "PlinkSshTransport::handleRegularInput:" << getConnectionStatus() << in;
	foreach(QString line, in.split(QRegExp("[\\n\\r]+")))
	{
		emit(spewLine(line));
	}
}

QString PlinkSshTransport::getPlinkExePath()
{
	return GnosticApp::getInstance().settings()->value("paths/plink", "plink").toString();
}
