#ifndef SSHTRANSPORT_HPP
#define SSHTRANSPORT_HPP

#include "Transport.hpp"
#include <QProcess>

class SshTransport : public Transport
{
    Q_OBJECT

public:
	enum SshAuthType
	{
		Password=0,
		PublicKey
	};

	//! Create an empty SshTransport object
	SshTransport(QObject* parent=0);
	//! Copy contructor, assignment
	SshTransport(SshTransport& other, QObject* parent=0);

	~SshTransport();

	virtual const QString getType() {return "SshTransport";}


	//! get a widget for configuring the transport.  should have
	//! save and cancel buttons and so on
	virtual TransportConfigWidget* getConfigWidget(QWidget* parent=0);

	//! Test that the transport can connect to the remote host and echo "hello world"
	virtual bool testTransport();

public slots:
	//! Use this transport object to run a command
	//! \param exec the program path to be executed
	//! \args a list of arguments to pass to the program
	//! \returns false if there is some bad problem with this transport
	//! object, for example the configuration is not complete
	virtual bool startMonitor(const QString& exec, const QStringList& args=QStringList());

	//! Kill the current monitor if it is running
	virtual void stopMonitor();

	//! Save transport settings
	virtual void saveTransport();

	//! Dump debugging info
	virtual void dumpDebug();

	// accessors
	const QString getUser();
	void setUser(const QString& u);
	const QString getHost();
	void setHost(const QString& h);
	const QString getKeyFilePath();
	void setKeyFilePath(const QString& k);
	SshAuthType getAuthType();
	void setAuthType(SshAuthType t);

protected:
	QString host;
	QString user;
	SshAuthType authType;
	QString keyFilePath;

private slots:
	void procStatusUpdate(QProcess::ProcessState);
	void procReadIn();
	void procReadErr();
	void procError(QProcess::ProcessError);
	void procDone(int);

private:
	void makeProcess();

	//! put all the connection stuff into plink args...
	const QStringList argsToCmd(const QStringList args);

private:
	QProcess* proc;
};

#endif // SSHTRANSPORT_HPP
