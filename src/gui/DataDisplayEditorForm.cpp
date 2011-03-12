#include "DataDisplayEditorForm.hpp"
#include "ui_DataDisplayEditorForm.h"

#include "DataDisplay.hpp"
#include "DataDisplayConfigWidget.hpp"
#include "GnosticApp.hpp"
#include "GnosticParser.hpp"
#include "DummyTransport.hpp"

#include <QStandardItem>
#include <QList>
#include <QSettings>
#include <QDebug>
#include <QModelIndex>
#include <QInputDialog>
#include <QMessageBox>

DataDisplayEditorForm::DataDisplayEditorForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::DataDisplayEditorForm),
		current(NULL)
{
	ui->setupUi(this);


	connect(ui->displayTable, SIGNAL(clicked(QModelIndex)), this, SLOT(displayTableClicked(QModelIndex)));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveCurrent()));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addNewDataDisplay()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(deleteCurrent()));
	connect(ui->testButton, SIGNAL(clicked()), this, SLOT(testCurrent()));

	model.setHorizontalHeaderLabels(QStringList() << "Section" << "Display Description");
	ui->displayTable->setModel(&model);
	populateTable();

	if (model.rowCount()>0)
	{
		ui->displayTable->selectRow(0);
		displayTableClicked(ui->displayTable->currentIndex());
	}

	ui->saveButton->setEnabled(false);
}

DataDisplayEditorForm::~DataDisplayEditorForm()
{
	delete ui;
}



void DataDisplayEditorForm::populateTable()
{
	//qDebug() << "DataDisplayEditorForm::populateTable";
	model.clear();
	QSettings* s = GnosticApp::getInstance().settings();
	foreach (QString section, DataDisplay::getSections())
	{
		//qDebug() << "DataDisplayEditorForm::populateTable adding row for section " << section;
		QList<QStandardItem*> row;
		row << new QStandardItem(section)
				<< new QStandardItem(s->value(QString("%1/description").arg(section)).toString());

		//qDebug() << row;
		model.appendRow(row);
	}
	ui->displayTable->hideColumn(0);
	ui->displayTable->setColumnWidth(1, 300);

}

void DataDisplayEditorForm::clearCurrent()
{
	if (current)
	{
		DataDisplayConfigWidget* dconf = current->getConfigWidget(this);
		if (dconf)
			ui->configLayout->removeWidget(current->getConfigWidget(this));

		delete current;
		current = NULL;
	}
}

void DataDisplayEditorForm::displayTableClicked(QModelIndex idx)
{
	selectDataDisplay(model.index(idx.row(), 0).data().toString());
}

void DataDisplayEditorForm::selectDataDisplay(const QString& section)
{
	//qDebug() << "DataDisplayEditorForm::selectDataDisplay called with" << section;
	clearCurrent();

	ui->saveButton->setEnabled(false);
	current = DataDisplay::makeFromConfig(section);
	//qDebug() << "DataDisplayEditorForm::selectDataDisplay after load, current is" << current;
	if (current)
	{
		DataDisplayConfigWidget* dconf = current->getConfigWidget(this);
		//qDebug() << "DataDisplayEditorForm::selectDataDisplay" << section << dconf;
		if (dconf)
		{
			connect(dconf, SIGNAL(wasUpdated()), this, SLOT(markUpdated()));
			ui->configLayout->addWidget(dynamic_cast<QWidget*>(dconf));
		}
	}

}

void DataDisplayEditorForm::selectRowWithId(const QString& id)
{
	//qDebug() << "DataDisplayEditorForm::selectRowWithId" << id;
	QList<QStandardItem*> search = model.findItems(id);
	if (search.count() > 0)
	{
		ui->displayTable->selectRow(search.at(0)->row());
		displayTableClicked(ui->displayTable->currentIndex());
	}
}

void DataDisplayEditorForm::markUpdated()
{
	ui->saveButton->setEnabled(true);
}

void DataDisplayEditorForm::saveCurrent()
{
	if (current)
	{
		current->saveSettings();
		ui->saveButton->setEnabled(false);
		populateTable();
	}
}

void DataDisplayEditorForm::addNewDataDisplay()
{
	// we need to prompt for the type of display...
	bool ok;
	QString type = QInputDialog::getItem(this,
					     "Select the type for the display",
					     tr("Type:"),
					     DataDisplay::getAvailableTypes(),
					     0,
					     false,
					     &ok);
	if (ok && !type.isEmpty())
	{
		clearCurrent();
		current = DataDisplay::makeNew(type);
		current->setDescription(QString("new %1 display").arg(type));
		current->saveSettings();
		populateTable();
		selectRowWithId(current->getId());
	}
}

void DataDisplayEditorForm::deleteCurrent()
{
	if (current)
	{
		GnosticApp::getInstance().settings()->remove(current->getId());
		ui->configLayout->removeWidget(current->getConfigWidget(this));
		delete current;
		current = NULL;
		populateTable();

		if (model.rowCount()>0)
		{
			ui->displayTable->selectRow(0);
			displayTableClicked(ui->displayTable->currentIndex());
		}

	}
}

void DataDisplayEditorForm::testCurrent()
{
	//qDebug() << "DataDisplayEditorForm::testCurrent";
	// Create a parser and a dummy data source, and hook it up...

	GnosticParser *parser = new GnosticParser(current);
	Transport* dummy = new DummyTransport(current);
	connect(dummy, SIGNAL(spewLine(QString)), parser, SLOT(takeLine(QString)));
	connect(current, SIGNAL(wasClosed()), dummy, SLOT(stop()));
	if (current->wantDataTypes() & DataDisplay::Lines)
	{
		//qDebug() << "DataDisplayEditorForm::testCurrent it wants Lines... hooking up line feed...";
		connect(parser, SIGNAL(spewLine(QString)), current, SLOT(takeLine(QString)));
	}
	else //qDebug() << "DataDisplayEditorForm::testCurrent it doesn't want Lines...";

	if (current->wantDataTypes() & DataDisplay::DataItems)
	{
		//qDebug() << "DataDisplayEditorForm::testCurrent it wants DataItems... hooking up item feed...";
		connect(parser, SIGNAL(spewDataItem(double,double,QString)), current, SLOT(takeDataItem(double,double,QString)));
	}
	else
	{
		//qDebug() << "DataDisplayEditorForm::testCurrent it doesn't want DataItems...";
	}

	current->show();
	dummy->start("ignore");
}
