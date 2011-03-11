#include "LocalTransport.hpp"
#include "GnosticApp.hpp"
#include "LocalTransportConfigWidget.hpp"
#include <QProcess>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QSettings>

LocalTransport::LocalTransport(QObject* parent) :
		Transport(parent),
		proc(this)
{
	connect(&proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(procError(QProcess::ProcessError)));
	connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(procReadErr()));
	connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(procReadIn()));
	connect(&proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(procStatusUpdate(QProcess::ProcessState)));
	connect(&proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
}

LocalTransport::~LocalTransport()
{
	proc.kill();
}

bool LocalTransport::test()
{
	//qDebug() << "LocalTransport::test";

	QString exe = GnosticApp::getInstance().settings()->value("programs/echo_path", "echo").toString();
	QProcess testProc;
	testProc.start(exe, QStringList() << "hello world");
	if (!testProc.waitForStarted(1000))
	{
		//qDebug() << "LocalTransport::test waitForStarted returned false";
		testProc.kill();
		return false;
	}
	if (!testProc.waitForFinished(1000))
	{
		//qDebug() << "LocalTransport::test  waitForFinished returned false";
		testProc.kill();
		return false;
	}
	QByteArray output = testProc.readAll();
	if (QString(output) != "hello world\n")
	{
		qWarning() << "LocalTransport::test - output was not what was expected:" << output;
		return false;
	}
	return true;
}

bool LocalTransport::start(const QString& exec, const QStringList& args)
{
	setConnectionStatus(Transport::EstablishingConnection);
	proc.start(exec, args);
	if (!proc.waitForStarted(1000))
	{
		qWarning() << "LocalTransport::start is taking way too long to start...";
		return false;
	}
	else
	{
		setConnectionStatus(Transport::Connected);
		return true;
	}
}

void LocalTransport::stop()
{
	if (proc.state() != QProcess::NotRunning)
		proc.kill();
}

const QString LocalTransport::saveSettings()
{
	//qDebug() << "LocalTransport::saveSettings, calling Transport::saveSettings first";
	Transport::saveSettings();
	return id;
	// actually we don't have any settings to save really...  but we should see
	// the type set correctly in the config.ini file...
}

bool LocalTransport::loadSettings(const QString& section)
{
	//qDebug() << "LocalTransport::loadSettings";
	// LocalTransport doesn't have to do anything more than the base class already does...
	return Transport::loadSettings(section);
}


void LocalTransport::dumpDebug()
{
	//qDebug() << "LocalTransport::dumpDebug calling Tranport::dumpDebug()";
	Transport::dumpDebug();
}

TransportConfigWidget* LocalTransport::getConfigWidget(QWidget* parent)
{
	if (!configWidget)
		configWidget = new LocalTransportConfigWidget(this, parent);

	return configWidget;
}

void LocalTransport::procStatusUpdate(QProcess::ProcessState newState)
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

void LocalTransport::procReadIn()
{
	////qDebug() << "LocalTransport::procReadIn";
	QByteArray line;
	while (1)
	{
		line = proc.readLine();
		if (line.isEmpty())
			break;

		////qDebug() << "LocalTransport::procReadIn line" << line;
		emit spewLine(line);
	}
}

void LocalTransport::procReadErr()
{
	qWarning() << "LocalTransport::procReadErr:" << proc.readAllStandardError();
}

void LocalTransport::procError(QProcess::ProcessError err)
{
	qWarning() << "LocalTransport::procError detected error:" << err;
	if (proc.state()==QProcess::NotRunning)
		setConnectionStatus(Transport::Disconnected);
}

void LocalTransport::procDone(int status)
{
	//qDebug() << "LocalTransport::procDone process has ended with status:" << status;
	setConnectionStatus(Transport::Disconnected);
}

