#include "TransportEditorForm.hpp"
#include "ui_TransportEditorForm.h"

#include "Transport.hpp"
#include "TransportConfigWidget.hpp"
#include "RemoteCommandConfigWidget.hpp"
#include "RemoteCommand.hpp"
#include "GnosticApp.hpp"

#include <QStandardItem>
#include <QList>
#include <QSettings>
#include <QDebug>
#include <QModelIndex>
#include <QInputDialog>
#include <QMessageBox>

TransportEditorForm::TransportEditorForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::TransportEditorForm)
{
	ui->setupUi(this);

	current = NULL;
	commandWidget = NULL;

	connect(ui->transportTable, SIGNAL(clicked(QModelIndex)), this, SLOT(transportTableClicked(QModelIndex)));
	connect(ui->saveTransportButton, SIGNAL(clicked()), this, SLOT(saveCurrent()));
	connect(ui->addTransportButton, SIGNAL(clicked()), this, SLOT(addNewTransport()));
	connect(ui->removeTransportButton, SIGNAL(clicked()), this, SLOT(deleteCurrent()));
	connect(ui->testTransportButton, SIGNAL(clicked()), this, SLOT(testCurrent()));

	model.setHorizontalHeaderLabels(QStringList() << "Section" << "Transport Description");
	ui->transportTable->setModel(&model);
	ui->transportTable->horizontalHeader()->setStretchLastSection(true);
	populateTable();

	ui->saveTransportButton->setEnabled(false);
}

TransportEditorForm::~TransportEditorForm()
{
	delete ui;
}

void TransportEditorForm::populateTable()
{
	//qDebug() << "TransportEditorForm::populateTable";
        QString oldSelection;
        QModelIndexList selectedRows = ui->transportTable->selectionModel()->selectedRows();
        if (selectedRows.count()>0)
                oldSelection = selectedRows.at(0).data().toString();

	model.clear();

	QSettings* s = GnosticApp::getInstance().settings();
	foreach (QString section, Transport::getSections())
	{
		//qDebug() << "TransportEditorForm::populateTable adding row for section " << section;
		QList<QStandardItem*> row;
		row << new QStandardItem(section)
				<< new QStandardItem(s->value(QString("%1/description").arg(section)).toString());

		//qDebug() << row;
		model.appendRow(row);
	}
	ui->transportTable->hideColumn(0);
        if (!selectedRows.isEmpty())
        {
                selectRowWithId(oldSelection);
        }
        else if (model.rowCount() > 0)
        {
                selectRowWithId(model.item(0,0)->data(Qt::DisplayRole).toString());
        }
}

void TransportEditorForm::clearCurrent()
{

	for(int i=0; i<ui->commandLayout->count(); i++)
	{
		ui->commandLayout->removeItem(ui->commandLayout->itemAt(i));
	}

	if (current)
	{
		TransportConfigWidget* tconf = current->getConfigWidget(this);
		if (tconf)
			ui->transportLayout->removeWidget(current->getConfigWidget(this));

		delete current;
		current = NULL;
	}
}

void TransportEditorForm::transportTableClicked(QModelIndex idx)
{
	selectTransport(model.index(idx.row(), 0).data().toString());
}

void TransportEditorForm::selectTransport(const QString& section)
{	
	clearCurrent();

	ui->saveTransportButton->setEnabled(false);
	current = Transport::makeFromConfig(section);

	if (current)
	{
		TransportConfigWidget* tconf = current->getConfigWidget();
		if(tconf)
		{
			connect(tconf, SIGNAL(wasUpdated()), this, SLOT(markUpdated()));
			ui->transportLayout->addWidget(dynamic_cast<QWidget*>(tconf));
		}

		RemoteCommandConfigWidget* cconf = current->getCommandWidget();
		if (cconf)
		{
			ui->commandLayout->addWidget(cconf);
		}
	}

}

void TransportEditorForm::selectRowWithId(const QString& id)
{
	QList<QStandardItem*> search = model.findItems(id);
	if (search.count() > 0)
	{
		ui->transportTable->selectRow(search.at(0)->row());
		transportTableClicked(ui->transportTable->currentIndex());
	}
}

void TransportEditorForm::markUpdated()
{
	ui->saveTransportButton->setEnabled(true);

}

void TransportEditorForm::saveCurrent()
{
	for(int i=0; i<ui->commandLayout->count(); i++)
	{
		ui->commandLayout->removeItem(ui->commandLayout->itemAt(i));
	}

	if (current)
	{
		QString theId = current->getId();
		current->saveSettings();
		ui->saveTransportButton->setEnabled(false);
		populateTable();
		selectRowWithId(theId);
	}
}

void TransportEditorForm::addNewTransport()
{
	// we need to prompt for the type of transport...
	bool ok;
	QString type = QInputDialog::getItem(this,
					     "Select the type for the transport",
					     tr("Type:"),
					     Transport::getAvailableTypes(),
					     0,
					     false,
					     &ok);
	if (ok && !type.isEmpty())
	{
		clearCurrent();
		current = Transport::makeNew(type);
		current->setDescription(QString("new %1 transport").arg(type));
		current->saveSettings();
		populateTable();
		ui->transportTable->selectRow(model.findItems(current->getId()).at(0)->row());
		transportTableClicked(ui->transportTable->currentIndex());
	}
}

void TransportEditorForm::deleteCurrent()
{
	if (current)
	{
		GnosticApp::getInstance().settings()->remove(current->getId());
		RemoteCommand::removeForTransport(current->getId());
		ui->transportLayout->removeWidget(current->getConfigWidget(this));

		for(int i=0; i<ui->commandLayout->count(); i++)
		{
			ui->commandLayout->removeItem(ui->commandLayout->itemAt(i));
		}

		delete current;
		current = NULL;
		populateTable();
                GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Transport);
	}
}

void TransportEditorForm::testCurrent()
{
	QMessageBox mb(this);

	if (!current)
	{
		mb.setText("No transport selected");
		mb.setIcon(QMessageBox::Warning);
	}
	else
	{
		if (current->test())
		{
			mb.setText("Transport configuration looks good");
			mb.setIcon(QMessageBox::Information);
		}
		else
		{
			mb.setText("Transport test failed.  How non-cromulent");
			mb.setIcon(QMessageBox::Warning);
		}
	}

	mb.exec();
}
