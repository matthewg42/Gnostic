#include "OpenSshTransport.hpp"
#include "GnosticApp.hpp"
#include "PasswordDialog.hpp"
#include "FileUtils.hpp"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

OpenSshTransport::OpenSshTransport(QObject *parent) :
		AbstractSshTransport(parent)
{
	proc.setProcessChannelMode(QProcess::SeparateChannels);

	connect(&proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(procError(QProcess::ProcessError)));
	connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(procReadErr()));
	connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(procReadIn()));
	connect(&proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(procStatusUpdate(QProcess::ProcessState)));
	connect(&proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
}

OpenSshTransport::~OpenSshTransport()
{
	//qDebug() << "OpenSshTransport::~OpenSshTransport";
}

bool OpenSshTransport::test()
{
	qDebug() << "OpenSshTransport::test";
	QMessageBox mb;
	mb.setText("OpenSshTransport::test TODO");
	return false;
}

bool OpenSshTransport::start(const QString& exec, const QStringList& args)
{
	qDebug() << "OpenSshTransport::start" << exec << args;

	if (proc.state() != QProcess::NotRunning)
	{
		qWarning() << "OpenSshTransport::start failure - process already running";
		return false;
	}

	dumpDebug();

	QStringList sshArgs;

	if (authType == AbstractSshTransport::PublicKey)
	{
		QMessageBox mb;
		mb.setText("public key authentication not yet implemented, please use password for now");
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
		return false;
	}

	sshArgs << QString("%1@%2").arg(user).arg(host) << exec << args;

	// Don't make a habit of printing the password, even to debug...
	// qDebug() << "PlinkSshTransport::establishConnection start(" << getPlinkExePath() << plinkArgs << ")";
	setConnectionStatus(Transport::EstablishingConnection);
	proc.start(getOpenExePath(), sshArgs);
	return true;

}

void OpenSshTransport::stop()
{
	if (proc.state() != QProcess::NotRunning)
		proc.kill();
}

const QString OpenSshTransport::saveSettings()
{
	qDebug() << "OpenSshTransport::saveSettings";
	AbstractSshTransport::saveSettings();
        GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Transport);
	return id;
}

void OpenSshTransport::dumpDebug()
{
	qDebug() << "OpenSshTransport::dumpDebug, calling AbstractSshTransport::dumpDebug...";
	AbstractSshTransport::dumpDebug();
	qDebug() << "OpenSshTransport::dumpDebug ssh exe path is" << getOpenExePath();
}

void OpenSshTransport::procReadIn()
{
	qDebug() << "OpenSshTransport::procReadIn";
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

void OpenSshTransport::procReadErr()
{
	QByteArray input = proc.readAllStandardError();
	qWarning() << "OpenSshTransport::procReadErr:" << getConnectionStatus() << input;
	handleConnectionInput(QString(input));

}

void OpenSshTransport::handleConnectionInput(const QString& in)
{
	QString input(in);
	if (input.contains("Are you sure you want to continue connecting?"))
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
			stop();
			return;
		}
	}
	else if (input.contains("Permission denied ")) // e.g. wrong password
	{
		QMessageBox mb;
		mb.setText(QString("Transport \"%1\" - permission denied (probably wrong password)").arg(getDescription()));
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
		qWarning() << "OpenSshTransport::test - access denied";
		stop();
		return;
	}
	else
	{
		handleRegularInput(input);
	}
}

void OpenSshTransport::handleRegularInput(const QString& in)
{
	qDebug() << "OpenSshTransport::handleRegularInput:" << getConnectionStatus() << in;
	foreach(QString line, in.split(QRegExp("[\\n\\r]+")))
	{
                if (!line.isEmpty())
                        emit(spewLine(line));
	}
}

QString OpenSshTransport::getOpenExePath()
{
	return GnosticApp::getInstance().settings()->value("paths/ssh", "ssh").toString();
        gnosticSetenv("SSH_ASKPASS", GnosticApp::getInstance().settings()->value("paths/ssh-askpass", "ssh-askpass").toString().toUtf8());
}

