#include "CounterDisplayConfigWidget.hpp"
#include "ui_CounterDisplayConfigWidget.h"
#include "CounterDisplay.hpp"

CounterDisplayConfigWidget::CounterDisplayConfigWidget(DataDisplay* d, QWidget *parent) :
		DataDisplayConfigWidget(d, parent),
		ui(new Ui::CounterDisplayConfigWidget)
{
	ui->setupUi(this);
	CounterDisplay* cd = static_cast<CounterDisplay*>(d);
	ui->descriptionEdit->setText(cd->getDescription());
	ui->filterEdit->setText(cd->getFilter());
	ui->displayLabelCheck->setChecked(cd->getDisplayLabel());

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), cd, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->filterEdit, SIGNAL(textEdited(QString)), cd, SLOT(setFilter(QString)));
	connect(ui->filterEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
}

CounterDisplayConfigWidget::~CounterDisplayConfigWidget()
{
	delete ui;
}
