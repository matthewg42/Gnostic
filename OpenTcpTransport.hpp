#ifndef OPENTCPTRANSPORT_HPP
#define OPENTCPTRANSPORT_HPP

#include "Transport.hpp"

#include <QtNetwork/QTcpSocket>

class OpenTcpTransport : public Transport
{
    Q_OBJECT

public:
	OpenTcpTransport();

//signals:
//        void connectionStatusChanged(Transport::TransportStatus newStatus);

public slots:
	virtual void startConnection();
	virtual void stopConnection();
	void setPort(int newPort) {port = newPort;}
	int getPort() {return port;}

protected slots:
	void stateChanged(QAbstractSocket::SocketState);

protected:
        void callRemoteMonitor();

        int port;
	QTcpSocket socket;

};

#endif // OPENTCPTRANSPORT_HPP
