#include "RemoteMonitorEditorForm.hpp"
#include "ui_RemoteMonitorEditorForm.h"

#include <QSettings>
#include <QDebug>
#include <QMessageBox>

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

	ui->monitorTable->setModel(&monitorModel);
	ui->monitorTable->horizontalHeader()->setStretchLastSection(true);
        ui->commandTable->setModel(&commandModel);
	ui->commandTable->horizontalHeader()->setStretchLastSection(true);

	displayModel.setHorizontalHeaderLabels(QStringList() << "Section" << "Display Description");
	ui->displayTable->setModel(&displayModel);
	ui->displayTable->horizontalHeader()->setStretchLastSection(true);

        connect(&GnosticApp::getInstance(), SIGNAL(configUpdated(GnosticApp::ConfigType)), this, SLOT(externalUpdate(GnosticApp::ConfigType)));

        populateTables();

}

RemoteMonitorEditorForm::~RemoteMonitorEditorForm()
{
	delete ui;
}

void RemoteMonitorEditorForm::populateTables()
{
        qDebug() << "RemoteMonitorEditorForm::populateTables";
        // remember the old selection if there is one...
        QString oldSelectionId;
        QModelIndexList oldSelection = ui->monitorTable->selectionModel()->selectedRows();
        if (oldSelection.count()>0)
        {
                oldSelectionId = oldSelection.at(0).data().toString();
                qDebug() << "RemoteMonitorEditorForm::populateTables oldSelectionId was" << oldSelectionId;
        }

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
        populateChildTables();
        if (!oldSelectionId.isEmpty())
        {
                selectRowWithId(oldSelectionId);
        }
        else if (monitorModel.rowCount()>0)
        {
                selectRowWithId(monitorModel.item(0,0)->data(Qt::DisplayRole).toString());
        }
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

        commandModel.setHorizontalHeaderLabels(QStringList() << "Section" << "Remote Commands");
        displayModel.setHorizontalHeaderLabels(QStringList() << "Section" << "Remote Displays");
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
	populateChildTables();
}

void RemoteMonitorEditorForm::setControlsFromMonitor(RemoteMonitor* m)
{
	if (m)
	{
		qDebug() << "RemoteMonitorEditorForm::setControlsFromMonitor";
		ui->descriptionEdit->setText(current->getDescription());

		// select the rows in the command table for this monitor...
		QList<int> rowsToSelect;
		foreach (RemoteCommand* c, m->getCommands())
		{
			QList<QStandardItem*> search = commandModel.findItems(c->getId());
			for(int i=0; i<search.count(); i++)
			{
				rowsToSelect << search.at(0)->row();
			}
		}
		foreach(int i, rowsToSelect)
		{
			ui->commandTable->selectRow(i);
		}

		// and for the displays...
		rowsToSelect.clear();
		foreach (DataDisplay* c, m->getDisplays())
		{
			QList<QStandardItem*> search = displayModel.findItems(c->getId());
			for(int i=0; i<search.count(); i++)
			{
				rowsToSelect << search.at(0)->row();
			}
		}
		foreach(int i, rowsToSelect)
		{
			ui->displayTable->selectRow(i);
		}
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
	qDebug() << "######################## SAVE ##############################";
	if (current)
	{
		current->setDescription(ui->descriptionEdit->text());
		current->clearDisplays();
		current->clearCommands();

		foreach (QModelIndex idx, ui->commandTable->selectionModel()->selection().indexes())
		{
			if (idx.column()==1)
			{
				QString section = commandModel.data(commandModel.index(idx.row(), 0)).toString();
				qDebug() << "RemoteMonitorEditorForm::saveCurrent got a selected command" << section << "from" << idx.row() << idx.column();
				current->addRemoteCommand(section);
			}
		}

		foreach (QModelIndex idx, ui->displayTable->selectionModel()->selection().indexes())
		{
			if (idx.column()==1)
			{
				QString section = displayModel.data(displayModel.index(idx.row(), 0)).toString();
				qDebug() << "RemoteMonitorEditorForm::saveCurrent got a selected display" << section;
				current->addDisplay(section);
			}
		}

		current->dumpDebug();
		current->saveSettings();
		ui->saveMonitorButton->setEnabled(false);
		ui->testMonitorButton->setEnabled(true);
                populateTables();
	}
}

void RemoteMonitorEditorForm::addNewMonitor()
{
	clearCurrent();
	current = new RemoteMonitor(this);
	current->setDescription("new monitor");
	current->saveSettings();
        QString id = current->getId();
        populateTables();
        selectRowWithId(id);

}

void RemoteMonitorEditorForm::deleteCurrent()
{
	if (current)
	{
		GnosticApp::getInstance().settings()->remove(current->getId());
		clearCurrent();
                populateTables();
                GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Monitor);
	}
}

void RemoteMonitorEditorForm::launchCurrent()
{
	if (!current)
		return;

	if (!current->start())
	{
		QMessageBox mb;
		mb.setText("Failed to start RemoteMonitor " + current->getDescription());
		mb.setIcon(QMessageBox::Warning);
		mb.exec();
	}
}

void RemoteMonitorEditorForm::madeUpdate()
{
	ui->saveMonitorButton->setEnabled(true);
	ui->testMonitorButton->setEnabled(false);
}

void RemoteMonitorEditorForm::externalUpdate(GnosticApp::ConfigType t)
{
        if (t != GnosticApp::Monitor)
                populateTables();
}
