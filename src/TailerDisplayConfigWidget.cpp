#include "TailerDisplayConfigWidget.hpp"
#include "ui_TailerDisplayConfigWidget.h"
#include "TailerDisplay.hpp"

#include <QColorDialog>
#include <QPixmap>
#include <QDebug>
#include <QWidget>

TailerDisplayConfigWidget::TailerDisplayConfigWidget(DataDisplay* d, QWidget *parent) :
		DataDisplayConfigWidget(d, parent),
		ui(new Ui::TailerDisplayConfigWidget),
		populating(false)
{
	ui->setupUi(this);
	myTailer = static_cast<TailerDisplay*>(d);
	ui->descriptionEdit->setText(myTailer->getDescription());
	ui->historySpin->setValue(myTailer->getHistory());

	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), myTailer, SLOT(setDescription(QString)));
	connect(ui->descriptionEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), myTailer, SLOT(setHistory(int)));
	connect(ui->historySpin, SIGNAL(valueChanged(int)), this, SLOT(madeUpdate()));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addPattern()));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(madeUpdate()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removePattern()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(madeUpdate()));
	connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(selectNewColor()));
	connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(madeUpdate()));
	connect(ui->patternTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(patternSelectionChanged()));
	connect(ui->patternTableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(patternChanged(int,int)));

	ui->patternTableWidget->horizontalHeader()->setStretchLastSection(true);
	populateTable();
}

TailerDisplayConfigWidget::~TailerDisplayConfigWidget()
{
	delete ui;
}

void TailerDisplayConfigWidget::populateTable()
{
	populating = true;
	qDebug() << "TailerDisplayConfigWidget::populateTable dumping myTailer:";
	// myTailer->dumpDebug();
	QString oldSelection;
	if (ui->patternTableWidget->selectedItems().count()>0)
	{
		oldSelection = ui->patternTableWidget->selectedItems().at(0)->text();
	}

	ui->patternTableWidget->clear();
	ui->patternTableWidget->setColumnCount(1);
	ui->patternTableWidget->setRowCount(myTailer->getHighlights().keys().count());
	for(int i=0; i<myTailer->getHighlights().keys().count(); i++)
	{
		QTableWidgetItem* item = new QTableWidgetItem(myTailer->getHighlights().keys().at(i));
		ui->patternTableWidget->setItem(i, 0, item);
	}

	if (ui->patternTableWidget->findItems(oldSelection, Qt::MatchExactly).count()>0)
	{
		ui->patternTableWidget->findItems(oldSelection, Qt::MatchExactly).at(0)->setSelected(true);
	}
	else if (ui->patternTableWidget->model()->rowCount()>0)
	{
		ui->patternTableWidget->selectRow(0);
	}
	populating=false;
}

void TailerDisplayConfigWidget::selectNewColor()
{
	if (ui->patternTableWidget->selectedItems().count()<=0)
		return;

	QColor newColor = QColorDialog::getColor(Qt::black, this, "Choose color");
	QString pattern = ui->patternTableWidget->selectedItems().at(0)->text();
	myTailer->addHighlight(pattern, newColor);
	patternSelected(pattern);
}

void TailerDisplayConfigWidget::patternSelectionChanged()
{
	if (ui->patternTableWidget->selectedItems().count() > 0)
	{
		patternSelected(ui->patternTableWidget->selectedItems().at(0)->text());
	}
}

void TailerDisplayConfigWidget::patternSelected(const QString& pattern)
{
	if(!myTailer->getHighlights().contains(pattern))
		return;

	currentPattern = pattern;

	QColor c = myTailer->getHighlights()[pattern];
	QPixmap colPixmap(60,22);
	colPixmap.fill(c);
	colorIcon = QIcon(QPixmap(colPixmap));
	ui->colorButton->setIcon(colorIcon);
}

void TailerDisplayConfigWidget::addPattern()
{
	qDebug() << "TailerDisplayConfigWidget::addPattern ++++";
	myTailer->addHighlight("new pattern", QColor(Qt::black));
	populateTable();
}

void TailerDisplayConfigWidget::removePattern()
{
	qDebug() << "TailerDisplayConfigWidget::removePattern";
	foreach(QTableWidgetItem* i, ui->patternTableWidget->selectedItems())
	{
		qDebug() << "removing" << i->text();
		myTailer->deleteHighlight(i->text());
	}

	populateTable();
}

void TailerDisplayConfigWidget::patternChanged(int r, int c)
{
	if(populating)
		return;

	qDebug() << "TailerDisplayConfigWidget::patternChanged" << r << c;
	if (ui->patternTableWidget->item(r, 0))
	{
		QString np = ui->patternTableWidget->item(r, 0)->text();
		if (np != currentPattern)
		{
			updatePattern(currentPattern, np);
		}
	}
}

void TailerDisplayConfigWidget::updatePattern(const QString& oldPattern, const QString& newPattern)
{
	qDebug() << "TailerDisplayConfigWidget::updatePattern" << oldPattern << newPattern;
	QColor col;
	if (!myTailer->getHighlights().contains(oldPattern))
		return;

	col = myTailer->getHighlights()[oldPattern];
	myTailer->deleteHighlight(oldPattern);
	myTailer->addHighlight(newPattern, col);
	populateTable();
}

