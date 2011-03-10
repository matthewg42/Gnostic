#ifndef DATADISPLAYEDITORFORM_HPP
#define DATADISPLAYEDITORFORM_HPP

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
	class DataDisplayEditorForm;
}

class DataDisplay;

class DataDisplayEditorForm : public QWidget
{
	Q_OBJECT

public:
	explicit DataDisplayEditorForm(QWidget *parent = 0);
	~DataDisplayEditorForm();

private:
	void populateTable();
	void clearCurrent();

private slots:
	void displayTableClicked(QModelIndex idx);
	void selectDataDisplay(const QString& section);
	void selectRowWithId(const QString& id);
	void markUpdated();
	void saveCurrent();
	void addNewDataDisplay();
	void deleteCurrent();
	void testCurrent();

private:
	Ui::DataDisplayEditorForm *ui;
	QStandardItemModel model;
	DataDisplay* current;
};

#endif // DATADISPLAYEDITORFORM_HPP
