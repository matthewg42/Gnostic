#include "DummyTransport.hpp"
#include "GnosticApp.hpp"
#include <QProcess>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QSettings>

DummyTransport::DummyTransport(QObject* parent) :
		Transport(parent)
{
	timer.setInterval(300);
	timer.setSingleShot(false);
	connect(&timer, SIGNAL(timeout()), this, SLOT(spew()));
}

DummyTransport::~DummyTransport()
{
}

bool DummyTransport::testTransport()
{
	return true;
}

bool DummyTransport::start(const QString& exec, const QStringList& args)
{
	qDebug() << "DummyTransport::start I'm a Dummy, I will ignore params:" <<  exec << args;
	setConnectionStatus(Transport::EstablishingConnection);
	timer.start();
	setConnectionStatus(Transport::Connected);
	return true;
}

void DummyTransport::stop()
{
	timer.stop();
	setConnectionStatus(Transport::Disconnected);
}

const QString DummyTransport::saveTransport()
{
	qDebug() << "DummyTransport::saveTransport, doesn't save anything...";
	return QString();
}

bool DummyTransport::loadSettings(const QString& section)
{
	qDebug() << "DummyTransport::loadSettings, doesn't do anything with section" << section;
	return true;
}


void DummyTransport::dumpDebug()
{
	qDebug() << "DummyTransport::dumpDebug calling Tranport::dumpDebug()";
	Transport::dumpDebug();
	qDebug() << "DummyTransport::dumpDebug hurp-a-durp";
}

TransportConfigWidget* DummyTransport::getConfigWidget(QWidget* parent)
{
	Q_UNUSED(parent);
	return NULL;
}

void DummyTransport::spewHeader()
{
	emit(spewLine("GNOSTIC-DATA-PROTOCOL-VERSION=1.0"));
	emit(spewLine("HOST=hurp.durp.net"));
	emit(spewLine("MONITOR-NAME=Hurp-a-durp"));
	emit(spewLine("DELIMITER=;"));
	emit(spewLine("END-HEADER"));
}

void DummyTransport::spew()
{
	foreach(QString label, QStringList() << "Hurp" << "Durp")
	{
		emit(spewLine(QString("%1;%2;%3")
			      .arg(QDateTime::currentMSecsSinceEpoch())
			      .arg(rand())
			      .arg(label)));
	}
}
