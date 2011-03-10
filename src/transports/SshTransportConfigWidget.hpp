#ifndef SSHTRANSPORTCONFIGWIDGET_HPP
#define SSHTRANSPORTCONFIGWIDGET_HPP


#include "TransportConfigWidget.hpp"

namespace Ui {
    class SshTransportConfigWidget;
}

class SshTransportConfigWidget : public TransportConfigWidget
{
    Q_OBJECT

public:
    explicit SshTransportConfigWidget(Transport* t, QWidget *parent = 0);
    ~SshTransportConfigWidget();

private:
    Ui::SshTransportConfigWidget *ui;
};

#endif // SSHTRANSPORTCONFIGWIDGET_HPP
