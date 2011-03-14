#include "TimeGraphDisplayConfigWidget.hpp"
#include "ui_TimeGraphDisplayConfigWidget.h"
#include "TimeGraphDisplay.hpp"

TimeGraphDisplayConfigWidget::TimeGraphDisplayConfigWidget(DataDisplay* d, QWidget *parent) :
		DataDisplayConfigWidget(d, parent),
		ui(new Ui::TimeGraphDisplayConfigWidget)
{
	ui->setupUi(this);
	TimeGraphDisplay* td = static_cast<TimeGraphDisplay*>(d);

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), td, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), td, SLOT(setHistory(int)));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));
	connect(ui->updateMsSpin, SIGNAL(valueChanged(int)), td, SLOT(setUpdateMs(int)));
	connect(ui->updateMsSpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));
	connect(ui->manualYScaleGroup, SIGNAL(toggled(bool)), td, SLOT(setYManualScale(bool)));
	connect(ui->manualYScaleGroup, SIGNAL(toggled(bool)), this, SLOT(madeUpdate()));
	connect(ui->yMinSpin, SIGNAL(valueChanged(double)), td, SLOT(setYMin(double)));
	connect(ui->yMinSpin, SIGNAL(valueChanged(double)), this, SLOT(madeUpdate()));
	connect(ui->yMaxSpin, SIGNAL(valueChanged(double)), td, SLOT(setYMax(double)));
	connect(ui->yMaxSpin, SIGNAL(valueChanged(double)), this, SLOT(madeUpdate()));

	ui->descriptionEdit->setText(td->getDescription());
	ui->historySpin->setValue(td->getHistory());
	ui->updateMsSpin->setValue(td->getUpdateMs());
	ui->manualYScaleGroup->setChecked(td->getYManualScale());
	ui->yMinSpin->setValue(td->getYMin());
	ui->yMaxSpin->setValue(td->getYMax());

}

TimeGraphDisplayConfigWidget::~TimeGraphDisplayConfigWidget()
{
	delete ui;
}
