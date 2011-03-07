#include "LocalTransport.hpp"
#include "LocalTransportConfigWidget.hpp"
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
		Transport(parent)
{
	shellPath = "/bin/sh";
	proc = new QProcess(this);
}

LocalTransport::~LocalTransport()
{
	stopMonitor();
}

LocalTransport::LocalTransport(LocalTransport& other, QObject* parent) :
		Transport(parent),
		shellPath(other.shellPath)
{
	proc = new QProcess(this);
}

TransportConfigWidget* LocalTransport::getConfigWidget(QWidget* parent)
{
	TransportConfigWidget* configWidget = new LocalTransportConfigWidget(this, parent);
	return configWidget;
}

bool LocalTransport::testTransport()
{
	qDebug() << "LocalTransport::testTransport testing with shell:" << shellPath;

	QProcess testProc(this);

	testProc.start(shellPath, argsToCmd(QStringList() << "echo" << "hello world"));

	if (!testProc.waitForStarted())
	{
		qDebug() << "LocalTransport::testTransport  waitForStarted returned false";
		return false;
	}

	if (!testProc.waitForFinished())
	{
		qDebug() << "LocalTransport::testTransport  waitForFinished returned false";
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

bool LocalTransport::startMonitor(const QString& exec, const QStringList& args)
{
	this->setConnectionStatus(Transport::EstablishingConnection);

	proc->start(exec, argsToCmd(QStringList() << args));
	return proc->waitForStarted(10000);
}

void LocalTransport::stopMonitor()
{
	if (proc->state() != QProcess::NotRunning)
		proc->kill();
}

void LocalTransport::saveTransport()
{
	qDebug() << "LocalTransport::saveTransport" << id;
	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/type").arg(id), "LocalTransport");
	settings->setValue(QString("%1/description").arg(id), description);
	settings->setValue(QString("%1/shell_path").arg(id), shellPath);
}

void LocalTransport::setShellPath(QString p)
{
	shellPath = p;
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
	QRegExp lineRE("^(\\d+):(\\d+(\\.\\d+)?):(.*)$");
	while (1)
	{
		line = proc->readLine();
		if (line.isEmpty())
			break;

		qDebug() << "read a line of data:" << line;
		if (lineRE.exactMatch(QString(line)))
		{
			qDebug() << "format looks good, got:" << lineRE.capturedTexts();
			bool ok;
			qint64 dataTime = QVariant(lineRE.capturedTexts().at(1)).toLongLong(&ok);
			if (!ok)
			{
				dataTime = QDateTime::currentMSecsSinceEpoch();
				qWarning() << "LocalTransport::procReadIn failed to convert timestamp to LongLong"
						<< lineRE.capturedTexts().at(1)
						<< "using current time";
			}

			double dataValue = QVariant(lineRE.capturedTexts().at(2)).toDouble(&ok);
			if (!ok)
			{
				dataValue = 0;
				qWarning() << "LocalTransport::procReadIn failed to convert data value to double"
						<< lineRE.capturedTexts().at(2)
						<< "using 0";
			}

			qDebug() << "LocalTransport::procReadIn managed to split to:"
					<< dataTime
					<< lineRE.capturedTexts().at(4)
					<< dataValue;

			emit receivedData(lineRE.capturedTexts().at(4), dataValue, dataTime);
		}
	}
}

void LocalTransport::procReadErr()
{
	qWarning() << "LocalTransport::procReadErr:" << proc->readAllStandardError();
}

void LocalTransport::procError(QProcess::ProcessError err)
{
	qWarning() << "LocalTransport::procError detected error:" << err;
}

void LocalTransport::procDone(int status)
{
	qDebug() << "LocalTransport::procDone process has ended with status:" << status;
}

void LocalTransport::makeProcess()
{
	proc = new QProcess(this);
	connect(proc, SIGNAL(error(QProcess::ProcessError)), this, SLOT(procError(QProcess::ProcessError)));
	connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(procReadErr()));
	connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(procReadIn()));
	connect(proc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(procStatusUpdate(QProcess::ProcessState)));
	connect(proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
}

const QStringList LocalTransport::argsToCmd(const QStringList args)
{
	QStringList ret;
	ret << "-c";
	foreach (QString a, args)
	{
		a = QString("'%1'").arg(a);
	}
	ret << args.join(" ");
	return ret;
}

