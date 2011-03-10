#include "TailerDisplayConfigWidget.hpp"
#include "ui_TailerDisplayConfigWidget.h"
#include "TailerDisplay.hpp"

TailerDisplayConfigWidget::TailerDisplayConfigWidget(DataDisplay* d, QWidget *parent) :
		DataDisplayConfigWidget(d, parent),
		ui(new Ui::TailerDisplayConfigWidget)
{
	ui->setupUi(this);
	TailerDisplay* td = static_cast<TailerDisplay*>(d);
	ui->descriptionEdit->setText(td->getDescription());
	ui->historySpin->setValue(td->getHistory());

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), td, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), td, SLOT(setHistory(int)));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));
}

TailerDisplayConfigWidget::~TailerDisplayConfigWidget()
{
	delete ui;
}
