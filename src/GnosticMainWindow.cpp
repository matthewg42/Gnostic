#include "GnosticMainWindow.hpp"
#include "ui_GnosticMainWindow.h"

#include "RemoteMonitor.hpp"
#include "DataDisplay.hpp"

#include <QDebug>
#include <QWidget>
#include <QTableWidgetItem>

GnosticMainWindow* GnosticMainWindow::singleton = NULL;

GnosticMainWindow::GnosticMainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GnosticMainWindow)
{
	Q_ASSERT(!singleton);
	singleton = this;
        ui->setupUi(this);
	configWindow = new ConfigurationWindow();
        aboutWindow = new About();

        ui->actionShow_monitors->setChecked(ui->monitorWidget->isVisible());
        connect(ui->actionConfigure, SIGNAL(triggered()), configWindow, SLOT(show()));
        connect(ui->actionAbout, SIGNAL(triggered()), aboutWindow, SLOT(show()));
        connect(ui->actionFullscreen, SIGNAL(toggled(bool)), this, SLOT(setFullscreen(bool)));
	connect(ui->actionShow_monitors, SIGNAL(toggled(bool)), this, SLOT(showMonitors(bool)));
	connect(ui->monitorWidget, SIGNAL(visibilityChanged(bool)), ui->actionShow_monitors, SLOT(setChecked(bool)));
	connect(ui->monitorTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(monitorTableLaunch(QModelIndex)));
        connect(ui->configButton, SIGNAL(clicked()), configWindow, SLOT(show()));

	refreshMonitorList();
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

void GnosticMainWindow::setFullscreen(bool b)
{
	if (b)
		showFullScreen();
	else
		showNormal();
}

void GnosticMainWindow::showMonitors(bool b)
{
	ui->monitorWidget->setVisible(b);
}

void GnosticMainWindow::refreshMonitorList()
{

	ui->monitorTable->clear();
	ui->monitorTable->setColumnCount(2);
	ui->monitorTable->hideColumn(0);
	ui->monitorTable->horizontalHeader()->setStretchLastSection(true);

	foreach(QString section, RemoteMonitor::getSections())
	{
		RemoteMonitor* m = RemoteMonitor::makeFromConfig(section);
		if (m)
		{
			QTableWidgetItem *sitem = new QTableWidgetItem(section);
			QTableWidgetItem *ditem = new QTableWidgetItem(m->getDescription());

			int r = ui->monitorTable->rowCount();
			ui->monitorTable->insertRow(r);
			ui->monitorTable->setItem(r, 0, sitem);
			ui->monitorTable->setItem(r, 1, ditem);
			delete m;
		}
	}

}

void GnosticMainWindow::monitorTableLaunch(QModelIndex idx)
{

	QString section = ui->monitorTable->item(idx.row(), 0)->data(Qt::DisplayRole).toString();
	RemoteMonitor *m = RemoteMonitor::makeFromConfig(section);
	if (m)
	{
		foreach (DataDisplay* d, m->getDisplays())
		{
			Q_ASSERT(d);
			ui->monitorLayout->addWidget(static_cast<QWidget*>(d));
		}

		m->start();
	}
}

