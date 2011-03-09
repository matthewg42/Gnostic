#include "GnosticMainWindow.hpp"
#include "ui_GnosticMainWindow.h"
#include <QDebug>
#include <QWidget>

#include "LocalTransport.hpp"
#include "OpenSshTransport.hpp"

GnosticMainWindow* GnosticMainWindow::singleton = NULL;

GnosticMainWindow::GnosticMainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GnosticMainWindow)
{
	Q_ASSERT(!singleton);
	singleton = this;
        ui->setupUi(this);

	OpenSshTransport* t = new OpenSshTransport(this);
	t->setDescription("a test Open SSH transport object");
	t->setHost("localhost");
	t->setUser("work");

	connect(t, SIGNAL(receivedLine(QString)), this, SLOT(gotLineOfInput(QString)));
	t->start("./mon.sh");
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

void GnosticMainWindow::gotLineOfInput(QString line)
{
	qDebug() << "GnosticMainWindow::gotLineOfInput" << line;
	ui->textBrowser->append(line);
}

