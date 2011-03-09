#include "TransportEditorForm.hpp"
#include "ui_TransportEditorForm.h"

#include "Transport.hpp"
#include "TransportConfigWidget.hpp"
#include "GnosticApp.hpp"


#include <QStandardItem>
#include <QList>
#include <QSettings>
#include <QDebug>
#include <QModelIndex>

TransportEditorForm::TransportEditorForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::TransportEditorForm),
		current(NULL)
{
	ui->setupUi(this);

	model.setHorizontalHeaderLabels(QStringList() << "Section" << "Transport Description");
	ui->transportTable->setModel(&model);
	ui->transportTable->hideColumn(0);
	ui->transportTable->setColumnWidth(1, 200);

	connect(ui->transportTable, SIGNAL(clicked(QModelIndex)), this, SLOT(transportTableClicked(QModelIndex)));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveCurrent()));

	populateTable();
	ui->saveButton->setEnabled(false);
}

TransportEditorForm::~TransportEditorForm()
{
	delete ui;
}



void TransportEditorForm::populateTable()
{
	qDebug() << "TransportEditorForm::populateTable";
	model.clear();
	QSettings* s = GnosticApp::getInstance().settings();
	foreach (QString section, Transport::getTransportSections())
	{
		qDebug() << "TransportEditorForm::populateTable adding row for section " << section;
		QList<QStandardItem*> row;
		row << new QStandardItem(section)
		<< new QStandardItem(s->value(QString("%1/description").arg(section)).toString());

		qDebug() << row;
		model.appendRow(row);
	}
}

void TransportEditorForm::transportTableClicked(QModelIndex idx)
{
	selectTransport(model.index(idx.row(), 0).data().toString());
}

void TransportEditorForm::selectTransport(const QString& section)
{
	if (current)
	{
		ui->configLayout->removeWidget(current->getConfigWidget(this));
		delete current;
	}

	ui->saveButton->setEnabled(false);
	current = Transport::loadTransport(section);
	if (current)
	{
		TransportConfigWidget* tconf = current->getConfigWidget(this);
		connect(tconf, SIGNAL(wasUpdated()), this, SLOT(markUpdated()));
		ui->configLayout->addWidget(dynamic_cast<QWidget*>(tconf));
	}

}

void TransportEditorForm::markUpdated()
{
	ui->saveButton->setEnabled(true);
}

void TransportEditorForm::saveCurrent()
{
	if (current)
	{
		current->saveTransport();
		ui->saveButton->setEnabled(false);
		populateTable();
	}
}
