#include "SSHTransport.hpp"

SSHTransport::SSHTransport()
{
}

void SSHTransport::startConnection()
{
    setConnectionStatus(EstablishingConnection);
}

void SSHTransport::stopConnection()
{
    setConnectionStatus(Disconnected);
}

void SSHTransport::callRemoteMonitor()
{
    // execute that command
}
