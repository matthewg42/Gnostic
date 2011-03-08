#include "TransportConfigWidget.hpp"
#include <QDebug>

TransportConfigWidget::TransportConfigWidget(QWidget *parent) :
		QWidget(parent)
{
}

TransportConfigWidget::~TransportConfigWidget()
{
}

void TransportConfigWidget::emitChanged()
{
	qDebug() << "TransportConfigWidget::emitChanged";
	emit(configChanged());
}
