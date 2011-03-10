#include "CounterDisplay.hpp"
#include "ui_CounterDisplay.h"

#include "CounterDisplayConfigWidget.hpp"

CounterDisplay::CounterDisplay(QWidget *parent) :
    DataDisplay(parent),
    ui(new Ui::CounterDisplay),
    filter()
{
    ui->setupUi(this);
    setDisplayLabel(true);
}

CounterDisplay::~CounterDisplay()
{
    delete ui;
}

DataDisplayConfigWidget* CounterDisplay::getConfigWidget(QWidget* parent)
{
	if (configWidget==NULL)
		configWidget = new CounterDisplayConfigWidget(this, parent);

	return configWidget;
}

void CounterDisplay::setFilter(QString pattern)
{
	filter = QRegExp(pattern);
}

QString CounterDisplay::getFilter()
{
	return filter.pattern();
}

void CounterDisplay::setDisplayLabel(bool b)
{
	ui->label->setVisible(b);
}

bool CounterDisplay::getDisplayLabel()
{
	return ui->label->isVisible();
}

