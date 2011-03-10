#include <QApplication>
#include <QString>

#include "config.hpp"
#include "GnosticApp.hpp"
#include "GnosticMainWindow.hpp"
#include "FileUtils.hpp"

static QString userDir;

int main(int argc, char *argv[])
{
	QCoreApplication::setApplicationName(PROG_NAME);
	QCoreApplication::setApplicationVersion(PROG_VERSION);
	QCoreApplication::setOrganizationDomain(PROG_DOMAIN);
	QCoreApplication::setOrganizationName(PROG_ORG);
	QApplication a(argc, argv);

	GnosticApp gapp(getConfigDir());
	if (!gapp.init())
		return 1;

	// TODO: Modify the environment to the install directory is
	// in the PATH.  This is so we can use our own ssh-askpass and so on.
	GnosticMainWindow w;
	w.show();
	return a.exec();
}
