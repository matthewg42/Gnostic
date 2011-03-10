#include "LineCounterDisplayConfigWidget.hpp"
#include "ui_LineCounterDisplayConfigWidget.h"
#include "LineCounterDisplay.hpp"

LineCounterDisplayConfigWidget::LineCounterDisplayConfigWidget(DataDisplay* d, QWidget *parent) :
		DataDisplayConfigWidget(d, parent),
		ui(new Ui::LineCounterDisplayConfigWidget)
{
	ui->setupUi(this);
	LineCounterDisplay* cd = static_cast<LineCounterDisplay*>(d);
	ui->descriptionEdit->setText(cd->getDescription());
	ui->filterEdit->setText(cd->getFilter());
	ui->displayLabelCheck->setChecked(cd->getDisplayLabel());

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), cd, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->filterEdit, SIGNAL(textEdited(QString)), cd, SLOT(setFilter(QString)));
	connect(ui->filterEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->displayLabelCheck, SIGNAL(toggled(bool)), cd, SLOT(setDisplayLabel(bool)));
	connect(ui->displayLabelCheck, SIGNAL(toggled(bool)), this, SLOT(madeUpdate()));
}

LineCounterDisplayConfigWidget::~LineCounterDisplayConfigWidget()
{
	delete ui;
}
