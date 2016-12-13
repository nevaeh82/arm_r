#include "ChannelSettings.h"
#include "ui_ChannelSettings.h"

#include <QPainter>

ChannelSettings::ChannelSettings(RdsProtobuf::ChannelOptions channel, QWidget *parent) :
	QWidget(parent),
	m_device(channel),
	ui(new Ui::ChannelSettings)
{
	ui->setupUi(this);

//	ui->sbAlt->setEnabled(false);
//	ui->sbLat->setEnabled(false);
//	ui->sbLon->setEnabled(false);

	connect(ui->pbClose, SIGNAL(clicked(bool)), this, SIGNAL(onClose()));

	setupWidget();
}

ChannelSettings::~ChannelSettings()
{
	delete ui;
}

RdsProtobuf::ChannelOptions ChannelSettings::getSettings()
{
	m_device.set_title(ui->leTitle->text().toStdString());
	m_device.mutable_receiver()->set_status(ui->cbStatus->isChecked());
	m_device.mutable_receiver()->set_ip(ui->ipFrame->text().toStdString());
	m_device.set_inversion(ui->cbInversion->isChecked());
	if( !m_device.has_coordinates() ) {
		m_device.mutable_coordinates();
	}

	m_device.mutable_coordinates()->set_altitude(ui->sbAlt->value());
	m_device.mutable_coordinates()->set_longitude(ui->sbLon->value());
	m_device.mutable_coordinates()->set_latitude(ui->sbLat->value());

	return m_device;
}

void ChannelSettings::clearWidget()
{

}

void ChannelSettings::setupWidget()
{
	//Setup myself
	ui->leTitle->setText(QString::fromStdString(m_device.title()));
	ui->cbStatus->setChecked(m_device.receiver().status());
	ui->ipFrame->setText(QString::fromStdString( m_device.receiver().ip() ));
	ui->cbInversion->setChecked( m_device.inversion() );

	if( m_device.has_coordinates() ) {

//		ui->sbAlt->setEnabled(true);
//		ui->sbLat->setEnabled(true);
//		ui->sbLon->setEnabled(true);

		ui->sbLat->setValue( m_device.coordinates().latitude() );
		ui->sbLon->setValue( m_device.coordinates().longitude() );
		ui->sbAlt->setValue( m_device.coordinates().altitude() );
	}
}

void ChannelSettings::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}
