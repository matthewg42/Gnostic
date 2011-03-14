#include "TransportConfigWidget.hpp"
#include "Transport.hpp"

TransportConfigWidget::TransportConfigWidget(Transport* t, QWidget *parent) :
    QWidget(parent)
{
	transport = t;
}

TransportConfigWidget::~TransportConfigWidget()
{
}

bool TransportConfigWidget::test()
{
	if (transport==NULL)
		return false;
	else
		return transport->test();
}

void TransportConfigWidget::madeUpdate()
{
	emit(wasUpdated());
}

Transport* TransportConfigWidget::getTransport()
{
	return transport;
}
