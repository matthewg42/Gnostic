#include <QApplication>
#include <QString>
#include <QDir>

#include "config.hpp"
#include "GnosticApp.hpp"
#include "GnosticMainWindow.hpp"

static QString userDir;

#ifdef Q_OS_WIN
# include <windows.h>
# ifndef _SHOBJ_H
# include <shlobj.h>
# include <QLibrary>
# endif
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

int main(int argc, char *argv[])
{
	QCoreApplication::setApplicationName(PROG_NAME);
	QCoreApplication::setApplicationVersion(PROG_VERSION);
	QCoreApplication::setOrganizationDomain(PROG_DOMAIN);
	QCoreApplication::setOrganizationName(PROG_ORG);
	QApplication a(argc, argv);

	// Determine where the config directory is... this is where all settings for the
	// program live, and is what will get put into one big file when settings are
	// exported and imported...
	// This is necessarily platform specific.  I wish there was a QT way to get this
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

	GnosticApp gapp(configDir);
	if (!gapp.init())
		return 1;

	GnosticMainWindow w;
	w.show();
	return a.exec();
}
