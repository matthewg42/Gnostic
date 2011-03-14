#include "config.hpp"
#include "FileUtils.hpp"
#include <QDir>
#include <QFileInfo>
#include <cstdlib>
#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>

const QString getConfigDir()
{
	QString configDir;

#ifdef Q_OS_WIN
	QString winApiPath = getWin32SpecialDirPath(CSIDL_APPDATA);
	if (!winApiPath.isEmpty())
	{
		winApiPath.replace("\\", "/");
		configDir = QString("%1/%2")
			    .arg(winApiPath)
			    .arg(PROG_NAME);
	}
	else
	{
		configDir = QDir::homePath() + "/" + PROG_NAME;
	}
#elif defined(Q_OS_MAC)
	configDir = QString("%1/Library/Application Support/%2")
		    .arg(QDir::homePath())
		    .arg(PROG_NAME);
#else
	configDir = QString("%1/.%2")
		    .arg(QDir::homePath())
		    .arg(PROG_NAME).toLower();
#endif
	return configDir;
}


const QString getInstallDir()
{
	return QFileInfo(QCoreApplication::argv()[0]).dir().path();
}

QStringList getSystemPath()
{
#ifdef Q_OS_WIN
	// stupid windows doing everything annoying different from sane OSes.
	return QString(getenv("PATH")).split(";");
#else
	return QString(getenv("PATH")).split(":");
#endif
}

void setSystemPath(const QStringList& path)
{
#ifdef Q_OS_WIN
	// stupid windows doing everything annoying different from sane OSes.

        gnosticSetenv("PATH", path.join(";").toLocal8Bit());
#else
        gnosticSetenv("PATH", path.join(":").toLocal8Bit());
#endif
}

void gnosticSetenv(const QString& variable, const QString& value)
{
#ifdef Q_OS_WIN
        if (! SetEnvironmentVariable(variable.toLocal8Bit().constData(), value.toLocal8Bit().constData()))
        {
                qWarning() << "gnosticSetenv: SetEnvironmentVariable failed" << variable << value;
        }
#else
	setenv(variable.toLocal8Bit().constData(), value.toLocal8Bit().constData(), 1);
#endif
}

#ifdef Q_OS_WIN
QString getWin32SpecialDirPath(int csidlId)
{
	// This function is implemented using code from QSettings implementation in QT
	// (GPL edition, version 4.3).
	QLibrary library(QLatin1String("shell32"));
	QT_WA( {
		typedef BOOL (WINAPI*GetSpecialFolderPath)(HWND, LPTSTR, int, BOOL);
		GetSpecialFolderPath SHGetSpecialFolderPath = (GetSpecialFolderPath)library.resolve("SHGetSpecialFolderPathW");
		if (SHGetSpecialFolderPath)
		{
			TCHAR tpath[MAX_PATH];
			SHGetSpecialFolderPath(0, tpath, csidlId, FALSE);
			return QString::fromUtf16((ushort*)tpath);
		}
	} , {
	typedef BOOL (WINAPI*GetSpecialFolderPath)(HWND, char*, int, BOOL);
	GetSpecialFolderPath SHGetSpecialFolderPath = (GetSpecialFolderPath)library.resolve("SHGetSpecialFolderPathA");
	if (SHGetSpecialFolderPath)
	{
		char cpath[MAX_PATH];
		SHGetSpecialFolderPath(0, cpath, csidlId, FALSE);
		return QString::fromLocal8Bit(cpath);
	}
} );

	return QString();
}
#endif

