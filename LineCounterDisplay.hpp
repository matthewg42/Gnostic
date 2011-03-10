#ifndef COUNTERDISPLAY_HPP
#define COUNTERDISPLAY_HPP

#include "DataDisplay.hpp"
#include <QRegExp>

namespace Ui {
	class LineCounterDisplay;
}

class DataDisplayConfigWidget;

class LineCounterDisplay : public DataDisplay
{
	Q_OBJECT

public:
	LineCounterDisplay(QWidget *parent = 0);
	~LineCounterDisplay();
	virtual const QString getType() { return "LineCounterDisplay"; }
	virtual DataDisplay::DataTypes wantDataTypes() { return DataDisplay::Lines; }

	virtual DataDisplayConfigWidget* getConfigWidget(QWidget* parent);

public slots:
	virtual const QString& saveDataDisplay();
	virtual bool loadSettings(const QString& section);
	virtual void dumpDebug();

	void setFilter(const QString pattern);
	QString getFilter();
	void setDisplayLabel(bool b);
	bool getDisplayLabel();

	void takeLine(QString line);

private:
	Ui::LineCounterDisplay *ui;
	QRegExp filter;
	bool showLabel;
};

#endif // COUNTERDISPLAY_HPP
