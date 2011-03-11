
#ifndef LOCALTRANSPORT_HPP
#define LOCALTRANSPORT_HPP

#include "Transport.hpp"
#include <QProcess>

class TransportConfigWidget;
class QWidget;

class LocalTransport : public Transport
{
	Q_OBJECT

public:
	//! Create an empty LocalTransport object
	LocalTransport(QObject* parent=0);
	~LocalTransport();
	virtual const QString getType() {return "LocalTransport";}

	//! Test that the transport invoke echo "hello world"
	virtual bool test();

	//! Get a config widget
	virtual TransportConfigWidget* getConfigWidget(QWidget* parent);

public slots:
	//! Use this transport object to run a command
	//! \param exec the program path to be executed
	//! \args a list of arguments to pass to the program
	//! \returns false if there is some bad problem with this transport
	//! object, for example the configuration is not complete
	virtual bool start(const QString& exec, const QStringList& args=QStringList());

	//! Kill the current monitor if it is running
	virtual void stop();

	virtual const QString saveSettings();

	virtual bool loadSettings(const QString& section);

	//! Dump debugging info
	virtual void dumpDebug();

protected slots:
	void procStatusUpdate(QProcess::ProcessState);
	void procReadIn();
	void procReadErr();
	void procError(QProcess::ProcessError);
	void procDone(int);

protected:
	QProcess proc;
};

#endif // LOCALTRANSPORT_HPP
