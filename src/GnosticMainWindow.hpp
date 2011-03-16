#ifndef GNOSTICMAINWINDOW_HPP
#define GNOSTICMAINWINDOW_HPP

#include <QMainWindow>
#include <QModelIndex>

#include "ConfigurationWindow.hpp"


namespace Ui {
	class GnosticMainWindow;
}

class GnosticMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit GnosticMainWindow(QWidget *parent = 0);
	~GnosticMainWindow();
	static GnosticMainWindow& getInstance();

public slots:
	void showConfigWindow(bool b=true);
	void setFullscreen(bool b=true);
	void showMonitors(bool b=true);
	void refreshMonitorList();
	void monitorTableLaunch(QModelIndex idx);

private:
	static GnosticMainWindow* singleton;
	Ui::GnosticMainWindow *ui;

	ConfigurationWindow* configWindow;
};

#endif // GNOSTICMAINWINDOW_HPP
