#include "RemoteMonitorEditorForm.hpp"
#include "ui_RemoteMonitorEditorForm.h"

#include <QSettings>
#include <QDebug>

#include "RemoteMonitor.hpp"
#include "DataDisplay.hpp"
#include "RemoteCommand.hpp"
#include "GnosticApp.hpp"

RemoteMonitorEditorForm::RemoteMonitorEditorForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::RemoteMonitorEditorForm)
{
	current = NULL;

	ui->setupUi(this);

	connect(ui->monitorTable, SIGNAL(clicked(QModelIndex)), this, SLOT(monitorTableClicked(QModelIndex)));
	connect(ui->saveMonitorButton, SIGNAL(clicked()), this, SLOT(saveCurrent()));
	connect(ui->addMonitorButton, SIGNAL(clicked()), this, SLOT(addNewMonitor()));
	connect(ui->removeMonitorButton, SIGNAL(clicked()), this, SLOT(deleteCurrent()));
	connect(ui->testMonitorButton, SIGNAL(clicked()), this, SLOT(launchCurrent()));

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->commandTable, SIGNAL(clicked(QModelIndex)), this, SLOT(madeUpdate()));
	connect(ui->displayTable, SIGNAL(clicked(QModelIndex)), this, SLOT(madeUpdate()));

	monitorModel.setHorizontalHeaderLabels(QStringList() << "Section" << "Remote Monitor Description");
	ui->monitorTable->setModel(&monitorModel);
	ui->monitorTable->horizontalHeader()->setStretchLastSection(true);

	commandModel.setHorizontalHeaderLabels(QStringList() << "Section" << "Remote Command Description");
	ui->commandTable->setModel(&commandModel);
	ui->commandTable->horizontalHeader()->setStretchLastSection(true);

	displayModel.setHorizontalHeaderLabels(QStringList() << "Section" << "Display Description");
	ui->displayTable->setModel(&displayModel);
	ui->displayTable->horizontalHeader()->setStretchLastSection(true);

	populateTable();
	populateChildTables();

	if (monitorModel.rowCount()>0)
	{
		ui->monitorTable->selectRow(0);
		monitorTableClicked(ui->monitorTable->currentIndex());
	}

}

RemoteMonitorEditorForm::~RemoteMonitorEditorForm()
{
	delete ui;
}

void RemoteMonitorEditorForm::populateTable()
{
	qDebug() << "RemoteMonitorEditorForm::populateTable";
	monitorModel.clear();
	QSettings* s = GnosticApp::getInstance().settings();
	foreach (QString section, RemoteMonitor::getSections())
	{
		QList<QStandardItem*> row;
		row << new QStandardItem(section)
		    << new QStandardItem(s->value(QString("%1/description").arg(section)).toString());

		monitorModel.appendRow(row);
	}
	ui->saveMonitorButton->setEnabled(false);
	ui->testMonitorButton->setEnabled(true);
	ui->monitorTable->hideColumn(0);
}

void RemoteMonitorEditorForm::populateChildTables()
{
	qDebug() << "RemoteMonitorEditorForm::populateChildTables";
	displayModel.clear();
	commandModel.clear();
	QSettings* s = GnosticApp::getInstance().settings();

	foreach (QString section, RemoteCommand::getSections())
	{
		QList<QStandardItem*> row;
		QString desc = s->value(QString("%1/description").arg(section), "").toString();
		if (!desc.isEmpty())
		{
			row << new QStandardItem(section)
			    << new QStandardItem(desc);
		}
		commandModel.appendRow(row);
	}

	foreach (QString section, DataDisplay::getSections())
	{
		QList<QStandardItem*> row;
		QString desc = s->value(QString("%1/description").arg(section), "").toString();
		if (!desc.isEmpty())
		{
			row << new QStandardItem(section)
			    << new QStandardItem(desc);
		}
		displayModel.appendRow(row);
	}

	ui->commandTable->hideColumn(0);
	ui->displayTable->hideColumn(0);

}

void RemoteMonitorEditorForm::clearCurrent()
{
	if (current) {
		ui->descriptionEdit->setText(QString());
		delete current;
		current = NULL;
	}
}

void RemoteMonitorEditorForm::setControlsFromMonitor(RemoteMonitor* m)
{
	if (m)
	{
		ui->descriptionEdit->setText(current->getDescription());
	}
	else
		this->clearCurrent();
}

void RemoteMonitorEditorForm::monitorTableClicked(QModelIndex idx)
{
	selectMonitor(monitorModel.index(idx.row(), 0).data().toString());
}

void RemoteMonitorEditorForm::selectMonitor(const QString& section)
{
	clearCurrent();

	current = RemoteMonitor::makeFromConfig(section);
	if (current)
	{
		setControlsFromMonitor(current);
		// TODO set selection on display and command tables to reflect included items
	}
	ui->saveMonitorButton->setEnabled(false);
	ui->testMonitorButton->setEnabled(true);

}

void RemoteMonitorEditorForm::selectRowWithId(const QString& id)
{
	QList<QStandardItem*> search = monitorModel.findItems(id);
	if (search.count() > 0)
	{
		ui->monitorTable->selectRow(search.at(0)->row());
		monitorTableClicked(ui->monitorTable->currentIndex());
	}
}

void RemoteMonitorEditorForm::saveCurrent()
{
	if (current)
	{
		current->setDescription(ui->descriptionEdit->text());
		// TODO - make displays and command from tables and add them to current...
		foreach (QModelIndex idx, ui->commandTable->selectionModel()->selection().indexes())
		{
			QString section = commandModel.data(commandModel.index(idx.row(), 0)).toString();
			qDebug() << "RemoteMonitorEditorForm::saveCurrent got a selected command" << section;
			current->addRemoteCommand(section);
		}

		foreach (QModelIndex idx, ui->displayTable->selectionModel()->selection().indexes())
		{
			QString section = displayModel.data(displayModel.index(idx.row(), 0)).toString();
			qDebug() << "RemoteMonitorEditorForm::saveCurrent got a selected display" << section;
			current->addDisplay(section);
		}

		current->dumpDebug();
		current->saveSettings();
		ui->saveMonitorButton->setEnabled(false);
		ui->testMonitorButton->setEnabled(true);
		populateTable();
	}
}

void RemoteMonitorEditorForm::addNewMonitor()
{
	clearCurrent();
	current = new RemoteMonitor(this);
	current->setDescription("new command");
	current->saveSettings();
	populateTable();

	if (monitorModel.rowCount()>0)
	{
		ui->monitorTable->selectRow(0);
		monitorTableClicked(ui->monitorTable->currentIndex());
	}
}

void RemoteMonitorEditorForm::deleteCurrent()
{
	if (current)
	{
		GnosticApp::getInstance().settings()->remove(current->getId());
		clearCurrent();
		populateTable();

		if (monitorModel.rowCount()>0)
		{
			ui->monitorTable->selectRow(0);
			monitorTableClicked(ui->monitorTable->currentIndex());
		}
	}
}

void RemoteMonitorEditorForm::launchCurrent()
{
	if (!current)
		return;

	current->start();
}

void RemoteMonitorEditorForm::madeUpdate()
{
	ui->saveMonitorButton->setEnabled(true);
	ui->testMonitorButton->setEnabled(false);
}
