#include "DataDisplay.hpp"
#include "GnosticApp.hpp"
#include "DataDisplayConfigWidget.hpp"

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
	return QStringList(); // TODO when we have types... add them
}

DataDisplay* DataDisplay::makeDataDisplay(const QString& type, QObject* parent)
{
//	if (DataDisplayType == "LocalDataDisplay")
//		return new LocalDataDisplay(parent);
//	else if (DataDisplayType == "PlinkSshDataDisplay")
//		return new PlinkSshDataDisplay(parent);
//	else if (DataDisplayType == "OpenSshDataDisplay")
//		return new OpenSshDataDisplay(parent);
//	else
		return NULL;

}

DataDisplay* DataDisplay::loadDataDisplay(const QString& section, QObject* parent)
{
	if (!GnosticApp::getInstance().settings()->childGroups().contains(section))
		return NULL;

	// TODO: this, when we have display types...

//	QString DataDisplayType = GnosticApp::getInstance().settings()->value(QString("%1/type").arg(section)).toString();
//	DataDisplay* DataDisplay;
//	if (DataDisplayType == "LocalDataDisplay")
//	{
//		LocalDataDisplay* t = new LocalDataDisplay(parent);
//		t->loadSettings(section);
//		DataDisplay = dynamic_cast<DataDisplay*>(t);
//	}
//	else if (DataDisplayType == "PlinkSshDataDisplay")
//	{
//		PlinkSshDataDisplay* t = new PlinkSshDataDisplay(parent);
//		t->loadSettings(section);
//		DataDisplay = dynamic_cast<DataDisplay*>(t);
//	}
//	else if (DataDisplayType == "OpenSshDataDisplay")
//	{
//		OpenSshDataDisplay* t = new OpenSshDataDisplay(parent);
//		t->loadSettings(section);
//		DataDisplay = dynamic_cast<DataDisplay*>(t);
//	}
//	else
		return NULL;

//	return DataDisplay;

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


