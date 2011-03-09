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

bool TransportConfigWidget::testTransport()
{
	if (transport==NULL)
		return false;
	else
		return transport->testTransport();
}

Transport* TransportConfigWidget::getTransport()
{
	return transport;
}