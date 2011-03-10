#include "LineCounterDisplay.hpp"
#include "ui_LineCounterDisplay.h"

#include "LineCounterDisplayConfigWidget.hpp"
#include "GnosticApp.hpp"

#include <QDebug>
#include <QSettings>

LineCounterDisplay::LineCounterDisplay(QWidget *parent) :
    DataDisplay(parent),
    ui(new Ui::LineCounterDisplay),
    filter(),
    showLabel(true)
{
    ui->setupUi(this);
    setDisplayLabel(showLabel);
}

LineCounterDisplay::~LineCounterDisplay()
{
    delete ui;
}

DataDisplayConfigWidget* LineCounterDisplay::getConfigWidget(QWidget* parent)
{
	if (configWidget==NULL)
		configWidget = new LineCounterDisplayConfigWidget(this, parent);

	return configWidget;
}

const QString& LineCounterDisplay::saveDataDisplay()
{
	qDebug() << "LineCounterDisplay::saveDataDisplay";
	DataDisplay::saveDataDisplay();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/filter").arg(id), getFilter());
	settings->setValue(QString("%1/display_label").arg(id), getDisplayLabel());
	return id;
}

bool LineCounterDisplay::loadSettings(const QString& section)
{
	qDebug() << "LineCounterDisplay::loadSettings" << section;
	if (!DataDisplay::loadSettings(section))
		return false;

	QSettings* settings = GnosticApp::getInstance().settings();
	setFilter(settings->value(QString("%1/filter").arg(section)).toString());
	setDisplayLabel(settings->value(QString("%1/display_label").arg(section)).toBool());

	return true;
}

void LineCounterDisplay::dumpDebug()
{
	qDebug() << "LineCounterDisplay::dumpDebug calling DataDisplay::dumpDebug()";
	qDebug() << "LineCounterDisplay::dumpDebug filter" << getFilter();
	qDebug() << "LineCounterDisplay::dumpDebug display label" << getDisplayLabel();
}

void LineCounterDisplay::setFilter(QString pattern)
{
	filter = QRegExp(pattern);
}

QString LineCounterDisplay::getFilter()
{
	return filter.pattern();
}

void LineCounterDisplay::setDisplayLabel(bool b)
{
	qDebug() << "LineCounterDisplay::setDisplayLabel with" << b;
	showLabel = b;
	ui->label->setVisible(showLabel);
}

bool LineCounterDisplay::getDisplayLabel()
{
	return showLabel;
}

void LineCounterDisplay::takeLine(QString line)
{
	if (filter.pattern().isEmpty() || filter.exactMatch(line))
	{
		ui->lineCountLcd->display(ui->lineCountLcd->value()+1);
		return;
	}
}
