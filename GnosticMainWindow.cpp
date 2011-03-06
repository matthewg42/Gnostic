#include "GnosticMainWindow.hpp"
#include "ui_GnosticMainWindow.h"
#include <QDebug>
#include <QTimer>

GnosticMainWindow* GnosticMainWindow::singleton = NULL;

GnosticMainWindow::GnosticMainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::GnosticMainWindow)
{
	ui->setupUi(this);
	Q_ASSERT(!singleton);
	singleton = this;
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
