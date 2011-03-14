#include <QSettings>
#include <QVariant>
#include <QDebug>

#include "RemoteCommand.hpp"
#include "RemoteMonitor.hpp"
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
	settings->setValue(QString("%1/type").arg(id), "RemoteCommand");
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
	QSettings* s = GnosticApp::getInstance().settings();
	if (!s->childGroups().contains(section))
	{
		qWarning() << "RemoteCommand::loadSettings no such section in config" << section;
		return false;
	}

	if (s->value(QString("%1/type").arg(section), "").toString() != "RemoteCommand")
	{
		qWarning() << "RemoteCommand::loadSettings section" << section << "incorrect type:" << s->value(QString("%1/type").arg(section), "").toString();
		return false;
	}


	id = section;

	setDescription(s->value(QString("%1/description").arg(section)).toString());
	setTransportId(s->value(QString("%1/transport_id").arg(section)).toString());
	setProgram(s->value(QString("%1/program").arg(section)).toString());
	setArguments(s->value(QString("%1/arguments").arg(section)).toStringList());
	setTimeout(s->value(QString("%1/timeout").arg(section)).toInt());
	setReconnect(s->value(QString("%1/reconnect").arg(section)).toBool());
	setRetries(s->value(QString("%1/retries").arg(section)).toInt());

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

bool RemoteCommand::erase(const QString& section)
{
	QSettings* s = GnosticApp::getInstance().settings();
	if (!s->childGroups().contains(section))
		return false;

	if (s->value(QString("%1/type").arg(section), "").toString() != "RemoteCommand")
		return false;

	RemoteMonitor::removeForCommand(section);
	s->remove(section);
        return true;
}

int RemoteCommand::removeForTransport(const QString& transportId)
{
	if (transportId.isEmpty())
		return 0;

	int count = 0;
	QSettings* s = GnosticApp::getInstance().settings();
	foreach(QString section, getSections())
	{
		if (s->value(QString("%1/transport_id").arg(section), "").toString() == transportId)
		{
			s->remove(section);
			count++;
		}
	}
	return count;
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

void RemoteCommand::autogenDescription()
{
	QStringList a;
	Transport* transport = getTransport();
	if (!transport)
		a << "[no transport]";
	else
		a << transport->getDescription();
	a << " running " << program << arguments;
	setDescription(a.join(" "));
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
