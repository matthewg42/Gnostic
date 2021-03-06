
#ifndef DUMMYTRANSPORT_HPP
#define DUMMYTRANSPORT_HPP

#include "Transport.hpp"
#include <QTimer>
#include <QMap>
#include <QString>

class TransportConfigWidget;
class QWidget;

//! \class DummyTransport
//! A DummyTransport is used when testing DataDisplays...  it doesn't connect to anything
//! or spawn any child processes... it just spits out random data which is suitable for
//! connecting to a GnosticParser and then testing a DataDisplay.
class DummyTransport : public Transport
{
	Q_OBJECT

public:
	//! Create an empty DummyTransport object
	DummyTransport(QObject* parent=0);
	~DummyTransport();
	virtual const QString getType() {return "DummyTransport";}

	//! A Dummy is always working... returns true;
	virtual bool test();

	//! Always returns NULL
	virtual TransportConfigWidget* getConfigWidget(QWidget* parent=0);
	virtual RemoteCommandConfigWidget* getCommandWidget(QWidget* parent=0);

public slots:
	//! A DummyTransport calls no programs.. the arguments are ignored.
	virtual bool start(const QString& exec, const QStringList& args=QStringList());

	//! Stop sending stuff please
	virtual void stop();

	//! Doesn't do anything
	virtual const QString saveSettings();

	//! Doesn't do anything
	virtual bool loadSettings(const QString& section);

	//! Show how much of a Dummy we are.
	virtual void dumpDebug();

protected slots:
	void spewHeader();
	void spew();

protected:
	QTimer timer;
	QMap<QString, double> lastVal;
};

#endif // DUMMYTRANSPORT_HPP
