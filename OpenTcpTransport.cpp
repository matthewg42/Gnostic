#include <QtNetwork/QTcpSocket>
#include "OpenTcpTransport.hpp"
#include <QDebug>

OpenTcpTransport::OpenTcpTransport()
    : port(30585), socket(this)
{
    connect(&socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
}

void OpenTcpTransport::startConnection()
{
    if (connectionStatus == Connected || connectionStatus == EstablishingConnection)
    {
        qDebug() << "OpenTcpTransport::startConnection already connected or establishing connection";
        return;
    }

    emit(connectionStatusChanged(EstablishingConnection));
    socket.connectToHost(host, port);
}

void OpenTcpTransport::stopConnection()
{
    qDebug() << "OpenTcpTransport::stopConnection";
    socket.disconnect();
}

void OpenTcpTransport::stateChanged(QAbstractSocket::SocketState s)
{
    qDebug() << "OpenTcpTransport::stateChanged" << s;
    switch (s)
    {
    case QAbstractSocket::UnconnectedState:
        setConnectionStatus(Disconnected);
        break;
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
        setConnectionStatus(EstablishingConnection);
    case QAbstractSocket::ConnectedState:
        setConnectionStatus(Connected);
    case QAbstractSocket::ClosingState:
    default:
        setConnectionStatus(Disconnected);
    }
}

void OpenTcpTransport::callRemoteMonitor()
{
    qDebug() << "OpenTcpTransport::callRemoteMonitor not implemented yet: TODO";
}
