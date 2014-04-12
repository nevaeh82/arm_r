#include "Prm300ControlWidget.h"
#include "ui_Prm300ControlWidget.h"

Prm300ControlWidget::Prm300ControlWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Prm300ControlWidget)
{
	ui->setupUi(this);
	connect(ui->pbAtt1Up, SIGNAL(clicked()), this, SLOT(slotAtt1Up()));
	connect(ui->pbAtt1Down, SIGNAL(clicked()), this, SLOT(slotAtt1Down()));
	connect(ui->pbAtt2Up, SIGNAL(clicked()), this, SLOT(slotAtt2Up()));
	connect(ui->pbAtt2Down, SIGNAL(clicked()), this, SLOT(slotAtt2Down()));
	connect(ui->cbFilter, SIGNAL(activated(int)), this, SIGNAL(signalSetFilter(int)));
}

Prm300ControlWidget::~Prm300ControlWidget()
{
	delete ui;
}

void Prm300ControlWidget::setData(int freq, int filter, int att1, int att2)
{
	Q_UNUSED(freq);

	switch(att1)
	{
		case 0:
			att1 = 0;
			break;
		case 1:
			att1 = 10;
			break;
		default:
			att1 = -1;
			break;
	}

	ui->lcdAtt1->display(att1);
	ui->lcdAtt2->display(att2);
	ui->cbFilter->setCurrentIndex(filter);
}

void Prm300ControlWidget::slotAtt1Up()
{
	if(ui->lcdAtt1->value() == 10)
	{
		return;
	}
	int value = 10;
	int aval = 1;
	ui->lcdAtt1->display(value);
	emit signalSetAtt1Value(aval);
}

void Prm300ControlWidget::slotAtt1Down()
{
	if(ui->lcdAtt1->value() == 0)
	{
		return;
	}
	int value = 0;
	ui->lcdAtt1->display(value);
	emit signalSetAtt1Value(value);
}

void Prm300ControlWidget::slotAtt2Up()
{
	if(ui->lcdAtt2->value() == 15)
	{
		return;
	}
	int value = ui->lcdAtt2->value();
	value++;
	ui->lcdAtt2->display(value);
	emit signalSetAtt2Value(value);
}

void Prm300ControlWidget::slotAtt2Down()
{
	if(ui->lcdAtt2->value() == 0)
	{
		return;
	}
	int value = ui->lcdAtt2->value();
	value--;
	ui->lcdAtt2->display(value);
	emit signalSetAtt2Value(value);
}
