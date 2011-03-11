#ifndef TAILERDISPLAY_HPP
#define TAILERDISPLAY_HPP

#include "DataDisplay.hpp"
#include <QRegExp>
#include <QStringList>

namespace Ui {
	class TailerDisplay;
}

class DataDisplayConfigWidget;

class TailerDisplay : public DataDisplay
{
	Q_OBJECT

public:
	TailerDisplay(QWidget *parent = 0);
	~TailerDisplay();
	virtual const QString getType() { return "TailerDisplay"; }
	virtual DataDisplay::DataTypes wantDataTypes() { return DataDisplay::Lines; }
	virtual DataDisplayConfigWidget* getConfigWidget(QWidget* parent);

public slots:
	virtual const QString& saveSettings();
	virtual bool loadSettings(const QString& section);
	virtual void dumpDebug();

	void setHistory(int h);
	int getHistory();

	void takeLine(QString line);

protected slots:
	void redisplayBuffer();

private:
	Ui::TailerDisplay *ui;
	static const int defaultHistoryLines = 200;

protected:
	int history;
	QStringList buffer;

};

#endif // TAILERDISPLAY_HPP
