#include "GnosticMainWindow.hpp"
#include "ui_GnosticMainWindow.h"
#include <QDebug>
#include <QWidget>

#include "RemoteMonitor.hpp"
#include "Transport.hpp"
#include "DataDisplay.hpp"

GnosticMainWindow* GnosticMainWindow::singleton = NULL;

GnosticMainWindow::GnosticMainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GnosticMainWindow)
{
	Q_ASSERT(!singleton);
	singleton = this;
        ui->setupUi(this);

	RemoteMonitor* m = new RemoteMonitor();
//	m->setDescription("My test monitor");
//	m->addDisplay("display_0");
//	m->addRemoteCommand("remote_command_0");
//	m->saveSettings();

	m->loadSettings("monitor_0");
	if (m)
		m->start();

}

GnosticMainWindow::~GnosticMainWindow()
{
	delete ui;
}

GnosticMainWindow& GnosticMainWindow::getInstance()
{
	Q_ASSERT(singleton);
	return *singleton;
}


