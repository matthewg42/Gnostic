#include "DataDisplay.hpp"
#include "GnosticApp.hpp"
#include "DataDisplayConfigWidget.hpp"
#include "LineCounterDisplay.hpp"

#include <QSettings>
#include <QDebug>

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
	return QStringList() << "LineCounterDisplay";
}

DataDisplay* DataDisplay::makeDataDisplay(const QString& type, QWidget* parent)
{
	if (type == "LineCounterDisplay")
		return new LineCounterDisplay(parent);
//	else if (DataDisplayType == "PlinkSshDataDisplay")
//		return new PlinkSshDataDisplay(parent);
//	else if (DataDisplayType == "OpenSshDataDisplay")
//		return new OpenSshDataDisplay(parent);
//	else
		return NULL;

}

DataDisplay* DataDisplay::loadDataDisplay(const QString& section, QWidget* parent)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
		return NULL;

	QString DataDisplayType = GnosticApp::getInstance().settings()->value(QString("%1/type").arg(section)).toString();
	DataDisplay* display;

	if (DataDisplayType == "LineCounterDisplay")
	{
		LineCounterDisplay* d = new LineCounterDisplay(parent);
		d->loadSettings(section);
		display = dynamic_cast<DataDisplay*>(d);
	}
	else
		return NULL;

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
}

void DataDisplay::takeDataItem(const double timestamp, const double value, const QString label)
{
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


