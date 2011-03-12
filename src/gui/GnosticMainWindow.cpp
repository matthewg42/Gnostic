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

//	Transport* t = Transport::makeFromConfig("transport_0");
//	DataDisplay* d = DataDisplay::makeFromConfig("display_2");
//	Q_ASSERT(t);
//	Q_ASSERT(d);
//	RemoteMonitor* m = RemoteMonitor::quickstart(t, "/home/mouse/mon.sh", QStringList(), d);
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


