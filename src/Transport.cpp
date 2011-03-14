#include "Transport.hpp"
#include "GnosticApp.hpp"
#include "LocalTransport.hpp"
#include "PlinkSshTransport.hpp"
#include "OpenSshTransport.hpp"
#include "TransportConfigWidget.hpp"
#include "RemoteCommandConfigWidget.hpp"

#include <QSettings>
#include <QDebug>

Transport::Transport(QObject* parent) :
		QObject(parent),
		id(),
		description(),
		connectionStatus(Transport::Disconnected)
{
	configWidget = NULL;
	commandWidget = NULL;
}

Transport::~Transport()
{
	if (configWidget)
		delete configWidget;

	if (commandWidget)
		commandWidget->deleteLater();
}

const QString& Transport::getId()
{
	return id;
}

const QString& Transport::getDescription()
{
	return description;
}

Transport::TransportStatus Transport::getConnectionStatus()
{
	return connectionStatus;
}

QStringList Transport::getAvailableTypes()
{
        // return QStringList() << "PlinkSshTransport" << "OpenSshTransport" << "LocalTransport";
        return QStringList() << "PlinkSshTransport" << "LocalTransport";
}

Transport* Transport::makeNew(const QString& type, QObject* parent)
{
	if (type == "LocalTransport")
		return new LocalTransport(parent);
	else if (type == "PlinkSshTransport")
		return new PlinkSshTransport(parent);
	else if (type == "OpenSshTransport")
		return new OpenSshTransport(parent);
	else
		return NULL;

}

Transport* Transport::makeFromConfig(const QString& section, QObject* parent)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
		return NULL;

	QString transportType = GnosticApp::getInstance().settings()->value(QString("%1/type").arg(section)).toString();
	Transport* transport;
	if (transportType == "LocalTransport")
	{
		LocalTransport* t = new LocalTransport(parent);
		t->loadSettings(section);
		transport = dynamic_cast<Transport*>(t);
	}
	else if (transportType == "PlinkSshTransport")
	{
		PlinkSshTransport* t = new PlinkSshTransport(parent);
		t->loadSettings(section);
		transport = dynamic_cast<Transport*>(t);
	}
	else if (transportType == "OpenSshTransport")
	{
		OpenSshTransport* t = new OpenSshTransport(parent);
		t->loadSettings(section);
		transport = dynamic_cast<Transport*>(t);
	}
	else
		return NULL;

	return transport;

}

const QString Transport::getNewId()
{
	QSet<int> existing;
	QRegExp rx("^transport_(\\d+)$");
	foreach (QString g, GnosticApp::getInstance().settings()->childGroups())
	{
		if (rx.exactMatch(g))
		{
			existing.insert(QVariant(rx.capturedTexts().at(1)).toInt());
		}
	}
	int i=0;
	while(existing.contains(i)) { i++; }
	return QString("transport_%1").arg(i);
}

QStringList Transport::getSections()
{
	QStringList result;
	foreach (QString s, GnosticApp::getInstance().settings()->childGroups())
	{
		if (s.startsWith("transport_"))
			result << s;
	}
	return result;
}

void Transport::setDescription(QString newDescription)
{
	description=newDescription;
}

const QString Transport::saveSettings()
{
	if (id.isEmpty())
		id = getNewId();

	GnosticApp::getInstance().settings()->setValue(QString("%1/type").arg(id), getType());
	GnosticApp::getInstance().settings()->setValue(QString("%1/description").arg(id), description);

	return id;
}

bool Transport::loadSettings(const QString& section)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
	{
		qWarning() << "Transport::loadSettings no such section in config" << section;
		return false;
	}

	if (GnosticApp::getInstance().settings()->value(QString("%1/type").arg(section)).toString() != getType())
	{
		qWarning() << "Transport::loadSettings section" << section << "doesn't refer to a" << getType();
		return false;
	}
	id = section;
	description = GnosticApp::getInstance().settings()->value(QString("%1/description").arg(section)).toString();

	return true;
}


void Transport::dumpDebug()
{
	//qDebug() << "Transport::dumpDebug @ "  << (void*)this;
	//qDebug() << "Transport::dumpDebug id" << id;
	//qDebug() << "Transport::dumpDebug description" << description;
}

void Transport::setConnectionStatus(TransportStatus newStatus)
{
	connectionStatus = newStatus;
	emit(connectionStatusChanged(connectionStatus));
}

