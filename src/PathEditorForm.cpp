#include "PathEditorForm.hpp"
#include "ui_PathEditorForm.h"

#include <QSettings>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>
#include <QDir>

#include "GnosticApp.hpp"

PathEditorForm::PathEditorForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::PathEditorForm)
{
	ui->setupUi(this);

	ui->plinkPathEdit->setObjectName("plink");
	ui->sshPathEdit->setObjectName("ssh");
	ui->sshAskpassPathEdit->setObjectName("ssh-askpass");
	loadPathsFromConfig();

	connect(ui->plinkPathEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->sshPathEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->sshAskpassPathEdit, SIGNAL(textEdited(QString)), this, SLOT(madeUpdate()));
	connect(ui->plinkPathButton, SIGNAL(clicked()), this, SLOT(browseForPath()));
	connect(ui->sshPathButton, SIGNAL(clicked()), this, SLOT(browseForPath()));
	connect(ui->sshAskpassPathButton, SIGNAL(clicked()), this, SLOT(browseForPath()));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(savePaths()));
}

PathEditorForm::~PathEditorForm()
{
	delete ui;
}

void PathEditorForm::loadPathsFromConfig()
{
	ui->sshPathEdit->setText(getPath("ssh"));
	ui->plinkPathEdit->setText(getPath("plink"));
	ui->sshAskpassPathEdit->setText(getPath("ssh-askpass"));
	ui->saveButton->setEnabled(false);
}

void PathEditorForm::madeUpdate()
{
	ui->saveButton->setEnabled(true);
}

void PathEditorForm::savePaths()
{
	setPath(ui->plinkPathEdit->text(), "plink");
	setPath(ui->sshPathEdit->text(), "ssh");
	setPath(ui->sshAskpassPathEdit->text(), "ssh-askpass");
	ui->saveButton->setEnabled(false);
}

bool PathEditorForm::setPath(const QString &path, const QString& program)
{
	QString prog = program;
	if (program.isEmpty())
		prog = sender()->objectName();
	if (prog.isEmpty())
		return false;

	GnosticApp::getInstance().settings()->setValue(QString("paths/%1").arg(prog), path);
	return true;
}

bool PathEditorForm::browseForPath(const QString& program)
{
	QString prog = program;
	if (program.isEmpty())
		prog = sender()->objectName();
	if (prog.isEmpty())
		return false;

	QString defDir = QFileInfo(getPath(program)).dir().path();

	if (defDir.isEmpty())
		defDir = GnosticApp::getInstance().getInstallationDir();

	QFileDialog fd;
	QString p = fd.getOpenFileName(this, QString("Please locate %1").arg(program), defDir);
	if (!p.isEmpty())
	{
		setPath(p, prog);
		loadPathsFromConfig();
		return true;
	}
	else
	{
		return false;
	}
}

const QString PathEditorForm::getPath(const QString& program)
{
	return GnosticApp::getInstance().settings()->value(QString("paths/%1").arg(program), "").toString();
}

