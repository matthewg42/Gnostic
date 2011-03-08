#ifndef LOCALTRANSPORTCONFIGWIDGET_HPP
#define LOCALTRANSPORTCONFIGWIDGET_HPP

#include "TransportConfigWidget.hpp"

namespace Ui
{
	class LocalTransportConfigWidget;
}

class LocalTransport;
class QWidget;

class LocalTransportConfigWidget : public TransportConfigWidget
{
	Q_OBJECT

public:
	explicit LocalTransportConfigWidget(LocalTransport* lt, QWidget* parent=0);
	~LocalTransportConfigWidget();

private:
	Ui::LocalTransportConfigWidget *ui;
};

#endif // LOCALTRANSPORTCONFIGWIDGET_HPP
