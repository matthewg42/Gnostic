#ifndef GNOSTICAPP_HPP
#define GNOSTICAPP_HPP

#include <QObject>

class QSettings;

class GnosticApp : public QObject
{
	Q_OBJECT
public:
	explicit GnosticApp(const QString& confDir, QObject *parent=0);
	~GnosticApp();
	static GnosticApp& getInstance();
	bool init();
	QSettings* settings();
	const QString& getConfigDir();
	const QString getNewTransportId();

private:
	const QString getIniPath();

private:
	static GnosticApp* singleton;
	QString configDir;
	QSettings* confSettings;
};

#endif // GNOSTICAPP_HPP
