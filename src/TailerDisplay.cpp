#include "TailerDisplay.hpp"
#include "ui_TailerDisplay.h"

#include "TailerDisplayConfigWidget.hpp"
#include "GnosticApp.hpp"

#include <QDebug>
#include <QSettings>
#include <QScrollBar>

TailerDisplay::TailerDisplay(QWidget *parent) :
		DataDisplay(parent),
		ui(new Ui::TailerDisplay),
		history(defaultHistoryLines)
{
	ui->setupUi(this);
}

TailerDisplay::~TailerDisplay()
{
	delete ui;
}

DataDisplayConfigWidget* TailerDisplay::getConfigWidget(QWidget* parent)
{
	if (configWidget==NULL)
		configWidget = new TailerDisplayConfigWidget(this, parent);

	return configWidget;
}

const QString& TailerDisplay::saveSettings()
{
	//qDebug() << "TailerDisplay::saveSettings";
	DataDisplay::saveSettings();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/history").arg(id), getHistory());
        GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Display);
	return id;
}

bool TailerDisplay::loadSettings(const QString& section)
{
	//qDebug() << "TailerDisplay::loadSettings" << section;
	if (!DataDisplay::loadSettings(section))
		return false;

	QSettings* settings = GnosticApp::getInstance().settings();
	setHistory(settings->value(QString("%1/history").arg(section), defaultHistoryLines).toInt());

	return true;
}

void TailerDisplay::dumpDebug()
{
	//qDebug() << "TailerDisplay::dumpDebug calling DataDisplay::dumpDebug()";
	//qDebug() << "TailerDisplay::dumpDebug history" << getHistory();
	//qDebug() << "TailerDisplay::dumpDebug currently" << buffer.count() << "lines in buffer";
}

void TailerDisplay::setHistory(int h)
{
	history = h;
	redisplayBuffer();
}

int TailerDisplay::getHistory()
{
	return history;
}

void TailerDisplay::takeLine(QString line)
{
	buffer << line;
	redisplayBuffer();
}

void TailerDisplay::redisplayBuffer()
{
	// First, determine if the display was "at the end"
	QScrollBar *sb = ui->textEdit->verticalScrollBar();
	bool atEnd = true;
        int oldValue = 0;
	if (sb)
	{
		oldValue = sb->value();
		atEnd = (sb->value() == sb->maximum());
	}

	while (buffer.count() > history)
		buffer.pop_front();

	ui->textEdit->setPlainText(buffer.join("\n"));
	sb = ui->textEdit->verticalScrollBar();
	if (sb)
	{
		if (atEnd)
			sb->setValue(sb->maximum());
		else
			sb->setValue(oldValue);
	}
}
