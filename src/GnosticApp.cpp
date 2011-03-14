#include <QDir>
#include <QFile>
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
#include "FileUtils.hpp"

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

	// Add the gnostic install directory to the system path
	QStringList paths = getSystemPath(); // from the utils
	paths.push_front(getInstallDir());
	setSystemPath(paths);
	qDebug() << "GnosticApp::init PATH is" << getSystemPath();

	foreach (QString exe, QStringList() << "ssh" << "plink" << "ssh-askpass")
	{
		QString key = QString("paths/%1").arg(exe);
		if (!confSettings->contains(key))
		{
			qDebug() << "GnosticApp::init no path to" << exe << "was found... trying to find it";
			foreach (QString d, getSystemPath())
			{
				QString candidate = QString("%1/%2").arg(d).arg(exe);
				if (QFileInfo(candidate).isExecutable())
				{
					qDebug() << "GnosticApp::init found" << candidate;
					confSettings->setValue(key, candidate);
					break;
				}
				else
				{
					qDebug() << "GnosticApp::init not found" << candidate;
				}
			}
		}
	}

	return true;
}

QSettings* GnosticApp::settings()
{
	return confSettings;
}

const QString GnosticApp::getConfigDir()
{
	return configDir;
}

const QString GnosticApp::getInstallationDir()
{
	// TODO!
	return getInstallDir();
}

const QString GnosticApp::getIniPath()
{
	return QString("%1/%2").arg(configDir).arg("config.ini");
}

QPen GnosticApp::getRecentPen(const QString& label, const QString& display)
{
        QString hash_both = hashPenKey(label+"::"+display);
        QString hash_label = hashPenKey(label);
	//qDebug() << "GnosticApp::getRecentPen" << key << " -> " << hash;
        QString serialized = confSettings->value(QString("recent_pens/%1").arg(hash_both), "").toString();
        if (serialized.isEmpty())
                serialized = confSettings->value(QString("recent_pens/%1").arg(hash_label), "").toString();

        if (serialized.isEmpty())
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

void GnosticApp::setRecentPen(const QString& label, const QString& display, QPen pen)
{
        QString hash_label = hashPenKey(label);
        QString hash_both  = hashPenKey(label+"::"+display);

	QString serialized = QString("%1:%2:%3;%4;%5;%6:%7:%8")
			     .arg((int)pen.style())
			     .arg(pen.width())
			     .arg(pen.color().red())
			     .arg(pen.color().green())
			     .arg(pen.color().blue())
			     .arg(pen.color().alpha())
			     .arg((int)pen.capStyle())
			     .arg((int)pen.joinStyle());

        confSettings->setValue(QString("recent_pens/%1").arg(hash_label), serialized);
        confSettings->setValue(QString("recent_pens/%1").arg(hash_both), serialized);
}

const QString GnosticApp::hashPenKey(const QString& key)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(key.toUtf8());
	return hash.result().toHex();
}

QVector<QColor> GnosticApp::getRecentColors(int max)
{
	QVector<QColor> standardColors;
	standardColors << Qt::red << Qt::darkRed << Qt::green
			<< Qt::darkGreen << Qt::blue << Qt::darkBlue
			<< Qt::cyan << Qt::darkCyan << Qt::magenta
			<< Qt::darkMagenta << Qt::yellow << Qt::darkYellow
			<< Qt::gray << Qt::darkGray << Qt::lightGray
			<< Qt::transparent;

	QVector<QColor> result;
	confSettings->beginGroup("recent_colors");
	QRegExp splitter("(\\d+);(\\d+);(\\d+);(\\d+)");
	QStringList keys = confSettings->childKeys();
	keys.sort();
	foreach(QString key, keys)
	{
		if (splitter.exactMatch(key))
		{
			QColor c(QVariant(splitter.capturedTexts().at(1)).toInt(),
				 QVariant(splitter.capturedTexts().at(1)).toInt(),
				 QVariant(splitter.capturedTexts().at(1)).toInt(),
				 QVariant(splitter.capturedTexts().at(1)).toInt());
			result.push_back(c);
		}
		if (result.size() >= max && max != -1)
			break;
	}
	confSettings->endGroup();

	// in case that wasn't enough, fill the array up with standard colors.
	for(int i=0; result.size() < max; i++)
	{
		result.push_back(standardColors.at(i%standardColors.size()));
	}
	return result;

}

void GnosticApp::addRecentColor(QColor c)
{
	// add a recent color.  only keep up to 100 though.
	QVector<QColor> recent = getRecentColors(-1);
	recent.push_front(c);
	confSettings->remove("recent_colors");
	for(int i=0; i<recent.size() && i < 100; i++)
	{
		confSettings->setValue(QString("recent_colors/col_%1").arg(i, 2, 10, QChar('0')),
				       QString("%1;%2;%3;%4").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha()));
	}
}


