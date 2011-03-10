
#ifndef LOCALTRANSPORT_HPP
#define LOCALTRANSPORT_HPP

#include "Transport.hpp"
#include <QTimer>

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
	virtual bool testTransport();

	//! Always returns NULL
	virtual TransportConfigWidget* getConfigWidget(QWidget* parent);

public slots:
	//! A DummyTransport calls no programs.. the arguments are ignored.
	virtual bool start(const QString& exec, const QStringList& args=QStringList());

	//! Stop sending stuff please
	virtual void stop();

	//! Doesn't do anything
	virtual const QString saveTransport();

	//! Doesn't do anything
	virtual bool loadSettings(const QString& section);

	//! Show how much of a Dummy we are.
	virtual void dumpDebug();

protected slots:
	void spewHeader();
	void spew();

protected:
	QTimer timer;
};

#endif // LOCALTRANSPORT_HPP
