#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QRegExp>
#include <QSet>
#include <QVariant>
#include <QRegExp>
#include <QCryptographicHash>
#include "GnosticApp.hpp"
#include "LocalTransport.hpp"
#include "PlinkSshTransport.hpp"

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

const QString GnosticApp::getIniPath()
{
	return QString("%1/%2").arg(configDir).arg("config.ini");
}

QPen GnosticApp::getRecentPen(const QString& key)
{
	QString hash = hashPenKey(key);
	qDebug() << "GnosticApp::getRecentPen" << key << " -> " << hash;
	QString serialized = confSettings->value(QString("recent_pens/%1").arg(hash), "").toString();
	if (serialized == "")
		return QPen();

	// The data format is: "s:w:b:c:j"
	// where:
	// s is int cast of enum Qt::PenStyle
	// w is int width
	// b is Color in r;g;b;a format
	// c is int cast of Qt::PenCapStyle enum
	// j is int cast of Qt::PenJoinStyle enum
	//                 s      w       r      g      b      a     c       j
	QRegExp penFmt("^(\\d+):(\\d+):(\\d+);(\\d+);(\\d+);(\\d+):(\\d+):(\\d+)$");
	if (penFmt.exactMatch(serialized))
	{
		Qt::PenStyle s = Qt::PenStyle(QVariant(penFmt.capturedTexts().at(1)).toInt());
		int w = QVariant(penFmt.capturedTexts().at(2)).toInt();
		int r = QVariant(penFmt.capturedTexts().at(3)).toInt();
		int g = QVariant(penFmt.capturedTexts().at(4)).toInt();
		int b = QVariant(penFmt.capturedTexts().at(5)).toInt();
		int a = QVariant(penFmt.capturedTexts().at(6)).toInt();
		Qt::PenCapStyle c = Qt::PenCapStyle(QVariant(penFmt.capturedTexts().at(7)).toInt());
		Qt::PenJoinStyle j = Qt::PenJoinStyle(QVariant(penFmt.capturedTexts().at(8)).toInt());
		QPen pen;
		pen.setStyle(s);
		pen.setWidth(w);
		pen.setBrush(QColor(r, g, b, a));
		pen.setCapStyle(c);
		pen.setJoinStyle(j);
		return pen;
	}
	else
		return QPen();

}

void GnosticApp::setRecentPen(const QString& key, QPen pen)
{
	QString hash = hashPenKey(key);
	QString serialized = QString("%1:%2:%3;%4;%5;%6:%7:%8")
			     .arg((int)pen.style())
			     .arg(pen.width())
			     .arg(pen.color().red())
			     .arg(pen.color().green())
			     .arg(pen.color().blue())
			     .arg(pen.color().alpha())
			     .arg((int)pen.capStyle())
			     .arg((int)pen.joinStyle());

	confSettings->setValue(QString("recent_pens/%1").arg(hash), serialized);
}

const QString GnosticApp::hashPenKey(const QString& key)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(key.toUtf8());
	return hash.result().toHex();
}
