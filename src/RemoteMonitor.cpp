#include <QSettings>
#include <QDebug>

#include "RemoteMonitor.hpp"
#include "Transport.hpp"
#include "DataDisplay.hpp"
#include "GnosticApp.hpp"
#include "GnosticParser.hpp"
#include "RemoteCommand.hpp"

RemoteMonitor::RemoteMonitor(QObject *parent) :
		QObject(parent)
{
	parser = new GnosticParser(this);
}

RemoteMonitor::~RemoteMonitor()
{
}

const QString& RemoteMonitor::getId()
{
	return id;
}

const QString& RemoteMonitor::getDescription()
{
	return description;
}

// RemoteMonitor::RemoteMonitorConfigWidget* getConfigWidget(QWidget* parent=0)
//{
//}

QStringList RemoteMonitor::getAvailableTypes()
{
	return QStringList() << "RemoteMonitor";
}

RemoteMonitor* RemoteMonitor::makeNew(const QString& type, QObject* parent)
{
	if (type == "RemoteMonitor")
		return new RemoteMonitor(parent);
	else
		return NULL;
}

RemoteMonitor* RemoteMonitor::makeFromConfig(const QString& section, QObject* parent)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
		return NULL;

	QString type = GnosticApp::getInstance().settings()->value(QString("%1/type").arg(section)).toString();
	RemoteMonitor* monitor;
	if (type == "RemoteMonitor")
	{
		RemoteMonitor* m = new RemoteMonitor(parent);
		m->loadSettings(section);
		monitor = m;
	}
//	else if (type == "OtherMonitorType")
//	{
//		OtherMonitorType* m = new OtherMonitorType(parent);
//		m->loadSettings(section);
//		monitor = dynamic_cast<Transport*>(m);
//	}
	else
		return NULL;

	return monitor;
}

const QString RemoteMonitor::getNewId()
{
	QSet<int> existing;
	QRegExp rx("^monitor_(\\d+)$");
	foreach (QString g, GnosticApp::getInstance().settings()->childGroups())
	{
		if (rx.exactMatch(g))
		{
			existing.insert(QVariant(rx.capturedTexts().at(1)).toInt());
		}
	}
	int i=0;
	while(existing.contains(i)) { i++; }
	return QString("monitor_%1").arg(i);
}

QStringList RemoteMonitor::getSections()
{
	QStringList result;
	foreach (QString s, GnosticApp::getInstance().settings()->childGroups())
	{
		if (s.startsWith("monitor_"))
			result << s;
	}
	return result;
}

int RemoteMonitor::removeForCommand(const QString& commandId)
{
	int count;
	foreach (QString section, getSections())
	{
		RemoteMonitor *m = makeFromConfig(section);
		bool removeMe = false;
		foreach(RemoteCommand* c, m->getCommands())
		{
			Q_ASSERT(c);
			if (c->getId() == commandId)
				removeMe = true;
		}
		if (removeMe)
		{
			qDebug() << "RemoteMonitor::removeForCommand removing" << m->getId() << "because it uses" << commandId;
			RemoteMonitor::erase(m->getId());
			count++;
		}
	}
	return count;
}

bool RemoteMonitor::erase(const QString& section)
{
	QSettings* s = GnosticApp::getInstance().settings();
	if (!s->childGroups().contains(section))
		return false;

	if (s->value(QString("%1/type").arg(section), "").toString()!= "RemoteMonitor")
	{
		qWarning() << "RemoteMonitor::erase section" << section << "has incorrect type" << s->value(QString("%1/type").arg(section), "").toString();
		return false;
	}

	s->remove(section);
	return true;
}

RemoteMonitor* RemoteMonitor::quickstart(Transport* transport,
					 const QString& program,
					 const QStringList& args,
					 DataDisplay* display,
					 QObject* parent)
{
	if (!transport || !display)
	{
		qWarning() << "RemoteMonitor::quickstart transport or display was NULL, ABORTING";
		return NULL;
	}

	RemoteMonitor* m = new RemoteMonitor(parent);
	RemoteCommand* c = new RemoteCommand(m);
	c->setTransport(transport);
	c->setProgram(program);
	c->setArguments(args);
	m->addRemoteCommand(c);
	m->addDisplay(display);
	m->start();
	return m;
}


bool RemoteMonitor::start()
{
	// Display the displays
	foreach(DataDisplay* d, displays)
	{
		d->show();
	}

	int failed = 0;
	int succeeded = 0;
	// Start the commands
	foreach(RemoteCommand* c, commands)
	{
		if (c->start())
			succeeded++;
		else
			failed++;
	}

	if (failed)
	{
		qWarning() << "RemoteMonitor::start"<< failed << "RemoteCommand(s) failed to start";
	}
	else
	{
		qDebug() << "RemoteMonitor::start " << succeeded << " started OK, " << failed << " failed to start";
	}
	return (failed==0);
}

void RemoteMonitor::stop()
{	
	foreach(RemoteCommand* command, commands)
	{
		command->stop();
	}

	foreach(DataDisplay* d, displays)
	{
		d->close();
	}
}

void RemoteMonitor::clearDisplays()
{
	qDebug() << "RemoteMonitor::clearDisplays";
	foreach (DataDisplay *d, displays)
	{
		if (d)
			delete d;
	}
	displays.clear();
}

void RemoteMonitor::clearCommands()
{
	qDebug() << "RemoteMonitor::clearCommands";
	foreach (RemoteCommand* c, commands)
	{
		if (c)
			delete c;
	}
	commands.clear();
}

void RemoteMonitor::setDescription(const QString& newDescription)
{
	description = newDescription;
}

void RemoteMonitor::addDisplay(DataDisplay* d)
{
	if (!d)
	{
		qWarning() << "RemoteMonitor::addRemoteCommand NULL display passed";
		return;
	}

	// hook up the displays to the
	if(d->wantDataTypes() & DataDisplay::Lines)
		connect(parser, SIGNAL(spewLine(QString)), d, SLOT(takeLine(QString)));
	if(d->wantDataTypes() & DataDisplay::DataItems)
		connect(parser, SIGNAL(spewDataItem(double,double,QString)), d, SLOT(takeDataItem(double,double,QString)));

	parser->incrementActiveDisplay();
	connect(d, SIGNAL(wasClosed()), parser, SLOT(decrementActiveDisplay()));
        d->setWindowTitle(this->getDescription());
	displays.append(d);

}

void RemoteMonitor::addDisplay(const QString& section)
{
	addDisplay(DataDisplay::makeFromConfig(section));
}

void RemoteMonitor::addRemoteCommand(RemoteCommand* c)
{
	if (!c)
	{
		qWarning() << "RemoteMonitor::addRemoteCommand NULL command passed";
		return;
	}

	if (commands.contains(c))
	{
		qWarning () << "RemoteMonitor::addRemoteCommand I already have this command, not adding again";
		return;
	}

	Transport* t = c->getTransport();
	if (t)
	{
		qDebug() << "RemoteMonitor::addRemoteCommand adding" << t->getId() << t->getDescription();
		connect(t, SIGNAL(spewLine(QString)), parser, SLOT(takeLine(QString)));
		connect(parser, SIGNAL(noMoreDisplays()), t, SLOT(stop()));
		commands.insert(c);
	}
	else
	{
		qWarning() << "RemoteMonitor::addRemoteCommand transport for RemoteCommand" << c->getDescription() << "is NULL, not adding...";
		return;
	}

}

void RemoteMonitor::addRemoteCommand(const QString& section)
{
	addRemoteCommand(RemoteCommand::makeFromConfig(section));
}

const QString RemoteMonitor::saveSettings()
{
	if (id.isEmpty())
		id = getNewId();

	QSettings* s = GnosticApp::getInstance().settings();
	s->beginGroup(id);
	s->setValue("type", getType());
	s->setValue("description", description);

	// clear old commands and displays (in case we're removing some)
	foreach(QString key, s->childKeys())
	{
		if (key.startsWith("command_") || key.startsWith("display_"))
		{
			qDebug() << "RemoteMonitor::saveSettings CLEARING old key" << key;
			s->remove(key);
		}
	}

	int n=0;
	foreach(RemoteCommand* c, commands)
	{
		if (!c->getId().isEmpty())
		{
			s->setValue(QString("command_%2").arg(n), c->getId());
			n++;
		}
	}

	n=0;
	foreach(DataDisplay* d, displays)
	{
		if (!d->getId().isEmpty())
		{
			s->setValue(QString("display_%2").arg(n), d->getId());
			n++;
		}
	}

	s->endGroup();

	return id;
}

bool RemoteMonitor::loadSettings(const QString& section)
{
	if (section.isEmpty())
	{
		qWarning() << "RemoteMonitor::loadSettings empty section";
		return false;
	}

	QSettings* s = GnosticApp::getInstance().settings();
	if (!s->childGroups().contains(section))
	{
		qWarning() << "RemoteMonitor::loadSettings no such section" << section;
		return false;
	}

	if (s->value(QString("%1/type").arg(section), "").toString() != getType())
	{
		qWarning() << "RemoteMonitor::loadSettings section" << section << "has wrong type";
		return false;
	}

	id = section;
	s->beginGroup(section);
	setDescription(s->value("description", "").toString());

	QStringList commandKeys;
	QStringList displayKeys;
	foreach(QString key, s->childKeys())
	{
		if (key.startsWith("command_"))
			commandKeys << s->value(key).toString();
		else if (key.startsWith("display_"))
			displayKeys << s->value(key).toString();
	}
	s->endGroup();

	foreach(QString key, commandKeys)
	{
		addRemoteCommand(key);
	}

	foreach(QString key, displayKeys)
	{
		addDisplay(key);
	}
	return true;
}

void RemoteMonitor::dumpDebug()
{
	qDebug() << "RemoteMonitor::dumpDebug @ " << this;
	qDebug() << "RemoteMonitor::dumpDebug id" << id;
	qDebug() << "RemoteMonitor::dumpDebug description" << description;
	qDebug() << "RemoteMonitor::dumpDebug calling dumpDebug for RemoteCommand objects:";
	foreach (RemoteCommand* c, commands)
	{
		c->dumpDebug();
	}
	qDebug() << "RemoteMonitor::dumpDebug calling dumpDebug for DataDisplay objects:";
	foreach (DataDisplay* d, displays)
	{
		d->dumpDebug();
	}
	qDebug() << "RemoteMonitor::dumpDebug END";
}

