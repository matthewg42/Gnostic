#include "LocalTransport.hpp"
#include "GnosticApp.hpp"
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

bool LocalTransport::testTransport()
{
	qDebug() << "LocalTransport::testTransport";

	QString exe = GnosticApp::getInstance().settings()->value("Programs/echo_path", "echo").toString();
	QProcess testProc;
	testProc.start(exe, QStringList() << "hello world");
	if (!testProc.waitForStarted(1000))
	{
		qDebug() << "LocalTransport::testTransport waitForStarted returned false";
		testProc.kill();
		return false;
	}
	if (!testProc.waitForFinished(1000))
	{
		qDebug() << "LocalTransport::testTransport  waitForFinished returned false";
		testProc.kill();
		return false;
	}
	QByteArray output = testProc.readAll();
	if (QString(output) != "hello world\n")
	{
		qWarning() << "LocalTransport::testTransport - output was not what was expected:" << output;
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

const QString& LocalTransport::saveTransport()
{
	qDebug() << "LocalTransport::saveTransport, calling Transport::saveTransport first";
	Transport::saveTransport();
	return id;
	// actually we don't have any settings to save really...  but we should see
	// the type set correctly in the config.ini file...
}

void LocalTransport::dumpDebug()
{
	qDebug() << "LocalTransport::dumpDebug() calling Tranport::dumpDebug()";
	Transport::dumpDebug();
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
	qDebug() << "LocalTransport::procReadIn";
	QByteArray line;
	while (1)
	{
		line = proc.readLine();
		if (line.isEmpty())
			break;

		emit receivedLine(line);
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
	qDebug() << "LocalTransport::procDone process has ended with status:" << status;
	setConnectionStatus(Transport::Disconnected);
}

