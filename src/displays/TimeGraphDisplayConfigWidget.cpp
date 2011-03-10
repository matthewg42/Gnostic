#include "TimeGraphDisplayConfigWidget.hpp"
#include "ui_TimeGraphDisplayConfigWidget.h"
#include "TimeGraphDisplay.hpp"

TimeGraphDisplayConfigWidget::TimeGraphDisplayConfigWidget(DataDisplay* d, QWidget *parent) :
		DataDisplayConfigWidget(d, parent),
		ui(new Ui::TimeGraphDisplayConfigWidget)
{
	ui->setupUi(this);
	TimeGraphDisplay* td = static_cast<TimeGraphDisplay*>(d);
	ui->descriptionEdit->setText(td->getDescription());
	ui->historySpin->setValue(td->getHistory());

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), td, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), td, SLOT(setHistory(int)));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));
}

TimeGraphDisplayConfigWidget::~TimeGraphDisplayConfigWidget()
{
	delete ui;
}
