#include "DataDisplayConfigWidget.hpp"
#include "DataDisplay.hpp"

DataDisplayConfigWidget::DataDisplayConfigWidget(DataDisplay* d, QWidget *parent) :
		QWidget(parent)
{
	display = d;
}

DataDisplayConfigWidget::~DataDisplayConfigWidget()
{
}

void DataDisplayConfigWidget::madeUpdate()
{
	emit(wasUpdated());
}

DataDisplay* DataDisplayConfigWidget::getDataDisplay()
{
	return display;
}
