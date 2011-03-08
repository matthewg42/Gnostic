#include "Transport.hpp"
#include "GnosticApp.hpp"

#include <QSettings>
#include <QDebug>

Transport::Transport(QObject* parent) :
		QObject(parent),
		id(),
		description(),
		connectionStatus(Transport::Disconnected)
{
}

Transport::~Transport()
{
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

void Transport::setDescription(QString newDescription)
{
	description=newDescription;
}

const QString& Transport::saveTransport()
{
	if (id.isEmpty())
		id = GnosticApp::getInstance().getNewTransportId();

	GnosticApp::getInstance().settings()->setValue(QString("%1/type").arg(id), getType());
	GnosticApp::getInstance().settings()->setValue(QString("%1/description").arg(id), description);

	return id;
}

void Transport::dumpDebug()
{
	qDebug() << "Transport::dumpDebug @ "  << (void*)this;
	qDebug() << "Transport::dumpDebug id" << id;
	qDebug() << "Transport::dumpDebug description" << description;
}

void Transport::setConnectionStatus(TransportStatus newStatus)
{
	connectionStatus = newStatus;
	emit(connectionStatusChanged(connectionStatus));
}

