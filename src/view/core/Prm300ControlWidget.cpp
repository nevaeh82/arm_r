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
	connect(ui->cbFilter, SIGNAL(activated(int)), this, SLOT(setPrmParams()));

    connect(ui->cbEnable, SIGNAL(toggled(bool)), this, SIGNAL(signalOnEnableReceiver(bool)));
}

Prm300ControlWidget::~Prm300ControlWidget()
{
	delete ui;
}

void Prm300ControlWidget::setData(quint16 freq, quint8 filter, quint8 att1, quint8 att2)
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

	m_params.att1 = att1;
	m_params.att2 = att2;
	m_params.filter = filter;
}

Prm Prm300ControlWidget::getPrmParams()
{
	return m_params;
}

void Prm300ControlWidget::setState(bool state)
{
	ui->cbEnable->setChecked(state);
}

void Prm300ControlWidget::setPrmParams()
{
	if(ui->lcdAtt1->value()) {
		m_params.att1 = 1;
	} else {
		m_params.att1 = 0;
	}

	m_params.att2 = ui->lcdAtt2->value();
	m_params.filter = ui->cbFilter->currentIndex();

	emit signalOnSetParams();
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
	setPrmParams();
	//emit signalSetAtt1Value(aval);
}

void Prm300ControlWidget::slotAtt1Down()
{
	if(ui->lcdAtt1->value() == 0)
	{
		return;
	}
	int value = 0;
	ui->lcdAtt1->display(value);
	setPrmParams();
	//emit signalSetAtt1Value(value);
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

	setPrmParams();
	//emit signalSetAtt2Value(value);
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

	setPrmParams();
	//emit signalSetAtt2Value(value);
}
