#ifndef REMOTEMONITOREDITORFORM_HPP
#define REMOTEMONITOREDITORFORM_HPP

#include <QWidget>
#include <QStandardItemModel>

#include "GnosticApp.hpp"

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
	void setControlsFromMonitor(RemoteMonitor* m);

private slots:
	void monitorTableClicked(QModelIndex idx);
	void selectMonitor(const QString& section);
	void selectRowWithId(const QString& id);
	void saveCurrent();
	void addNewMonitor();
	void deleteCurrent();
	void launchCurrent();
	void madeUpdate();
        void externalUpdate(GnosticApp::ConfigType);
        void populateTables();
        void populateChildTables();

private:
	Ui::RemoteMonitorEditorForm *ui;
	QStandardItemModel monitorModel;
	QStandardItemModel commandModel;
	QStandardItemModel displayModel;
	RemoteMonitor* current;
};

#endif // REMOTEMONITOREDITORFORM_HPP
