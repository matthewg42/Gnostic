#include "TransportConfig.hpp"
#include "ui_TransportConfig.h"

#include <QDebug>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QSettings>
#include <QListView>
#include <QStandardItemModel>
#include <QList>
#include <QModelIndex>

#include "LocalTransport.hpp"
#include "TransportConfigWidget.hpp"
#include "GnosticApp.hpp"

TransportConfig::TransportConfig(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::TransportConfig),
		transport(NULL),
		configWidget(NULL)
{
	ui->setupUi(this);
	ui->typeCombo->insertItem(0, "LocalTransport");
	model = new QStandardItemModel(this);
	populateTransportModel();
	connect(ui->typeCombo, SIGNAL(activated(QString)), this, SLOT(changeTransportType(QString)));
	connect(ui->transportView, SIGNAL(clicked(QModelIndex)), this, SLOT(transportSelectionChanged(QModelIndex)));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentTransport()));
}

TransportConfig::~TransportConfig()
{
	delete ui;
}

void TransportConfig::doTest()
{
	qDebug() << "TransportConfig::doTest doTest...";
	QMessageBox mb;
	if (!transport->testTransport())
		mb.setText("Transport test FAILED - check your settings!");
	else
		mb.setText("Transport test was SUCCESSFUL");

	mb.exec();
}

void TransportConfig::changeTransportType(const QString newType, Transport* newTransport)
{
	qDebug() << "TransportConfig::changeTransportType" << newType << newTransport;
	if (newTransport==NULL)
	{
		if (transport)
		{
			delete transport;
			transport = NULL;
		}
	}

	if (newType == "LocalTransport")
	{
		if (newTransport==NULL)
			transport = new LocalTransport();
		else
			transport = newTransport;

		if (configWidget!=NULL)
		{
			ui->configLayout->removeWidget(dynamic_cast<QWidget*>(configWidget));
			delete configWidget;
		}

		configWidget = transport->getConfigWidget();
		connect(configWidget, SIGNAL(configChanged()), this, SLOT(configUpdated()));
		ui->configLayout->addWidget(configWidget);
	}
	else
	{
		qWarning() << "TransportConfig::changeTransportType unknown transport type selected" << newType;
	}
}

void TransportConfig::populateTransportModel()
{
	qDebug() << "TransportConfig::populateTransportModel";
	QSettings* settings = GnosticApp::getInstance().settings();
	model->clear();
	foreach (QString id, GnosticApp::getInstance().getTransportIds())
	{
		QString desc = settings->value(QString("%1/description").arg(id), "").toString();
		qDebug() << "adding" << id << desc;
		QList<QStandardItem*> row;
		row << new QStandardItem(id) << new QStandardItem(desc);
		model->appendRow(row);
	}
	ui->transportView->setModel(model);
	ui->transportView->hideColumn(0);
	ui->transportView->setColumnWidth(1, 1000);
}

void TransportConfig::transportSelectionChanged(QModelIndex i)
{
	int row = i.row();
	loadTransportWithId(model->item(row, 0)->text());
}

void TransportConfig::loadTransportWithId(const QString& id)
{
	Transport* newTransport = GnosticApp::getInstance().getTransportWithId(id);
	if (!newTransport)
	{
		qWarning() << "TransportConfig::loadTransportWithId failed to get a transport for id" << id;
		return;
	}

	int typeIdx = ui->typeCombo->findText(newTransport->getType());
	if (typeIdx==-1)
	{
		qWarning() << "TransportConfig::loadTransportWithId failed to find type in combo" << newTransport->getType();
		return;
	}
	ui->typeCombo->setCurrentIndex(typeIdx);

	if (transport)
	{
		delete transport;
		transport = NULL;
	}

	changeTransportType(newTransport->getType(), newTransport);
	ui->saveButton->setEnabled(false);
}

void TransportConfig::configUpdated()
{
	qDebug() << "TransportConfig::configUpdated received changed";
	ui->saveButton->setEnabled(true);
}

void TransportConfig::saveCurrentTransport()
{
	qDebug() << "TransportConfig::saveCurrentTransport";
	transport->saveTransport();
	QString id = transport->getId();
	populateTransportModel();
	foreach (QStandardItem* i, model->findItems(id))
	{
		ui->transportView->selectRow(i->row());
		break;
	}
	ui->saveButton->setEnabled(false);
}

void TransportConfig::createNewTransport()
{
//	model->appendRow(GnosticApp::getNewTransportId())
//	QList<QStandardItem*> row;
//	row << new QStandardItem(id) << new QStandardItem(desc);
//	model->appendRow(row);
	// TODO: work out how to make a new object with an ID which doesn't exist in config.ini file yet
	// and which we can safely change the type of... first need to make an alternative type and
	// check that doesn't cause problems.

}
