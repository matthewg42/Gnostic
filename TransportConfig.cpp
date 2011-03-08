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
#include "SshTransport.hpp"
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
	ui->typeCombo->insertItem(0, "SshTransport");
	model = new QStandardItemModel(this);
	populateTransportModel();
	connect(ui->typeCombo, SIGNAL(activated(QString)), this, SLOT(changeTransportType(QString)));
	connect(ui->transportView, SIGNAL(clicked(QModelIndex)), this, SLOT(transportSelectionChanged(QModelIndex)));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveCurrentTransport()));
	connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentTransport()));
	connect(ui->testButton, SIGNAL(clicked()), this, SLOT(testCurrentTransport()));
	connect(ui->newButton, SIGNAL(clicked()), this, SLOT(createNewTransport()));
}

TransportConfig::~TransportConfig()
{
	delete ui;
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
	}
	else if (newType == "SshTransport")
	{
		if (newTransport==NULL)
			transport = new SshTransport();
		else
			transport = newTransport;
	}
	else
	{
		qWarning() << "TransportConfig::changeTransportType unknown transport type selected" << newType;
	}

	if (configWidget!=NULL)
	{
		ui->configLayout->removeWidget(dynamic_cast<QWidget*>(configWidget));
		delete configWidget;
	}

	configWidget = transport->getConfigWidget();
	connect(configWidget, SIGNAL(configChanged()), this, SLOT(configUpdated()));
	ui->configLayout->addWidget(configWidget);
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
	qDebug() << "TransportConfig::loadTransportWithId" << id;
	Transport* newTransport = GnosticApp::getInstance().getTransportWithId(id);
	if (!newTransport)
	{
		qWarning() << "TransportConfig::loadTransportWithId failed to get a transport for id" << id;
		return;
	}
	else
	{
		qDebug() << "TransportConfig::loadTransportWithId dump:";
		newTransport->dumpDebug();
	}

	int typeIdx = ui->typeCombo->findText(newTransport->getType());
	if (typeIdx==-1)
	{
		qWarning() << "TransportConfig::loadTransportWithId failed to find type in combo" << newTransport->getType();
		return;
	}
	else
	{
		qDebug() << "TransportConfig::loadTransportWithId setting type combo idx" << typeIdx << "for" << newTransport->getType();
		ui->typeCombo->setCurrentIndex(typeIdx);
	}

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
	selectTransportWithId(id);
}

void TransportConfig::selectTransportWithId(const QString& id)
{
	foreach (QStandardItem* i, model->findItems(id))
	{
		ui->transportView->selectRow(i->row());
		break;
	}
	ui->saveButton->setEnabled(false);
}

void TransportConfig::createNewTransport()
{
	qDebug() << "TransportConfig::createNewTransport";
	SshTransport* newTransport = new SshTransport();
	QString id = newTransport->getId();
	newTransport->saveTransport();
	newTransport->dumpDebug();
	populateTransportModel();
	selectTransportWithId(id);
	ui->typeCombo->setCurrentIndex(ui->typeCombo->findText(newTransport->getType()));
	changeTransportType(newTransport->getType());
}

void TransportConfig::deleteCurrentTransport()
{
	qDebug() << "TransportConfig::deleteCurrentTransport";
	QString id = transport->getId();
	GnosticApp::getInstance().settings()->remove(id);
	populateTransportModel();
	ui->saveButton->setEnabled(false);
}

void TransportConfig::testCurrentTransport()
{
	qDebug() << "TransportConfig::testCurrentTransport";
	QMessageBox mb;
	if (!transport)
	{
		mb.setText("Hurp Durp - no transport was selected...");
		mb.setIcon(QMessageBox::Information);
	}
	else
	{
		if (transport->testTransport())
		{
			mb.setText("Looks good");
			mb.setIcon(QMessageBox::Information);
		}
		else
		{
			mb.setText("Something not right here");
			mb.setIcon(QMessageBox::Warning);
		}
	}

	mb.exec();

}
