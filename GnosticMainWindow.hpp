#ifndef GNOSTICMAINWINDOW_HPP
#define GNOSTICMAINWINDOW_HPP

#include <QMainWindow>

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

private:
	static GnosticMainWindow* singleton;
	Ui::GnosticMainWindow *ui;
};

#endif // GNOSTICMAINWINDOW_HPP
