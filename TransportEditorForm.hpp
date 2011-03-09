#ifndef TRANSPORTEDITORFORM_HPP
#define TRANSPORTEDITORFORM_HPP

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
	class TransportEditorForm;
}

class Transport;

class TransportEditorForm : public QWidget
{
	Q_OBJECT

public:
	explicit TransportEditorForm(QWidget *parent = 0);
	~TransportEditorForm();

private:
	void populateTable();
	void clearCurrent();

private slots:
	void transportTableClicked(QModelIndex idx);
	void selectTransport(const QString& section);
	void markUpdated();
	void saveCurrent();
	void addNewTransport();
	void deleteCurrent();
	void testCurrent();

private:
	Ui::TransportEditorForm *ui;
	QStandardItemModel model;
	Transport* current;
};

#endif // TRANSPORTEDITORFORM_HPP
