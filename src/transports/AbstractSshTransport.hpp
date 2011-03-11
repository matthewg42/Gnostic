#ifndef ABSTRACTSSHTRANSPORT_HPP
#define ABSTRACTSSHTRANSPORT_HPP

#include "Transport.hpp"
#include <QProcess>

//! \class AbstractSshTransport
//! Because we want to support multiple ssh implementations (Putty's PLINK which is
//! easy to bundle for the win32 build, and the openssh client which is typically
//! shipped with Linux systems), this class handles those things the different Ssh
//! implementation have in common - authentication credentials and so on - and leaves
//! the specifics to derived classes.
//! See also: PlinkSshTransport and OpenSshTransport

class QWidget;
class TransportConfigWidget;

class AbstractSshTransport : public Transport
{
    Q_OBJECT

public:
	enum SshAuthType
	{
		Password=0,
		PublicKey
	};

	AbstractSshTransport(QObject* parent=0);
	~AbstractSshTransport() = 0;

	virtual TransportConfigWidget* getConfigWidget(QWidget* parent);

public slots:
	virtual const QString saveSettings();

	virtual bool loadSettings(const QString& section);

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

protected slots:
	virtual void procStatusUpdate(QProcess::ProcessState);
	virtual void procReadIn();
	virtual void procReadErr();
	virtual void procError(QProcess::ProcessError);
	virtual void procDone(int);

protected:
	QProcess proc;

};

#endif // ABSTRACTSSHTRANSPORT_HPP
