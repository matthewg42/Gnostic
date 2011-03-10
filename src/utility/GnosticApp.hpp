#ifndef GNOSTICAPP_HPP
#define GNOSTICAPP_HPP

#include <QObject>
#include "Transport.hpp"
#include <QPen>

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

	//! Looks to see what the most recent QPen was for a given key.  The key is typically
	//! a combination of the monitor name and label name.  In any case it is hashed and
	//! the hash is used as the key in the config.ini file (because we can't control
	//! spaces and other non-legal ini file key characters in the input)
	//! \param key is something unique for the pen in question.
	//! \returns the QPen which was most recently defined for a data item
	//! called "label".  If label is QString(), QPen() is returned.
	QPen getRecentPen(const QString& key);

	//! Saves the a pen for a given label.
	void setRecentPen(const QString& key, QPen pen);

private:
	const QString getIniPath();

	//! takes some srbitrary text and returns a hash from it, which is suitable as a
	//! key in the config.ini file.
	const QString hashPenKey(const QString& key);

private:
	static GnosticApp* singleton;
	QString configDir;
	QSettings* confSettings;

};

#endif // GNOSTICAPP_HPP
