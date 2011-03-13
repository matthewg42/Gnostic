#ifndef REMOTECOMMANDCONFIGWIDGET_HPP
#define REMOTECOMMANDCONFIGWIDGET_HPP

#include <QWidget>
#include <QStandardItemModel>

class RemoteCommand;

class Transport;

namespace Ui {
	class RemoteCommandConfigWidget;
}

class RemoteCommandConfigWidget : public QWidget
{
	Q_OBJECT

public:
	explicit RemoteCommandConfigWidget(Transport* t, QWidget *parent = 0);
	~RemoteCommandConfigWidget();

	void clearCurrent();

private:
	void populateTable();
	void setControlsEnabled(bool b);
	void setControlsFromCommand(RemoteCommand* c);

private slots:
	void commandTableClicked(QModelIndex idx);
	void selectCommand(const QString& section);
	void selectRowWithId(const QString& id);
	void saveCurrent();
	void addNewCommand();
	void deleteCurrent();
	void testCurrent();
	void madeUpdate();

private:
	Ui::RemoteCommandConfigWidget *ui;
	QStandardItemModel model;
	Transport* transport;
	RemoteCommand* current;

};

#endif // REMOTECOMMANDCONFIGWIDGET_HPP
