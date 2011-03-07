#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include "GnosticApp.hpp"

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

	confSettings = new QSettings(getIniPath());
	if (!confSettings)
	{
		qCritical() << "Failed to create config.ini:" << getIniPath() << "ABORTING";
		return false;
	}

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

const QString GnosticApp::getNewTransportId()
{
	return "GnosticApp::getNewTransportId TODO";
}

const QString GnosticApp::getIniPath()
{
	return QString("%1/%2").arg(configDir).arg("config.ini");
}
