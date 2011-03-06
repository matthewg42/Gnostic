#include "TransportConfig.hpp"
#include "ui_TransportConfig.h"

TransportConfig::TransportConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransportConfig)
{
    ui->setupUi(this);
}

TransportConfig::~TransportConfig()
{
    delete ui;
}
