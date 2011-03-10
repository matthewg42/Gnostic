#ifndef DATADISPLAY_HPP
#define DATADISPLAY_HPP

#include <QWidget>
#include <QString>

class DataDisplayConfigWidget;

//! \class DataDisplay
//! This pure virtual class defines the interface for configuring and using
//! diaplay classes.
//! Sub-class this to make your own displays
class DataDisplay : public QWidget
{
	Q_OBJECT

public:
	//! \enum DataTypes lets other classes know which types of data this DataDisplay will process.
	enum DataTypes {
		Lines    = 0x00000001,  //!< Please connect line input to takeLine.
		DataItem = 0x00000002,  //!< Please connect data item input to takeDataItem.
	};

	DataDisplay(QWidget *parent = 0);
	virtual ~DataDisplay() = 0;

	//! Returns a string description of the type, e.g. "GraphDataDisplay"
	virtual const QString getType() = 0;
	virtual DataDisplay::DataTypes wantDataTypes() = 0;

	//! Gets the id for this DataDisplay.
	//! \returns the config.ini section which defines this DataDisplay
	//!          or QString() if this DataDisplay is not saved in the
	//!          config.ini file.
	const QString& getId();

	//! Get a one-line description of the DataDisplay.  This will generally
	//! be something like "Time graph, one minute history", but may also
	//! include other information.
	const QString& getDescription();

	virtual DataDisplayConfigWidget* getConfigWidget(QWidget* parent=0) = 0;

	//! Get a list of display types which are known
	//! \return e.g. ("CounterDataDisplay", "TailerDataDisplay", "GraphDataDisplay")
	static QStringList getAvailableTypes();

	//! Make a new display object of the type "displayType"
	//! \param displayType a string DataDisplay type, e.g. "GraphDataDisplay".
	//! \param parent a QObject which is to be the parent of the new DataDisplay.
	//! \returns NULL if there is a problem (e.g. unknown displayType), else
	//!          a pointer to a new DataDisplay object of the desired type.
	static DataDisplay* makeDataDisplay(const QString& transportType, QWidget* parent=0);

	//! Create a new DataDisplay object from a definition in the config file.
	//! \param id the section of the config.ini file which describes the DataDisplay.
	//! \param parent a pointer to a QObject which is the parent of the new DataDisplay.
	//! \returns NULL if there is a problem (e.g. no such id), else
	//!          a pointer to a new DataDisplay object of the desired type.
	static DataDisplay* loadDataDisplay(const QString& id, QWidget* parent=0);

	//! Sequence generator for making a new and unique display ID.
	//! \returns string something like this: "display_1" where the part
	//!          after the _ is the first integer >= 0 which makes an
	//!          ID which is not an exisiting section in the config.ini file.
	static const QString getNewDataDisplayId();

	//! Get a list of all sections in the config.ini file which describe
	//! DataDisplay objects.
	static QStringList getDataDisplaySections();

public slots:
	//! This is how DataDisplay objects get their data... A connection is made from a
	//! GnosticParser, which emits spewLine and spewDataItem...
	virtual void takeLine(QString line);
	virtual void takeDataItem(const double timestamp, const double value, const QString label);

	//! Set the description of the display
	void setDescription(QString newDescription);

	//! Save DataDisplay settings to the config.ini file.
	//! The current ID of the DataDisplay will be used if it is not
	//! empty, otherwise a new, unique ID will be generated, and set
	//! as the current ID.
	//! In either case the ID after the save is returned.
	virtual const QString& saveDataDisplay() = 0;

	//! Load the settings for this DataDisplay from the specified section of the config.ini
	//! \param section the section in the config.ini file which describes the DataDisplay
	//! \returns true on successful loading, false otherwise.
	virtual bool loadSettings(const QString& section) = 0;

	//! Dump info about the DataDisplay onto debugging output
	virtual void dumpDebug() = 0;

protected:
	QString id;
	QString description;
	DataDisplayConfigWidget* configWidget;

};

#endif // DATADISPLAY_HPP
