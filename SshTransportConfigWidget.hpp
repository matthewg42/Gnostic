#ifndef SSHTRANSPORTCONFIGWIDGET_HPP
#define SSHTRANSPORTCONFIGWIDGET_HPP

#include "TransportConfigWidget.hpp"

namespace Ui {
    class SshTransportConfigWidget;
}

class SshTransport;
class QWidget;

class SshTransportConfigWidget : public TransportConfigWidget
{
    Q_OBJECT

public:
    SshTransportConfigWidget(SshTransport* st, QWidget *parent=0);
    ~SshTransportConfigWidget();

private slots:
    void authTypeChanged();
    void chooseKeyPath();

private:
    Ui::SshTransportConfigWidget *ui;
    SshTransport* sshTransport;
};

#endif // SSHTRANSPORTCONFIGWIDGET_HPP
