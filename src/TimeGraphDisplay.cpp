#include "TimeGraphDisplay.hpp"
#include "ui_TimeGraphDisplay.h"

#include "TimeGraphDisplayConfigWidget.hpp"
#include "GnosticApp.hpp"
#include "PenStyleWidget.hpp"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_item.h>
#include <qwt_scale_draw.h>

#include <QDebug>
#include <QSettings>
#include <QScrollBar>
#include <QDateTime>
#include <QTimer>
#include <QMenu>
#include <QIcon>
#include <QContextMenuEvent>


// TODO: split TimeScaleDraw out into separate files
class TimeScaleDraw: public QwtScaleDraw
{
public:
	TimeScaleDraw();
	~TimeScaleDraw();

	QPoint labelPosition (double val) const;

protected:
	virtual QwtText label(double v) const;

private:
	QFont* scaleFont;
};

TimeScaleDraw::TimeScaleDraw()
{
	scaleFont = new QFont();
	scaleFont->setPointSize(8);
}

TimeScaleDraw::~TimeScaleDraw()
{
	delete scaleFont;
}

QwtText TimeScaleDraw::label(double v) const
{
	QwtText t(QDateTime::fromMSecsSinceEpoch(v).toString("              hh:mm:ss"));
	t.setFont(*scaleFont);
	return t;
}
// end TimeScaleDraw

TimeGraphDisplay::TimeGraphDisplay(QWidget *parent) :
		DataDisplay(parent),
		ui(new Ui::TimeGraphDisplay),
		autoYScale(TimeGraphDisplay::defaultYAuto),
		history(TimeGraphDisplay::defaultHistoryS),
		updateMs(TimeGraphDisplay::defaultUpdateMs),
		ymin(TimeGraphDisplay::defaultYMin),
		ymax(TimeGraphDisplay::defaultYMax),
		followRealTime(false)
{
	ui->setupUi(this);

	penWidget = NULL;
	selectedCurve = NULL;

	graph = new QwtPlot(this);
	ui->vLayout->addWidget(dynamic_cast<QWidget*>(graph));
	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
	grid->attach(graph);
	graph->setCanvasBackground(QColor(29, 100, 141)); // nice blue
	graph->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());
	graph->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
	graph->legend()->setItemMode(QwtLegend::ClickableItem);
	graph->setAxisLabelRotation(QwtPlot::xBottom, 90);

	// Set time to now
	maxX = QDateTime::currentMSecsSinceEpoch();

	// re-draw the time scale from maxX to maxX - history seconds
	setXScale();

	// Set up the y-scale
	setYScale();

	connect(graph, SIGNAL(legendClicked(QwtPlotItem*)), this, SLOT(setPlotItemStyle(QwtPlotItem*)));

	// Make a timer to call redrawing of graph.
	redrawTimer = new QTimer(this);
	redrawTimer->setInterval(updateMs);
	redrawTimer->setSingleShot(false);
	connect(redrawTimer, SIGNAL(timeout()), graph, SLOT(replot()));
	connect(redrawTimer, SIGNAL(timeout()), this, SLOT(updateXMax()));
	redrawTimer->start();
}

TimeGraphDisplay::~TimeGraphDisplay()
{
	// clean up the history data store
	foreach (QString label, data.keys())
	{
		delete data[label].first;
		delete data[label].second.first;
		delete data[label].second.second;
		data.remove(label);
	}
	delete ui;
	delete graph;
}

DataDisplayConfigWidget* TimeGraphDisplay::getConfigWidget(QWidget* parent)
{
	if (configWidget==NULL)
		configWidget = new TimeGraphDisplayConfigWidget(this, parent);

	return configWidget;
}

const QString& TimeGraphDisplay::saveSettings()
{
	//qDebug() << "TimeGraphDisplay::saveSettings";
	DataDisplay::saveSettings();

	QSettings* settings = GnosticApp::getInstance().settings();
	settings->setValue(QString("%1/history").arg(id), getHistory());
        settings->setValue(QString("%1/y_auto_scale").arg(id), getYAutoScale());
	settings->setValue(QString("%1/y_min").arg(id), getYMin());
	settings->setValue(QString("%1/y_max").arg(id), getYMax());
	settings->setValue(QString("%1/update_ms").arg(id), getUpdateMs());
	settings->setValue(QString("%1/hidden").arg(id), hidden);
	settings->setValue(QString("%1/follow_real_time").arg(id), getFollowRealTime());
	GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Display);
	return id;
}

bool TimeGraphDisplay::loadSettings(const QString& section)
{
	//qDebug() << "TimeGraphDisplay::loadSettings" << section;
	if (!DataDisplay::loadSettings(section))
		return false;

	QSettings* settings = GnosticApp::getInstance().settings();
	setHistory(settings->value(QString("%1/history").arg(section), TimeGraphDisplay::defaultHistoryS).toInt());
	setYAutoScale(settings->value(QString("%1/y_auto_scale").arg(section), TimeGraphDisplay::defaultYAuto).toBool());
	setYMin(settings->value(QString("%1/y_min").arg(section), TimeGraphDisplay::defaultYMin).toDouble());
	setYMax(settings->value(QString("%1/y_max").arg(section), TimeGraphDisplay::defaultYMax).toDouble());
	setUpdateMs(settings->value(QString("%1/update_ms").arg(section), TimeGraphDisplay::defaultUpdateMs).toInt());
	hidden = settings->value(QString("%1/hidden").arg(section), "").toStringList();
	setFollowRealTime(settings->value(QString("%1/follow_real_time").arg(section), false).toBool());

	return true;
}

void TimeGraphDisplay::dumpDebug()
{
	//qDebug() << "TimeGraphDisplay::dumpDebug calling DataDisplay::dumpDebug()";
	//qDebug() << "TimeGraphDisplay::dumpDebug history" << getHistory();
	//qDebug() << "TimeGraphDisplay::dumpDebug yauto" << getYAutoScale();
	//qDebug() << "TimeGraphDisplay::dumpDebug ymin" << getYMin();
	//qDebug() << "TimeGraphDisplay::dumpDebug ymax" << getYMax();
	//qDebug() << "TimeGraphDisplay::dumpDebug updateMs" << getUpdateMs();
}

void TimeGraphDisplay::setHistory(int h)
{
	if (history > 0)
		history = h;
}

int TimeGraphDisplay::getHistory()
{
	return history;
}

void TimeGraphDisplay::takeDataItem(const double timestamp, const double value, const QString label)
{
	// TODO - handle out-of-order timestamps...
	QVector<double>* xValues;
	QVector<double>* yValues;
	QwtPlotCurve* curve;

	if (!data.contains(label))
	{
		curve = new QwtPlotCurve(label);
		curve->attach(graph);
		// curve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
                curve->setPen(GnosticApp::getInstance().getRecentPen(label, getId()));
		xValues = new QVector<double>;
		yValues = new QVector<double>;
		data.insert(label, mkEntry(curve, xValues, yValues));
	}
	else
	{
		curve = data[label].first;
		xValues = data[label].second.first;
		yValues = data[label].second.second;
	}

	// point at the last timestamp in the data
	int insertIdx = xValues->size();
	for(int i=insertIdx-1; i>=0; i--)
	{
		if (xValues->at(i) > timestamp)
			insertIdx = i;
	}

	// if were still at the end, we should use the [efficient] append function...
	if (insertIdx > xValues->size())
	{
		xValues->append(timestamp);
		yValues->append(value);
	}
	else
	{
		//qDebug() << "inserting at" << insertIdx << "size is" << xValues->size();
		xValues->insert(insertIdx, timestamp);
		yValues->insert(insertIdx, value);
	}

	// xValues should be sorted, so we set the maxX to the last value...
	if (followRealTime)
		maxX = QDateTime::currentMSecsSinceEpoch();
	else
		maxX = xValues->at(xValues->size()-1);

	// remove values which are too old
	while (xValues->at(0) < maxX - (history*1000))
	{
		xValues->pop_front();
		yValues->pop_front();
	}

#ifdef QWT_API_VER_6
        // Argh, this is really horrible, sorry.  I have different versions of Qwt installed
        // on Linux and Windows with slightly different APIs
        data[label].first->setRawSamples(xValues->constData(), yValues->constData(), xValues->size());
#else
	data[label].first->setRawData(xValues->constData(), yValues->constData(), xValues->size());
#endif

	setXScale();

}

bool TimeGraphDisplay::getYAutoScale()
{
	return autoYScale;
}

void TimeGraphDisplay::setYAutoScale(bool b)
{
	autoYScale = b;
	setYScale();
}

bool TimeGraphDisplay::getYManualScale()
{
	return !autoYScale;
}

void TimeGraphDisplay::setYManualScale(bool b)
{
	autoYScale = !b;
	setYScale();
}

void TimeGraphDisplay::setYMin(double d)
{
	ymin = d;
	setYScale();
}

double TimeGraphDisplay::getYMin()
{
	return ymin;
}

void TimeGraphDisplay::setYMax(double d)
{
	ymax = d;
	setYScale();
}

double TimeGraphDisplay::getYMax()
{
	return ymax;
}

void TimeGraphDisplay::setUpdateMs(int ms)
{
	updateMs = ms;
	redrawTimer->setInterval(updateMs);
}

int TimeGraphDisplay::getUpdateMs()
{
	return updateMs;
}

void TimeGraphDisplay::setFollowRealTime(bool b)
{
	followRealTime = b;
}

bool TimeGraphDisplay::getFollowRealTime()
{
	return followRealTime;
}

void TimeGraphDisplay::updateXMax()
{
	if (getFollowRealTime())
	{
		maxX = QDateTime::currentMSecsSinceEpoch();
		setXScale();
	}
}

void TimeGraphDisplay::setXScale()
{
	double behind = (history-1) * 1000;
	double ahead = behind * 0.02;
	graph->setAxisScale(QwtPlot::xBottom, maxX - behind, maxX + ahead);
}

void TimeGraphDisplay::setYScale()
{
	graph->setAxisScale(QwtPlot::yLeft, ymin, ymax);
	if (autoYScale)
		graph->setAxisAutoScale(QwtPlot::yLeft);
}

void TimeGraphDisplay::setPlotItemStyle(QwtPlotItem* item)
{
	if (!item)
		return;

	selectedCurve = static_cast<QwtPlotCurve*>(item);

	if (!penWidget)
		penWidget = new PenStyleWidget();

	penWidget->disconnect(this);
	connect(penWidget, SIGNAL(setPen(QPen)), this, SLOT(setNewPen(QPen)));
	penWidget->setInitialPen(selectedCurve->pen());
	penWidget->show();

}


void TimeGraphDisplay::setNewPen(QPen p)
{
	if (selectedCurve)
	{
		selectedCurve->setPen(p);
                GnosticApp::getInstance().setRecentPen(selectedCurve->title().text(), getId(), p);
	}
}

QPair< QwtPlotCurve*, QPair< QVector<double>*, QVector<double>* > > TimeGraphDisplay::mkEntry(
		QwtPlotCurve* c,
		QVector<double>* x,
		QVector<double>* y)
{
	QPair< QwtPlotCurve*, QPair< QVector<double>*, QVector<double>* > > ret;
	ret.first = c;
	ret.second.first = x;
	ret.second.second = y;
	return ret;
}

void TimeGraphDisplay::contextMenuEvent(QContextMenuEvent* event)
{
        QMenu menu(this);
        menu.addAction(QIcon(":/icons/images/exit.png"), "Close display", this, SLOT(close()));
        menu.exec(mapToGlobal(event->pos()));
        return;
}


