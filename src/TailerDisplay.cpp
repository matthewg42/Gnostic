#include "TailerDisplay.hpp"
#include "ui_TailerDisplay.h"

#include "TailerDisplayConfigWidget.hpp"
#include "GnosticApp.hpp"

#include <QDebug>
#include <QSettings>
#include <QScrollBar>

TailerDisplay::TailerDisplay(QWidget *parent) :
		DataDisplay(parent),
		ui(new Ui::TailerDisplay)
{
	ui->setupUi(this);
	setHistory(defaultHistoryLines);
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
	qDebug() << "TailerDisplay::saveSettings";
	DataDisplay::saveSettings();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/history").arg(id), getHistory());

	int n=0;
	foreach(QString pattern, highlights.keys())
	{
		QColor c = highlights[pattern];
		QString val = QString("%1:%2:%3:%4:%5")
			      .arg(c.red())
			      .arg(c.green())
			      .arg(c.blue())
			      .arg(c.alpha())
			      .arg(pattern);
		settings->setValue(QString("%1/highlight_%2").arg(id).arg(n), val);
		n++;
	}

        GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Display);
	return id;
}

bool TailerDisplay::loadSettings(const QString& section)
{
	//qDebug() << "TailerDisplay::loadSettings" << section;
	if (!DataDisplay::loadSettings(section))
		return false;

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->beginGroup(section);
	setHistory(settings->value("history", defaultHistoryLines).toInt());

	highlights.clear();
	foreach(QString key, settings->childKeys())
	{
		if (key.startsWith("highlight_"))
		{
			QRegExp rx("^(\\d+):(\\d+):(\\d+):(\\d+):(.*)$");
			if (rx.exactMatch(settings->value(key).toString()))
			{
				QColor c(QVariant(rx.capturedTexts().at(1)).toInt(),
					 QVariant(rx.capturedTexts().at(2)).toInt(),
					 QVariant(rx.capturedTexts().at(3)).toInt(),
					 QVariant(rx.capturedTexts().at(4)).toInt());
				addHighlight(rx.capturedTexts().at(5), c);
			}
		}
	}
	settings->endGroup();

	return true;
}

void TailerDisplay::dumpDebug()
{
	qDebug() << "TailerDisplay::dumpDebug calling DataDisplay::dumpDebug()";
	qDebug() << "TailerDisplay::dumpDebug history" << getHistory();
	foreach(QString pattern, highlights.keys())
	{
		qDebug() << "TailerDisplay::dumpDebug pattern" << pattern << highlights[pattern];
	}
}

void TailerDisplay::addHighlight(const QString& pattern, const QColor color)
{
	highlights.insert(pattern, color);
}

void TailerDisplay::deleteHighlight(const QString& pattern)
{
	highlights.remove(pattern);
}

void TailerDisplay::setHistory(int h)
{
	ui->textEdit->document()->setMaximumBlockCount(h);
}

int TailerDisplay::getHistory()
{
	return ui->textEdit->document()->maximumBlockCount();
}

void TailerDisplay::takeLine(QString line)
{
	QTextCharFormat fmt = ui->textEdit->currentCharFormat();
	fmt.setForeground(QBrush(Qt::black));
	foreach (QString pattern, highlights.keys())
	{
		if (line.contains(QRegExp(pattern)))
		{
			fmt.setForeground(QBrush(highlights[pattern]));
			break;
		}
	}
	ui->textEdit->setCurrentCharFormat(fmt);
	ui->textEdit->append(line);
}

