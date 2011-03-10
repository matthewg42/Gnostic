#ifndef LOCALTRANSPORTCONFIGWIDGET_HPP
#define LOCALTRANSPORTCONFIGWIDGET_HPP

#include <QWidget>
#include "TransportConfigWidget.hpp"

namespace Ui {
    class LocalTransportConfigWidget;
}

class LocalTransportConfigWidget : public TransportConfigWidget
{
    Q_OBJECT

public:
    explicit LocalTransportConfigWidget(Transport* t, QWidget *parent = 0);
    ~LocalTransportConfigWidget();

private:
    Ui::LocalTransportConfigWidget *ui;
};

#endif // LOCALTRANSPORTCONFIGWIDGET_HPP
