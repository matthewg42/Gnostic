#ifndef GNOSTICMAINWINDOW_HPP
#define GNOSTICMAINWINDOW_HPP

#include <QMainWindow>

#include "Transport.hpp"

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

private slots:
	void receiveData(QString label, double value, qint64 timeStamp);
        void transportStatusChanged(Transport::TransportStatus newStatus);
	void doTest();

private:
	static GnosticMainWindow* singleton;
	Ui::GnosticMainWindow *ui;
	Transport* transport;
};

#endif // GNOSTICMAINWINDOW_HPP
