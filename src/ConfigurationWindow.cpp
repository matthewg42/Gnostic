#include "ConfigurationWindow.hpp"
#include "ui_ConfigurationWindow.h"

#include "GnosticApp.hpp"

#include <QSettings>

ConfigurationWindow::ConfigurationWindow(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::ConfigurationWindow)
{
	ui->setupUi(this);

        if (GnosticApp::getInstance().settings()->value("paths/plink", "").toString().isEmpty() ||
            GnosticApp::getInstance().settings()->value("paths/ssh", "").toString().isEmpty() &&
            GnosticApp::getInstance().settings()->value("paths/ssh_askpass", "").toString().isEmpty())
        {
                ui->listWidget->setCurrentRow(3);
                ui->stackedWidget->setCurrentIndex(3);
        }
        else
        {
                ui->listWidget->setCurrentRow(0);
                ui->stackedWidget->setCurrentIndex(0);
        }
}

ConfigurationWindow::~ConfigurationWindow()
{
	delete ui;
}
