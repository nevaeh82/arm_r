#include "netpingalarm.h"
#include "ui_netpingalarm.h"

netpingalarm::netpingalarm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::netpingalarm)
{
	ui->setupUi(this);

	this->setWindowFlags(Qt::WindowStaysOnTopHint);
}

netpingalarm::~netpingalarm()
{
	delete ui;
}

void netpingalarm::setText(QString str)
{
	ui->label->setText(str);
}


