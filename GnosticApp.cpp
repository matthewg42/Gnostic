#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QRegExp>
#include <QSet>
#include <QVariant>
#include "GnosticApp.hpp"
#include "LocalTransport.hpp"

GnosticApp* GnosticApp::singleton = NULL;

GnosticApp::GnosticApp(const QString& confDir, QObject *parent) :
		QObject(parent),
		configDir(confDir),
		confSettings(NULL)
{
	Q_ASSERT(!singleton);
	singleton = this;
	setObjectName("GnosticApp");
}

GnosticApp::~GnosticApp()
{
}

GnosticApp& GnosticApp::getInstance()
{
	Q_ASSERT(singleton);
	return *singleton;
}

bool GnosticApp::init()
{
	QDir cd(configDir);
	if (!cd.exists())
	{
		if (!cd.mkpath(configDir))
		{
			qCritical() << "Failed to create config dir:" << configDir << "ABORTING";
			return false;
		}
	}

	confSettings = new QSettings(getIniPath(), QSettings::IniFormat);
	if (!confSettings)
	{
		qCritical() << "Failed to create config.ini:" << getIniPath() << "ABORTING";
		return false;
	}

	qDebug() << "GnosticApp::GnosticApp ini path is" << getIniPath();
	return true;
}

QSettings* GnosticApp::settings()
{
	return confSettings;
}

const QString& GnosticApp::getConfigDir()
{
	return configDir;
}

QStringList GnosticApp::getTransportIds()
{
	QStringList ids;
	qDebug() << "GnosticApp::getTransportIds";
	QRegExp rx("^transport_(\\d+)$");
	foreach (QString g, confSettings->childGroups())
	{
		qDebug() << "found a child group" << g;
		if (rx.exactMatch(g))
		{
			ids << g;
		}
	}
	return ids;
}

Transport* GnosticApp::getTransportWithId(const QString& id)
{
	if (!confSettings->childGroups().contains(id))
	{
		return NULL;
	}

	confSettings->beginGroup(id);
	QString transportType = confSettings->value("type", "").toString();
	if (transportType == "LocalTransport")
	{
		LocalTransport* lt = new LocalTransport();
		lt->id = id;
		lt->setDescription(confSettings->value("description", "").toString());
		lt->setShellPath(confSettings->value("shell_path", "").toString());
		confSettings->endGroup();
		return dynamic_cast<Transport*>(lt);
	}
	else {
		qWarning() << "GnosticApp::getTransportWithId unknown type:" << transportType;
		confSettings->endGroup();
		return NULL;
	}
}

const QString GnosticApp::getNewTransportId()
{
	QSet<int> existing;
	QRegExp rx("^transport_(\\d+)$");
	foreach (QString g, confSettings->childGroups())
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

const QString GnosticApp::getIniPath()
{
	return QString("%1/%2").arg(configDir).arg("config.ini");
}
