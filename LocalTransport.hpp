
#ifndef LOCALTRANSPORT_HPP
#define LOCALTRANSPORT_HPP

#include "Transport.hpp"
#include <QProcess>

class LocalTransport : public Transport
{
	Q_OBJECT

public:
	//! Create an empty LocalTransport object
	LocalTransport(QObject* parent=0);
	//! Copy contructor, assignment
	LocalTransport(LocalTransport& other, QObject* parent=0);

	~LocalTransport();

	//! get a widget for configuring the transport.  should have
	//! save and cancel buttons and so on
	virtual QWidget* getConfigWidget();

public slots:
	//! Use this transport object to run a command
	//! \param exec the program path to be executed
	//! \args a list of arguments to pass to the program
	//! \returns false if there is some bad problem with this transport
	//! object, for example the configuration is not complete
	virtual bool startMonitor(const QString& exec, const QStringList& args);

	//! Kill the current monitor if it is running
	virtual void stopMonitor();

	//! Save transport settings
	virtual void saveTransport();

private slots:
	void procStatusUpdate(QProcess::ProcessState);
	void procReadIn();
	void procReadErr();
	void procError(QProcess::ProcessError);
	void procDone(int);

private:
	void makeProcess();

private:
	QString shellPath;
	QProcess* proc;
};

#endif // LOCALTRANSPORT_HPP
