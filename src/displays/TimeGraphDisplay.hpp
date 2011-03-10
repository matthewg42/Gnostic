#ifndef TIMEGRAPHDISPLAY_HPP
#define TIMEGRAPHDISPLAY_HPP


#include "DataDisplay.hpp"
#include <QRegExp>
#include <QStringList>
#include <QString>
#include <QVector>
#include <QMap>
#include <QProcess>
#include <QPen>

namespace Ui {
	class TimeGraphDisplay;
}

class QwtPlot;
class QwtPlotCurve;
class QwtPlotItem;
class QCloseEvent;
class QResizeEvent;
class QSettings;
class DataDisplayConfigWidget;
class PenStyleWidget;

class TimeGraphDisplay : public DataDisplay
{
	Q_OBJECT

public:
	TimeGraphDisplay(QWidget *parent = 0);
	~TimeGraphDisplay();
	virtual const QString getType() { return "TimeGraphDisplay"; }
	virtual DataDisplay::DataTypes wantDataTypes() { return DataDisplay::DataItems; }
	virtual DataDisplayConfigWidget* getConfigWidget(QWidget* parent);

public slots:
	virtual const QString& saveDataDisplay();
	virtual bool loadSettings(const QString& section);
	virtual void dumpDebug();

	void setHistory(int h);
	int getHistory();
	bool getYAutoScale();
	void setYAutoScale(bool b);
	bool getYManualScale();
	void setYManualScale(bool b);
	void setYMin(double d);
	double getYMin();
	void setYMax(double d);
	double getYMax();
	void setUpdateMs(int ms);
	int getUpdateMs();

	void takeDataItem(const double timestamp, const double value, const QString label);

protected slots:

	void setXScale();
	void setYScale();
	void setPlotItemStyle(QwtPlotItem*);
	void setNewPen(QPen p);

private:
	Ui::TimeGraphDisplay *ui;
	static const int defaultHistoryLines = 200;

protected:
	QPair< QwtPlotCurve*, QPair< QVector<double>*, QVector<double>* > > mkEntry(QwtPlotCurve* c, QVector<double>* x, QVector<double>* y);

protected:
	QwtPlot* graph;
	bool autoYScale;
	int history;
	int updateMs;
	double maxX;
	QMap< QString, QPair< QwtPlotCurve*, QPair< QVector<double>*, QVector<double>* > > > data;
	double ymin, ymax;

private:
	static const int defaultUpdateMs = 250;
	static const int defaultHistoryS = 60;
	static const bool defaultYAuto = true;
	static const double defaultYMin = 0;
	static const double defaultYMax = 100;
	PenStyleWidget* penWidget;
	QwtPlotCurve* selectedCurve;
};

#endif // TIMEGRAPHDISPLAY_HPP
