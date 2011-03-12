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
#include <QMap>
#include <QPair>
#include <cmath>

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

bool DummyTransport::test()
{
	return true;
}

bool DummyTransport::start(const QString& exec, const QStringList& args)
{
	Q_UNUSED(exec);
	Q_UNUSED(args);
	//qDebug() << "DummyTransport::start I'm a Dummy, I will ignore params";
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

const QString DummyTransport::saveSettings()
{
	//qDebug() << "DummyTransport::saveSettings, doesn't save anything...";
	return QString();
}

bool DummyTransport::loadSettings(const QString& section)
{
	Q_UNUSED(section);
	//qDebug() << "DummyTransport::loadSettings, doesn't do anything with section" << section;
	return true;
}


void DummyTransport::dumpDebug()
{
	//qDebug() << "DummyTransport::dumpDebug calling Tranport::dumpDebug()";
	Transport::dumpDebug();
	//qDebug() << "DummyTransport::dumpDebug hurp-a-durp";
}

TransportConfigWidget* DummyTransport::getConfigWidget(QWidget* parent)
{
	Q_UNUSED(parent);
	return NULL;
}

RemoteCommandConfigWidget* DummyTransport::getCommandWidget(QWidget* parent)
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
	QMap< QString, double > mid;
	mid.insert("Hurp", 0);
	mid.insert("Durp", 70);
	foreach(QString label, QStringList() << mid.keys())
	{
		qint64 ts = QDateTime::currentMSecsSinceEpoch();
		// every so often, hiccup and send a value older than some previous values...
		if (((int)rand())%20 == 0)
		{
			//qDebug() << "DummyTransport::spew hiccup";
			ts -= 5000;
		}
		emit(spewLine(QString("%1;%2;%3")
			      .arg(ts)
			      .arg(fmod(rand(), 50) - 25 + mid[label])
			      .arg(label)));
	}
}


