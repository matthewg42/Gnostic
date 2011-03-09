#include "GnosticMainWindow.hpp"
#include "ui_GnosticMainWindow.h"
#include <QDebug>
#include <QWidget>

#include "LocalTransport.hpp"
#include "GnosticParser.hpp"

GnosticMainWindow* GnosticMainWindow::singleton = NULL;

GnosticMainWindow::GnosticMainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GnosticMainWindow)
{
	Q_ASSERT(!singleton);
	singleton = this;
        ui->setupUi(this);

	Transport* t = Transport::loadTransport("transport_0", this);
	if (t)
	{
		GnosticParser* p = new GnosticParser(this);
		connect(t, SIGNAL(spewLine(QString)), p, SLOT(takeLine(QString)));
		t->start("/home/mouse/mon.sh");
	}
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

