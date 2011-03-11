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

	//qDebug() << "RemoteMonitor::start " << succeeded << " started OK, " << failed << " failed to start";
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
		connect(t, SIGNAL(spewLine(QString)), parser, SLOT(takeLine(QString)));
	}
	else
	{
		qWarning() << "RemoteMonitor::addRemoteCommand transport for RemoteCommand" << c->getDescription() << "is NULL, not adding...";
		return;
	}

	commands.insert(c);
}

void RemoteMonitor::addRemoteCommand(const QString& section)
{
	addRemoteCommand(RemoteCommand::makeFromConfig(section));
}

const QString RemoteMonitor::saveSettings()
{
	if (id.isEmpty())
		id = getNewId();

	GnosticApp::getInstance().settings()->setValue(QString("%1/type").arg(id), getType());
	GnosticApp::getInstance().settings()->setValue(QString("%1/description").arg(id), description);

	int n=0;
	foreach(RemoteCommand* c, commands)
	{
		if (!c->getId().isEmpty())
		{
			GnosticApp::getInstance().settings()->setValue(
					QString("%1/command_%2").arg(id).arg(n),
					c->getId());
			n++;
		}
	}

	n=0;
	foreach(DataDisplay* d, displays)
	{
		if (!d->getId().isEmpty())
		{
			GnosticApp::getInstance().settings()->setValue(
					QString("%1/display_%2").arg(id).arg(n),
					d->getId());
			n++;
		}
	}

	return id;
}

bool RemoteMonitor::loadSettings(const QString& section)
{
	// TODO!
	return false;
}

void RemoteMonitor::dumpDebug()
{
	//qDebug() << "RemoteMonitor::dumpDebug @ " << this;
	//qDebug() << "RemoteMonitor::dumpDebug id" << id;
	//qDebug() << "RemoteMonitor::dumpDebug description" << description;
	//qDebug() << "RemoteMonitor::dumpDebug calling dumpDebug for RemoteCommand objects:";
	foreach (RemoteCommand* c, commands)
	{
		c->dumpDebug();
	}
	//qDebug() << "RemoteMonitor::dumpDebug calling dumpDebug for DataDisplay objects:";
	foreach (DataDisplay* d, displays)
	{
		d->dumpDebug();
	}
	//qDebug() << "RemoteMonitor::dumpDebug END";
}

