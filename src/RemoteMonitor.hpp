#ifndef REMOTEMONITOR_HPP
#define REMOTEMONITOR_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QSet>

class DataDisplay;
class Transport;
class GnosticParser;
class RemoteCommand;

//! \class RemoteMonitor
//! Models multiple RemoteCommands, displaying on Multiple DataDisplays.
//! All traffic goes through a single GnosticParser object...  all displays
//! see data from all RemoteCommands.
//
// [ Transport + Command ] ---\                     /---> [ DataDisplay ]
// [ Transport + Command ] -----> [ GnosticParser ] ----> [ DataDisplay ]
// [ Transport + Command ] ---/                     \---> [ DataDisplay ]
class RemoteMonitor : public QObject
{
	Q_OBJECT
public:
	explicit RemoteMonitor(QObject *parent = 0);
	virtual ~RemoteMonitor();
	virtual const QString getType() { return "RemoteMonitor"; }
	const QString& getId();
	const QString& getDescription();
	// virtual RemoteMonitorConfigWidget* getConfigWidget(QWidget* parent=0);

	const QSet< RemoteCommand* > getCommands() { return commands; }
	const QList< DataDisplay* > getDisplays() { return displays; }


	// Factory members
	static QStringList getAvailableTypes();
	static RemoteMonitor* makeNew(const QString& type, QObject* parent=0);
	static RemoteMonitor* makeFromConfig(const QString& section, QObject* parent=0);
	static const QString getNewId();
	static bool erase(const QString& section);
	static QStringList getSections();
	static int removeForCommand(const QString& commandId);

	static RemoteMonitor* quickstart(Transport* transport,
					 const QString& program,
					 const QStringList& args,
					 DataDisplay* display,
					 QObject* parent=0);


public slots:
	virtual bool start();
	virtual void stop();
	void clearDisplays();
	void clearCommands();
	void setDescription(const QString& newDescription);
	void addDisplay(DataDisplay* d);
	void addDisplay(const QString& section);
	void addRemoteCommand(RemoteCommand* c);
	void addRemoteCommand(const QString& section);
	virtual const QString saveSettings();
	virtual bool loadSettings(const QString& section);
	virtual void dumpDebug();

protected:
	QString id;
	QString description;
	GnosticParser* parser;
	QSet< RemoteCommand* > commands;
	QList< DataDisplay* > displays;

};

#endif // REMOTEMONITOR_HPP
