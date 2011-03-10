#include "DataDisplay.hpp"
#include "GnosticApp.hpp"
#include "DataDisplayConfigWidget.hpp"
#include "LineCounterDisplay.hpp"
#include "TailerDisplay.hpp"
#include "TimeGraphDisplay.hpp"

#include <QSettings>
#include <QDebug>
#include <QCloseEvent>

DataDisplay::DataDisplay(QWidget* parent) :
		QWidget(parent),
		id(),
		description()
{
	configWidget = NULL;
}

DataDisplay::~DataDisplay()
{
	if (configWidget)
		delete configWidget;
}

const QString& DataDisplay::getId()
{
	return id;
}

const QString& DataDisplay::getDescription()
{
	return description;
}

QStringList DataDisplay::getAvailableTypes()
{
	return QStringList() << "LineCounterDisplay" << "TailerDisplay" << "TimeGraphDisplay";
}

DataDisplay* DataDisplay::makeDataDisplay(const QString& type, QWidget* parent)
{
	if (type == "LineCounterDisplay")
		return new LineCounterDisplay(parent);
	else if (type == "TailerDisplay")
		return new TailerDisplay(parent);
	else if (type == "TimeGraphDisplay")
		return new TimeGraphDisplay(parent);
	else
	{
		qWarning() << "DataDisplay::makeDataDisplay UNKNOWN type" << type;
		return NULL;
	}

}

DataDisplay* DataDisplay::loadDataDisplay(const QString& section, QWidget* parent)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
		return NULL;

	QString type = GnosticApp::getInstance().settings()->value(QString("%1/type").arg(section)).toString();
	DataDisplay* display;
	if (type == "LineCounterDisplay")
	{
		LineCounterDisplay* d = new LineCounterDisplay(parent);
		d->loadSettings(section);
		display = dynamic_cast<DataDisplay*>(d);
	}
	else if (type == "TailerDisplay")
	{
		TailerDisplay* d = new TailerDisplay(parent);
		d->loadSettings(section);
		display = dynamic_cast<DataDisplay*>(d);
	}
	else if (type == "TimeGraphDisplay")
	{
		TimeGraphDisplay* d = new TimeGraphDisplay(parent);
		d->loadSettings(section);
		display = dynamic_cast<DataDisplay*>(d);
	}
	else
	{
		qWarning() << "DataDisplay::loadDataDisplay section" << section << "is for a type" << type << "I don't recognize, returning NULL";
		return NULL;
	}

	return display;

}

const QString DataDisplay::getNewDataDisplayId()
{
	QSet<int> existing;
	QRegExp rx("^display_(\\d+)$");
	foreach (QString g, GnosticApp::getInstance().settings()->childGroups())
	{
		if (rx.exactMatch(g))
		{
			existing.insert(QVariant(rx.capturedTexts().at(1)).toInt());
		}
	}
	int i=0;
	while(existing.contains(i)) { i++; }
	return QString("display_%1").arg(i);
}

QStringList DataDisplay::getDataDisplaySections()
{
	QStringList result;
	foreach (QString s, GnosticApp::getInstance().settings()->childGroups())
	{
		if (s.startsWith("display_"))
			result << s;
	}
	return result;
}

void DataDisplay::takeLine(QString line)
{
	Q_UNUSED(line);
}

void DataDisplay::takeDataItem(const double timestamp, const double value, const QString label)
{
	Q_UNUSED(timestamp);
	Q_UNUSED(value);
	Q_UNUSED(label);
}

void DataDisplay::setDescription(QString newDescription)
{
	description=newDescription;
}

const QString& DataDisplay::saveDataDisplay()
{
	if (id.isEmpty())
		id = getNewDataDisplayId();

	GnosticApp::getInstance().settings()->setValue(QString("%1/type").arg(id), getType());
	GnosticApp::getInstance().settings()->setValue(QString("%1/description").arg(id), description);

	return id;
}

bool DataDisplay::loadSettings(const QString& section)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
	{
		qWarning() << "DataDisplay::loadSettings no such section in config" << section;
		return false;
	}

	if (GnosticApp::getInstance().settings()->value(QString("%1/type").arg(section)).toString() != getType())
	{
		qWarning() << "DataDisplay::loadSettings section" << section << "doesn't refer to a" << getType();
		return false;
	}
	id = section;
	description = GnosticApp::getInstance().settings()->value(QString("%1/description").arg(section)).toString();

	return true;
}


void DataDisplay::dumpDebug()
{
	qDebug() << "DataDisplay::dumpDebug @ "  << (void*)this;
	qDebug() << "DataDisplay::dumpDebug id" << id;
	qDebug() << "DataDisplay::dumpDebug description" << description;
}

void DataDisplay::closeEvent(QCloseEvent *event)
{
	event->accept();
	emit(wasClosed());
}


