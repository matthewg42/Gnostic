#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QDebug>

#include "RemoteCommandConfigWidget.hpp"
#include "ui_RemoteCommandConfigWidget.h"
#include "RemoteCommand.hpp"
#include "GnosticApp.hpp"
#include "DataDisplay.hpp"
#include "GnosticParser.hpp"

RemoteCommandConfigWidget::RemoteCommandConfigWidget(Transport* t, QWidget *parent) :
		QWidget(parent),
		ui(new Ui::RemoteCommandConfigWidget)
{
	Q_ASSERT(t);
	qDebug() << "RemoteCommandConfigWidget::RemoteCommandConfigWidget for" << t->getDescription();
	current = NULL;
	transport = t;
	ui->setupUi(this);

	connect(ui->commandTable, SIGNAL(clicked(QModelIndex)), this, SLOT(commandTableClicked(QModelIndex)));
	connect(ui->saveCommandButton, SIGNAL(clicked()), this, SLOT(saveCurrent()));
	connect(ui->addCommandButton, SIGNAL(clicked()), this, SLOT(addNewCommand()));
	connect(ui->removeCommandButton, SIGNAL(clicked()), this, SLOT(deleteCurrent()));
	connect(ui->testCommandButton, SIGNAL(clicked()), this, SLOT(testCurrent()));

	connect(ui->commandEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->timeoutSpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));
	connect(ui->reconnectCheck, SIGNAL(toggled(bool)), this, SLOT(madeUpdate()));
	connect(ui->retiesSpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));

	model.setHorizontalHeaderLabels(QStringList() << "Section" << "Remote Command Description");
	ui->commandTable->setModel(&model);
	ui->commandTable->horizontalHeader()->setStretchLastSection(true);

	populateTable();

	if (model.rowCount()>0)
	{
		ui->commandTable->selectRow(0);
		commandTableClicked(ui->commandTable->currentIndex());
	}
}

RemoteCommandConfigWidget::~RemoteCommandConfigWidget()
{
	delete ui;
}


void RemoteCommandConfigWidget::populateTable()
{
	qDebug() << "RemoteCommandConfigWidget::populateTable";
	model.clear();
        QString oldSelection;
        QModelIndexList selected = ui->commandTable->selectionModel()->selectedRows();
        if (selected.count()>0)
        {
                oldSelection = selected.at(0).data().toString();
        }

	QSettings* s = GnosticApp::getInstance().settings();
	foreach (QString section, RemoteCommand::getSections())
	{
		qDebug() << "RemoteCommandConfigWidget::populateTable considering section " << section;
		// TODO - find an API based way to do this so we don't have to duplicate the  ini setting...
		if (s->value(QString("%1/transport_id").arg(section), "").toString() == transport->getId())
		{
			qDebug() << "RemoteCommandConfigWidget::populateTable adding" << transport->getId() << section;
			QList<QStandardItem*> row;
			row << new QStandardItem(section)
			    << new QStandardItem(s->value(QString("%1/description").arg(section)).toString());

			model.appendRow(row);
		}
		else
		{
			qDebug() << "RemoteCommandConfigWidget::populateTable" << section << "isn't for this transport" << transport->getId();
		}
	}
	ui->saveCommandButton->setEnabled(false);
	ui->testCommandButton->setEnabled(true);
	ui->commandTable->hideColumn(0);
        selectRowWithId(oldSelection);
}

void RemoteCommandConfigWidget::clearCurrent()
{
	if (current) {
		ui->commandEdit->setText(QString());
		ui->timeoutSpin->setValue(60);
		ui->reconnectCheck->setChecked(true);
		ui->retiesSpin->setValue(3);
		setControlsEnabled(false);
		delete current;
		current = NULL;
	}
}

void RemoteCommandConfigWidget::setControlsEnabled(bool b)
{
	ui->commandEdit->setEnabled(b);
	ui->timeoutSpin->setEnabled(b);
	ui->reconnectCheck->setEnabled(b);
	ui->retiesSpin->setEnabled(b);
}

void RemoteCommandConfigWidget::setControlsFromCommand(RemoteCommand* c)
{
	if (c)
	{
		QString cmd = current->getProgram().trimmed() + " " + current->getArguments().join(" ").trimmed();
		ui->commandEdit->setText(cmd.trimmed());
		ui->timeoutSpin->setValue(current->getTimeout());
		ui->reconnectCheck->setChecked(current->getReconnect());
		ui->retiesSpin->setValue(current->getRetries());
		setControlsEnabled(true);
	}
	else
		this->clearCurrent();
}

void RemoteCommandConfigWidget::commandTableClicked(QModelIndex idx)
{
	selectCommand(model.index(idx.row(), 0).data().toString());
}

void RemoteCommandConfigWidget::selectCommand(const QString& section)
{
	clearCurrent();

	current = RemoteCommand::makeFromConfig(section);
	if (current)
	{
		setControlsFromCommand(current);
	}
	ui->saveCommandButton->setEnabled(false);
	ui->testCommandButton->setEnabled(true);

}

void RemoteCommandConfigWidget::selectRowWithId(const QString& id)
{
	QList<QStandardItem*> search = model.findItems(id);
	if (search.count() > 0)
	{
		ui->commandTable->selectRow(search.at(0)->row());
		commandTableClicked(ui->commandTable->currentIndex());
	}
        else if (model.rowCount()>0)
        {
                ui->commandTable->selectRow(0);
                commandTableClicked(ui->commandTable->currentIndex());
        }
}

void RemoteCommandConfigWidget::saveCurrent()
{
	if (current)
	{
		QStringList args = ui->commandEdit->text().split(" ");
		qDebug() << "RemoteCommandConfigWidget::saveCurrent args"  << args;
		QString prog;
		if (args.count() > 0)
		{
			qDebug() << "RemoteCommandConfigWidget::saveCurrent b4 pop prog" << prog << "args"  << args;
			prog = args.at(0);
			args.pop_front();
			qDebug() << "RemoteCommandConfigWidget::saveCurrent af pop prog" << prog << "args"  << args;
		}
		current->setProgram(prog.trimmed());
		current->setArguments(args);
		current->setTimeout(ui->timeoutSpin->value());
		current->setReconnect(ui->reconnectCheck->isChecked());
		current->setRetries(ui->retiesSpin->value());
		current->dumpDebug();
		current->saveSettings();
		ui->saveCommandButton->setEnabled(false);
		ui->testCommandButton->setEnabled(true);
		populateTable();
	}
}

void RemoteCommandConfigWidget::addNewCommand()
{
	// we need to prompt for the type of transport...
	clearCurrent();
	current = new RemoteCommand(this);
	current->setDescription("new command");
	current->setTransportId(transport->getId());
	current->saveSettings();
        QString id = current->getId();
	populateTable();
        selectRowWithId(id);
}

void RemoteCommandConfigWidget::deleteCurrent()
{
	if (current)
	{
		GnosticApp::getInstance().settings()->remove(current->getId());
		clearCurrent();
		populateTable();
                GnosticApp::getInstance().sendConfigUpdated(GnosticApp::Command);
	}
}

void RemoteCommandConfigWidget::testCurrent()
{
	if (!current)
		return;
	DataDisplay* display = DataDisplay::makeNew("TailerDisplay", 0);
	connect(current->getTransport(), SIGNAL(spewLine(QString)), display, SLOT(takeLine(QString)));
	connect(display, SIGNAL(wasClosed()), current->getTransport(), SLOT(stop()));
	display->show();
	current->start();
}

void RemoteCommandConfigWidget::madeUpdate()
{
	ui->saveCommandButton->setEnabled(true);
	ui->testCommandButton->setEnabled(false);

}
