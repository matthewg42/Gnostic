#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

#include <QObject>
#include <QStringList>

class TransportConfigWidget;
class QWidget;

//! \class Transport
//! Models a method of calling a program somewhere (probably a remote host)
//! and transporting the stream of output from that program back to Gnostic.
//! A Transaport may prompt the user for additional credentials at
//! connection time (such as password) if they cannot be stored.
//! A transport is used to "do" the remote execution of monitor command
//! and pass the information to a graphing class.
class Transport : public QObject
{
	Q_OBJECT

public:
	enum TransportStatus
	{
		Disconnected,
		EstablishingConnection,
		Connected
	};

	Transport(QObject* parent=0);
	virtual ~Transport() = 0;

	//! Returns a string description of the type, e.g. "Transport"
	virtual const QString getType() = 0;

	//! Gets the id for this Transport.
	//! \returns the config.ini section which defines this Transport
	//!          or QString() if this Transport is not saved in the
	//!          config.ini file.
	const QString& getId();

	//! Get a one-line description of the transport.  This will generally
	//! be something like a device description, but may also include other
	//! information.  A typical value might be "secondary DNS".
	const QString& getDescription();

	//! Get the current connection status of a transport
	Transport::TransportStatus getConnectionStatus();

	//! Test that the transport works with the current settings
	//! e.g. that it can connect and authenticate with a remote host,
	//! issue a command and read back the output.
	//! \returns true on a successful test, false otherwise.
	virtual bool testTransport() = 0;

	//! Get a widget containing controls necessary to configure this Transport.
	virtual TransportConfigWidget* getConfigWidget(QWidget* parent=0) = 0;

	//! Get a list of transport types which are known
	//! \return e.g. ("LocalTransport", "PlinkSshTransport", "OpenSshTransport")
	static QStringList getAvailableTypes();

	//! Make a new transport object of the type "transportType"
	//! \param transportType a string Transport type, e.g. "LocalTransport".
	//! \param parent a QObject which is to be the parent of the new Transport.
	//! \returns NULL if there is a problem (e.g. unknown transportType), else
	//!          a pointer to a new Trasport object of the desired type.
	static Transport* makeTransport(const QString& transportType, QObject* parent=0);

	//! Create a new Transport object from a definition in the config file.
	//! \param id the section of the config.ini file which describes the transport.
	//! \param parent a pointer to a QObject which is the parent of the new transport.
	//! \returns NULL if there is a problem (e.g. no such id), else
	//!          a pointer to a new Trasport object of the desired type.
	static Transport* loadTransport(const QString& id, QObject* parent=0);

	//! Sequence generator for making a new and unique transport ID.
	//! \returns string something like this: "transport_1" where the part
	//!          after the _ is the first integer >= 0 which makes an
	//!          ID which is not an exisiting section in the config.ini file.
	static const QString getNewTransportId();

	//! Get a list of all sections in the config.ini file which describe
	//! Transport objects.
	static QStringList getTransportSections();

public slots:
	//! Use this transport object to run a command
	//! \param exec the program path to be executed
	//! \args a list of arguments to pass to the program
	//! \returns false if there is some bad problem with this transport
	//! object, for example the configuration is not complete
	virtual bool start(const QString& exec, const QStringList& args=QStringList()) = 0;

	//! Kill the current monitor if it is running
	virtual void stop() = 0;

	//! Set the description of the monitor
	void setDescription(QString newDescription);

	//! Save transport settings to the config.ini file.
	//! The current ID of the Transport will be used if it is not
	//! empty, otherwise a new, unique ID will be generated, and set
	//! as the current ID.
	//! In either case the ID after the save is returned.
	virtual const QString& saveTransport() = 0;

	//! Load the settings for this transport from the specified section of the config.ini
	//! \param section the section in the config.ini file which describes the Transport
	//! \returns true on successful loading, false otherwise.
	virtual bool loadSettings(const QString& section) = 0;

	//! Dump info about the transport onto debugging output
	virtual void dumpDebug() = 0;

signals:
	//! This transport is ready to pass data to a monitor
	//! Transports are concerned only with passing data around line by line,
	//! not what is in it.
	void receivedLine(QString line);

	//! Changes when the transport's connection status changes
	void connectionStatusChanged(Transport::TransportStatus newStatus);

protected:
	//! Handle changes in status.  If in a subclass you need to take some action like
	//! sending a remote command once the status has changed to connected, you should
	//! re-implement this function and do it there
	virtual void setConnectionStatus(Transport::TransportStatus);
	QString id;
	QString description;
	TransportStatus connectionStatus;
	TransportConfigWidget* configWidget;

};

#endif // TRANSPORT_HPP
