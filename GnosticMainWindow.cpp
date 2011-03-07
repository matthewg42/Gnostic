#include "GnosticMainWindow.hpp"
#include "ui_GnosticMainWindow.h"
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QMessageBox>

#include "LocalTransport.hpp"

GnosticMainWindow* GnosticMainWindow::singleton = NULL;

GnosticMainWindow::GnosticMainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GnosticMainWindow)
{
	Q_ASSERT(!singleton);
	singleton = this;
        ui->setupUi(this);

	transport = new LocalTransport(this);
	QWidget* ltw = transport->getConfigWidget();
        ui->transportSettingsLayout->addWidget(ltw);
        ltw->show();

	connect(ui->testButton, SIGNAL(clicked()), this, SLOT(doTest()));
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

void GnosticMainWindow::receiveData(QString label, double value, qint64 timeStamp)
{
	qDebug() << "GnosticMainWindow::receiveData got" << label << value << timeStamp;
}

void GnosticMainWindow::transportStatusChanged(Transport::TransportStatus newStatus)
{
	qDebug() << "GnosticMainWindow::transportStatusChanged newStatus:" << newStatus;
}

void GnosticMainWindow::doTest()
{
	qDebug() << "GnosticMainWindow::doTest doTest...";
	QMessageBox mb;
	if (!transport->testTransport())
		mb.setText("Transport test FAILED - check your settings!");
	else
		mb.setText("Transport test was SUCCESSFUL");

	mb.exec();
}
