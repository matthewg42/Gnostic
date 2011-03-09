#include "DataDisplayConfigWidget.hpp"
#include "DataDisplay.hpp"

DataDisplayConfigWidget::DataDisplayConfigWidget(DataDisplay* t, QWidget *parent) :
    QWidget(parent)
{
	display = t;
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
