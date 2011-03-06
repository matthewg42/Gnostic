#ifndef TRANSPORT_HPP
#define TRANSPORT_HPP

#include <QObject>

class QWidget;

//! \class Transport
//! models an authenitcation route to a remote host
//! keeps a host and other details necessary to establish a connection.
//! A Transaport may prompt the user for additional credentials at
//! connection time (such as password) if they cannot be stored.
//! A transport is used to "do" the remote execution of monitor command
//! and pass the information to a graphing class.
//!
//! The base class is pure virtual - implementation of transport might
//! include TcpSocket connection with no authentication, SSH, telnet,
//! netcat and so on.  A very simple Transport might just be a local shell
//! where there isn't any remote connection at all.
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

	//! Create an empty transort object
	Transport(QObject* parent=0);
	//! Copy contructor, assignment
	Transport(Transport& other, QObject* parent=0);

	virtual ~Transport() = 0;

	//! get the current connection status of a transport
	Transport::TransportStatus getConnectionStatus();

	//! get a widget for configuring the transport.  should have
	//! save and cancel buttons and so on
	virtual QWidget* getConfigWidget() = 0;

public slots:
	//! Use this transport object to run a command
	//! \param exec the program path to be executed
	//! \args a list of arguments to pass to the program
	//! \returns false if there is some bad problem with this transport
	//! object, for example the configuration is not complete
	virtual bool startMonitor(const QString& exec, const QStringList& args) = 0;

	//! Kill the current monitor if it is running
	virtual void stopMonitor() = 0;

	const QString& getId();

	//! Get a one line description of the monitor
	const QString& getDescription();

	//! Set the description of the monitor
	void setDescription(QString newDescription);

	//! Save transport settings
	virtual void saveTransport() = 0;

signals:
	//! This transport is ready to pass data to a grapher
	void receivedData(qint64 timeStamp, QString label, double value);

	//! Changes when the transport's connection status changes
	void connectionStatusChanged(Transport::TransportStatus newStatus);

protected:
	//! Handle changes in status.  If in a subclass you need to take some action like
	//! sending a remote command once the status has changed to connected, you should
	//! re-implement this function and do it there
	virtual void setConnectionStatus(Transport::TransportStatus);

private:
	QString id;
	QString description;
	TransportStatus connectionStatus;

};

#endif // TRANSPORT_HPP
