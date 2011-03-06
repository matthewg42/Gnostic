#ifndef SSHTRANSPORT_HPP
#define SSHTRANSPORT_HPP

#include "Transport.hpp"

class SSHTransport : public Transport
{
    Q_OBJECT

public:
    SSHTransport();

public slots:
    void startConnection();
    void stopConnection();
    void setPort(int newPort) {port = newPort;}
    int getPort() {return port;}
    void setUser(QString newUser) {user = newUser;}
    QString getUser() {return user;}
    void setPass(QString newPass) {user = newPass;}

protected:
    void callRemoteMonitor();

    int port;
    QString user;
    QString pass;

};

#endif // SSHTRANSPORT_HPP
