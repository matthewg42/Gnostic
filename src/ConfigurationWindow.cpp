#include "ConfigurationWindow.hpp"
#include "ui_ConfigurationWindow.h"

ConfigurationWindow::ConfigurationWindow(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::ConfigurationWindow)
{
	ui->setupUi(this);
	ui->listWidget->setCurrentRow(0);
	ui->stackedWidget->setCurrentIndex(0);
}

ConfigurationWindow::~ConfigurationWindow()
{
	delete ui;
}
