#include "Transport.hpp"
#include "GnosticApp.hpp"

#include <QSettings>

Transport::Transport(QObject* parent) :
		QObject(parent),
		description("new transport"),
		connectionStatus(Disconnected),
		tested(false)
{
	id = GnosticApp::getInstance().getNewTransportId();
}

Transport::Transport(Transport& other, QObject* parent) :
		QObject(parent),
		connectionStatus(Disconnected)
{
	id = GnosticApp::getInstance().getNewTransportId();
	description = other.description;
	tested = other.tested;
}

Transport::TransportStatus Transport::getConnectionStatus()
{
	return connectionStatus;
}

Transport::~Transport()
{
	// gndn.
}

const QString& Transport::getId()
{
	return id;
}

const QString& Transport::getDescription()
{
	return description;
}

void Transport::setDescription(QString newDescription)
{
	description=newDescription;
}

void Transport::setTested(bool t)
{
	tested = t;
}

bool Transport::getTested()
{
	return tested;
}

void Transport::setConnectionStatus(TransportStatus newStatus)
{
	connectionStatus = newStatus;
	emit(connectionStatusChanged(connectionStatus));
}


