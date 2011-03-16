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

        this->setWindowFlags(Qt::Tool);
	ui->styleCombo->insertItem(Qt::SolidLine, "Solid");
	ui->styleCombo->insertItem(Qt::DashLine, "Dash");
	ui->styleCombo->insertItem(Qt::DotLine, "Dot");
	connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(chooseColor()));
	connect(ui->widthSpin, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
	connect(ui->styleCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPenStyle(QString)));
}

PenStyleWidget::~PenStyleWidget()
{
	delete ui;
}

void PenStyleWidget::setInitialPen(QPen p)
{
	workingPen = p;
	oldPen = p;
        updatePreview();
}


void PenStyleWidget::chooseColor()
{
	QColor newColor = QColorDialog::getColor(workingPen.color(), this, "Choose color", QColorDialog::ShowAlphaChannel);
	workingPen.setBrush(newColor);
	saveStyle();
}

void PenStyleWidget::setWidth(int i)
{
	workingPen.setWidth(i);
	saveStyle();
}

void PenStyleWidget::setPenStyle(QString s)
{
	if (s=="Solid")
		workingPen.setStyle(Qt::SolidLine);
	else if (s=="Dash")
		workingPen.setStyle(Qt::DashLine);
	else if (s=="Dot")
		workingPen.setStyle(Qt::DotLine);

	saveStyle();

}

void PenStyleWidget::saveStyle()
{
	// this is called on accepted... it means the pen is OK  :)
	emit(setPen(workingPen));
        updatePreview();
	// this->close();
}

void PenStyleWidget::revertStyle()
{
	// this is called on rejected... we emit the original pen one last time and close.
	emit(setPen(oldPen));
	this->close();
}

void PenStyleWidget::updatePreview()
{
        QPixmap colPixmap(22,22);
        colPixmap.fill(workingPen.color());
        colorIcon = QIcon(QPixmap(colPixmap));
        ui->colorButton->setIcon(colorIcon);
}
