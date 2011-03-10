#include "PenStyleWidget.hpp"
#include "ui_PenStyleWidget.h"
#include "GnosticApp.hpp"

#include <QPixmap>
#include <QIcon>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QColorDialog>
#include <QPainter>

PenStyleWidget::PenStyleWidget(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::PenStyleWidget)
{
	ui->setupUi(this);

	ui->styleCombo->insertItem(Qt::SolidLine, "Solid");
	ui->styleCombo->insertItem(Qt::DashLine, "Dash");
	ui->styleCombo->insertItem(Qt::DotLine, "Dot");
	connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(chooseColor()));
	connect(ui->widthSpin, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	connect(ui->styleCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPenStyle(QString)));
	//connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveStyle()));
	//connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(revertStyle()));
	scene = new QGraphicsScene(0,0,100,100,this);
	ui->sampleView->setScene(scene);
	ui->sampleView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->sampleView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->sampleView->fitInView(0,0,100,50);

	colorScene = new QGraphicsScene(0,0,4,1, this);
	ui->colorView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->colorView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->colorView->setScene(colorScene);
	QVector<QColor> recentColors = GnosticApp::getInstance().getRecentColors(8);
	for (int i=0; i<8; i++)
	{
		colorScene->addRect(i,0,i+1,1, QPen(), QBrush(recentColors.at(i)));
	}
	ui->colorView->fitInView(0,0,8,1, Qt::IgnoreAspectRatio);

	redrawSample();
}

PenStyleWidget::~PenStyleWidget()
{
	delete ui;
}

void PenStyleWidget::setInitialPen(QPen p)
{
	workingPen = p;
	oldPen = p;
}


void PenStyleWidget::chooseColor()
{
	QColor newColor = QColorDialog::getColor(workingPen.color(), this, "Choose color", QColorDialog::ShowAlphaChannel);
	workingPen.setBrush(newColor);
	redrawSample();
}

void PenStyleWidget::setWidth(int i)
{
	workingPen.setWidth(i);
	redrawSample();
}

void PenStyleWidget::setPenStyle(QString s)
{
	if (s=="Solid")
		workingPen.setStyle(Qt::SolidLine);
	else if (s=="Dash")
		workingPen.setStyle(Qt::DashLine);
	else if (s=="Dot")
		workingPen.setStyle(Qt::DotLine);

	redrawSample();
}

void PenStyleWidget::saveStyle()
{
	// this is called on accepted... it means the pen is OK  :)
	emit(setPen(workingPen));
	this->close();
}

void PenStyleWidget::revertStyle()
{
	// this is called on rejected... we emit the original pen one last time and close.
	emit(setPen(oldPen));
	this->close();
}

void PenStyleWidget::redrawSample()
{
	scene->clear();
	double x = 10;
	QVector<double> ys;
	ys << 20 << 25 << 60 << 80 << 50 << 60 << 30 << 10;
	for(int i=0; i<ys.size()-1; i++)
	{
		scene->addLine(x, ys.at(i), x+10, ys.at(i+1),workingPen);
		x+=10;
	}
	emit(setPen(workingPen));
}


