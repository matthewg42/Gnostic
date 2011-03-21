#include "FirstRunWizard.hpp"
#include "ui_FirstRunWizard.h"

#include <QDebug>
#include <QFileDialog>

FirstRunWizard::FirstRunWizard(QWidget *parent) :
                QWizard(parent),
                ui(new Ui::FirstRunWizard)
{
        ui->setupUi(this);
        connect(ui->sampleRadio, SIGNAL(clicked()), this, SLOT(setSample()));
        connect(ui->emptyRadio, SIGNAL(clicked()), this, SLOT(setEmpty()));
        connect(ui->selectRadio, SIGNAL(clicked()), this, SLOT(setSelect()));
        connect(ui->iniBrowseButton, SIGNAL(clicked()), this, SLOT(browseForPath()));
        setSample();
}

FirstRunWizard::~FirstRunWizard()
{
        delete ui;
}

void FirstRunWizard::setSample()
{
        ui->iniBrowseButton->setEnabled(false);
        ui->iniPathEdit->setEnabled(false);
        iniPath = ":/data/sample_config.ini";
}

void FirstRunWizard::setEmpty()
{
        ui->iniBrowseButton->setEnabled(false);
        ui->iniPathEdit->setEnabled(false);
        iniPath = QString();
}

void FirstRunWizard::setSelect()
{
        ui->iniBrowseButton->setEnabled(true);
        ui->iniPathEdit->setEnabled(true);
        iniPath = ui->iniPathEdit->text();
}

void FirstRunWizard::pathEdited(QString p)
{
        iniPath = p;
}

void FirstRunWizard::browseForPath()
{
        ui->iniPathEdit->setText(QFileDialog::getOpenFileName(this, "Select a config.ini file to use", QString(), "ini files (*.ini)"));
        iniPath = ui->iniPathEdit->text();
}
