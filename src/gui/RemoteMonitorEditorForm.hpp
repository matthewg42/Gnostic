#ifndef REMOTEMONITOREDITORFORM_HPP
#define REMOTEMONITOREDITORFORM_HPP

#include <QWidget>
#include <QStandardItemModel>

class RemoteMonitor;

namespace Ui {
	class RemoteMonitorEditorForm;
}

class RemoteMonitorEditorForm : public QWidget
{
	Q_OBJECT

public:
	explicit RemoteMonitorEditorForm(QWidget *parent = 0);
	~RemoteMonitorEditorForm();

	void clearCurrent();

private:
	void populateTable();
	void populateChildTables();
	void setControlsFromMonitor(RemoteMonitor* m);

private slots:
	void monitorTableClicked(QModelIndex idx);
	void selectMonitor(const QString& section);
	void selectRowWithId(const QString& id);
	void saveCurrent();
	void addNewMonitor();
	void deleteCurrent();
	void testCurrent();
	void madeUpdate();

private:
	Ui::RemoteMonitorEditorForm *ui;
	QStandardItemModel monitorModel;
	QStandardItemModel commandModel;
	QStandardItemModel displayModel;
	RemoteMonitor* current;
};

#endif // REMOTEMONITOREDITORFORM_HPP
