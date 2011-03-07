#ifndef GNOSTICAPP_HPP
#define GNOSTICAPP_HPP

#include <QObject>
#include "Transport.hpp"

class QSettings;


//! \class GnosticApp
//! This singleton class provides Gnostic-wide services to all parts of the app who want them.
//! This includes access to the global QSettings object.
class GnosticApp : public QObject
{
	Q_OBJECT
public:
	//! Constructor
	//! \param confDir the path to the configuration directory to use.  This is platform-specific.
	//! \param parent the QObject parent if desired.
	explicit GnosticApp(const QString& confDir, QObject *parent=0);
	~GnosticApp();

	//! Gets a referece to the singleton instance of the class.
	static GnosticApp& getInstance();

	//! Initializes some things
	bool init();

	//! Get a pointer to the global QSettings object which is used all over the place.
	QSettings* settings();

	//! Get the configuration directory path.
	const QString& getConfigDir();

	//! Get a list of all known transport IDs
	QStringList getTransportIds();

	//! Create a Transport object with the settings from the specified section of
	//! the config.ini file.
	//! \param id the section of the ini file containing the settings for
	//!        the transport.
	//! \returns a pointer to a new Transport object from the settings in the
	//!          config.ini file, or NULL if no section "id" exists (or for some
	//!          reason doesn't look right).
	Transport* getTransportWithId(const QString& id);

	//! Sequence generator for making a new and unique transport ID.
	//! \returns string something like this: "transport_1" where the part
	//!          after the _ is the first integer >= 0 which makes an
	//!          ID which is not an exisiting section in the config.ini file.
	const QString getNewTransportId();

private:
	const QString getIniPath();

private:
	static GnosticApp* singleton;
	QString configDir;
	QSettings* confSettings;

};

#endif // GNOSTICAPP_HPP
