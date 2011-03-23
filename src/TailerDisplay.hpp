#ifndef TAILERDISPLAY_HPP
#define TAILERDISPLAY_HPP

#include "DataDisplay.hpp"
#include <QRegExp>
#include <QStringList>
#include <QMap>
#include <QString>
#include <QColor>

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

	void addHighlight(const QString& pattern, const QColor color);
	void deleteHighlight(const QString& pattern);
	QMap< QString, QColor > getHighlights() { return highlights; }

	void setHistory(int h);
	int getHistory();

	void takeLine(QString line);

private:
	Ui::TailerDisplay *ui;
	static const int defaultHistoryLines = 100;
	QMap< QString, QColor > highlights;

};

#endif // TAILERDISPLAY_HPP
