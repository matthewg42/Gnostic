#ifndef COUNTERDISPLAY_HPP
#define COUNTERDISPLAY_HPP

#include "DataDisplay.hpp"
#include <QRegExp>

namespace Ui {
	class CounterDisplay;
}

class DataDisplayConfigWidget;

class CounterDisplay : public DataDisplay
{
	Q_OBJECT

public:
	explicit CounterDisplay(QWidget *parent = 0);
	~CounterDisplay();

	DataDisplayConfigWidget* getConfigWidget(QWidget* parent);

public slots:
	void setFilter(const QString pattern);
	QString getFilter();
	void setDisplayLabel(bool b);
	bool getDisplayLabel();

private:
	Ui::CounterDisplay *ui;
	QRegExp filter;
};

#endif // COUNTERDISPLAY_HPP
