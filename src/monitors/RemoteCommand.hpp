#ifndef REMOTECOMMAND_HPP
#define REMOTECOMMAND_HPP

#include <QString>
#include <QStringList>

class Transport;
class RemoteCommandConfigWidget;
class DataDisplay;

class RemoteCommand : public QObject
{
	Q_OBJECT

public:
	RemoteCommand(QObject* parent=0);
	~RemoteCommand();

	const QString saveSettings();
	bool loadSettings(const QString& section);


	//! If necessary instantiates, and returns a transport object
	//! returns NULL if transportId is not existing in the config file.
	Transport* getTransport();
	void setTransport(Transport* t);

	static RemoteCommand* makeFromConfig(const QString& section);
	static QStringList getSections();
	static const QString getNewId();

public slots:
	bool start();
	void stop();

	const QString& getId() { return id; }
	const QString& getDescription() { return description; }
	void setDescription(const QString& d) { description = d; }
	const QString& getTransportId() { return transportId; }
	bool setTransportId(const QString& id);
	const QString& getProgram() { return program; }
	void setProgram(const QString& p) { program=p; }
	const QStringList& getArguments() { return arguments; }
	void setArguments(const QStringList& a) { arguments = a; }
	int getTimeout() { return timeout; }
	void setTimeout(int t) { timeout = t; }
	bool getReconnect() { return reconnect; }
	void setReconnect(bool r) { reconnect = r; }
	int getRetries() { return retries; }
	void setRetries(int r) { retries = r; }

	void dumpDebug();

private:
	QString id;
	QString description;
	QString transportId;
	QString program;
	QStringList arguments;
	int timeout;
	bool reconnect;
	int retries;
	Transport* transport;

};

#endif // REMOTECOMMAND_HPP
