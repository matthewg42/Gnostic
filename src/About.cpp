#include "About.hpp"
#include "ui_About.h"

#include "config.hpp"

About::About(QWidget *parent) :
                QWidget(parent),
                ui(new Ui::About)
{
        ui->setupUi(this);
        QString html = ui->textBrowser->document()->toHtml();
        html.replace("__VERSION__", PROG_VERSION);
        ui->textBrowser->document()->setHtml(html);
}

About::~About()
{
        delete ui;
}
