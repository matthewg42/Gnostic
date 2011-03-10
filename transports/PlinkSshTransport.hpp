#ifndef PLINKSSHTRANSPORT_HPP
#define PLINKSSHTRANSPORT_HPP

#include "Transport.hpp"
#include "AbstractSshTransport.hpp"
#include <QProcess>

class PlinkSshTransport : public AbstractSshTransport
{
    Q_OBJECT

public:
	//! Create an empty PlinkSshTransport object
	PlinkSshTransport(QObject* parent=0);
	~PlinkSshTransport();
	virtual const QString getType() {return "PlinkSshTransport";}

	//! Test that the transport can connect to the remote host and echo "hello world"
	virtual bool testTransport();

public slots:
	//! Use this transport object to run a command
	//! \param exec the program path to be executed
	//! \args a list of arguments to pass to the program
	//! \returns false if there is some bad problem with this transport
	//! object, for example the configuration is not complete
	virtual bool start(const QString& exec, const QStringList& args=QStringList());

	//! Kill the current monitor if it is running
	virtual void stop();

	//! Save transport settings
	virtual const QString saveTransport();

	//! Dump debugging info
	virtual void dumpDebug();

private:
	//! accepts a reference to a process object and with that process
	//! launches plink with current host and authentication credentials.
	//! This function blocks until a connection has successfully been
	//! established, which means that any known hosts questions have
	//! been answered, password has been entered if required and so on.
	//! \returns the first non-authentication/non-error output (which might
	//!          be all of it).  Returns QString() on connection error.
	QString establishConnection(QProcess& proc, const QString& exe, const QStringList& args);

	//! get the path to the plink executable
	QString getPlinkExePath();
};

#endif // PLINKSSHTRANSPORT_HPP
