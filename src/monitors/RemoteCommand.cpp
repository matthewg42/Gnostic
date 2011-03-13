#include <QSettings>
#include <QVariant>
#include <QDebug>

#include "RemoteCommand.hpp"
#include "GnosticApp.hpp"
#include "Transport.hpp"

RemoteCommand::RemoteCommand(QObject *parent) :
		QObject(parent),
		timeout(120),
		reconnect(true),
		retries(3)
{
	transport = NULL;
}

RemoteCommand::~RemoteCommand()
{
}

const QString RemoteCommand::saveSettings()
{
	if (id.isEmpty())
		id = getNewId();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/description").arg(id), description);
	settings->setValue(QString("%1/transport_id").arg(id), getTransportId());
	settings->setValue(QString("%1/program").arg(id), getProgram());

	if (arguments.isEmpty())
		settings->remove(QString("%1/arguments").arg(id));
	else
		settings->setValue(QString("%1/arguments").arg(id), getArguments());

	settings->setValue(QString("%1/timeout").arg(id), getTimeout());
	settings->setValue(QString("%1/reconnect").arg(id), getReconnect());
	settings->setValue(QString("%1/retries").arg(id), getRetries());

	return id;
}

bool RemoteCommand::loadSettings(const QString& section)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
	{
		qWarning() << "RemoteCommand::loadSettings no such section in config" << section;
		return false;
	}

	id = section;

	setDescription(GnosticApp::getInstance().settings()->value(QString("%1/description").arg(section)).toString());
	setTransportId(GnosticApp::getInstance().settings()->value(QString("%1/transport_id").arg(section)).toString());
	setProgram(GnosticApp::getInstance().settings()->value(QString("%1/program").arg(section)).toString());
	setArguments(GnosticApp::getInstance().settings()->value(QString("%1/arguments").arg(section)).toStringList());
	setTimeout(GnosticApp::getInstance().settings()->value(QString("%1/timeout").arg(section)).toInt());
	setReconnect(GnosticApp::getInstance().settings()->value(QString("%1/reconnect").arg(section)).toBool());
	setRetries(GnosticApp::getInstance().settings()->value(QString("%1/retries").arg(section)).toInt());

	return true;
}

Transport* RemoteCommand::getTransport()
{
	if (!transport)
		transport = Transport::makeFromConfig(getTransportId());

	return transport;
}

void RemoteCommand::setTransport(Transport* t)
{
	if (!t)
		return;

	if (transport)
	{
		qWarning() << "RemoteCommand::setTransport there was already a transport exisitng, but we nuked it...";
	}

	transport = t;
	transportId = transport->getId();
}

//RemoteCommandConfigWidget* RemoteCommand::getConfigWidget(QWidget* parent)
//{
//	// TODO
//	return NULL;
//}

RemoteCommand* RemoteCommand::makeFromConfig(const QString& section)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
	{
		qWarning() << "RemoteCommand::makeFromConfig no such section in config" << section;
		return NULL;
	}

	RemoteCommand* c = new RemoteCommand();
	if (!c->loadSettings(section))
	{
		delete c;
		c = NULL;
	}

	return c;
}

QStringList RemoteCommand::getSections()
{
	QStringList result;
	foreach (QString s, GnosticApp::getInstance().settings()->childGroups())
	{
		if (s.startsWith("remote_command_"))
			result << s;
	}
	return result;
}

const QString RemoteCommand::getNewId()
{
	QSet<int> existing;
	QRegExp rx("^remote_command_(\\d+)$");
	foreach (QString g, GnosticApp::getInstance().settings()->childGroups())
	{
		if (rx.exactMatch(g))
		{
			existing.insert(QVariant(rx.capturedTexts().at(1)).toInt());
		}
	}
	int i=0;
	while(existing.contains(i)) { i++; }
	return QString("remote_command_%1").arg(i);
}

bool RemoteCommand::setTransportId(const QString& id)
{
	if (transport)
	{
		qWarning() << "RemoteCommand::setTransportId transport has already been created for the old id" << transportId << "NOT changing it!";
		return false;
	}
	else
	{
		transportId = id;
		return true;
	}
}

void RemoteCommand::dumpDebug()
{
	qDebug() << "RemoteCommand::dumpDebug" << this << "program:" << program << "args:" << arguments << "transport:" << GnosticApp::getInstance().settings()->value( QString("%1/description").arg(getTransportId()), "[unknown transport]").toString();
}

bool RemoteCommand::start()
{
	Transport* t = this->getTransport();
	if (t)
		return t->start(program, arguments);
	else
		return false;
}

void RemoteCommand::stop()
{
	Transport* t = this->getTransport();
	if (t)
		t->stop();

}
