#include "CounterDisplay.hpp"
#include "ui_CounterDisplay.h"

#include "CounterDisplayConfigWidget.hpp"
#include "GnosticApp.hpp"

#include <QDebug>
#include <QSettings>

CounterDisplay::CounterDisplay(QWidget *parent) :
    DataDisplay(parent),
    ui(new Ui::CounterDisplay),
    filter(),
    showLabel(true)
{
    ui->setupUi(this);
    setDisplayLabel(showLabel);
}

CounterDisplay::~CounterDisplay()
{
    delete ui;
}

DataDisplayConfigWidget* CounterDisplay::getConfigWidget(QWidget* parent)
{
	if (configWidget==NULL)
		configWidget = new CounterDisplayConfigWidget(this, parent);

	return configWidget;
}

const QString& CounterDisplay::saveDataDisplay()
{
	qDebug() << "CounterDisplay::saveDataDisplay";
	DataDisplay::saveDataDisplay();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/filter").arg(id), getFilter());
	settings->setValue(QString("%1/display_label").arg(id), getDisplayLabel());
	return id;
}

bool CounterDisplay::loadSettings(const QString& section)
{
	qDebug() << "CounterDisplay::loadSettings" << section;
	if (!DataDisplay::loadSettings(section))
		return false;

	QSettings* settings = GnosticApp::getInstance().settings();
	setFilter(settings->value(QString("%1/filter").arg(section)).toString());
	setDisplayLabel(settings->value(QString("%1/display_label").arg(section)).toBool());

	return true;
}

void CounterDisplay::dumpDebug()
{
	qDebug() << "CounterDisplay::dumpDebug calling DataDisplay::dumpDebug()";
	qDebug() << "CounterDisplay::dumpDebug filter" << getFilter();
	qDebug() << "CounterDisplay::dumpDebug display label" << getDisplayLabel();
}

void CounterDisplay::setFilter(QString pattern)
{
	filter = QRegExp(pattern);
}

QString CounterDisplay::getFilter()
{
	return filter.pattern();
}

void CounterDisplay::setDisplayLabel(bool b)
{
	qDebug() << "CounterDisplay::setDisplayLabel with" << b;
	showLabel = b;
	ui->label->setVisible(showLabel);
}

bool CounterDisplay::getDisplayLabel()
{
	return showLabel;
}

