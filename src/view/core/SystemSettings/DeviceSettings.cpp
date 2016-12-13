#include "DeviceSettings.h"
#include "ui_DeviceSettings.h"

DeviceSettings::DeviceSettings(RdsProtobuf::DeviceOptions device, QWidget *parent) :
	QWidget(parent),
	m_device(device),
	ui(new Ui::DeviceSettings)
{
	ui->setupUi(this);
	setupWidget();

	//connect(ui->pbClose, SIGNAL(clicked(bool)), this, SLOT(onClose()));
	connect(ui->pbAddChannel, SIGNAL(clicked(bool)), this, SLOT(onAddChannel()));
}

DeviceSettings::~DeviceSettings()
{
	delete ui;
}

RdsProtobuf::DeviceOptions DeviceSettings::getSettings()
{
	RdsProtobuf::DeviceOptions options;

	options.set_title(ui->leTitle->text().toStdString());
	options.set_status(ui->cbStatus);
	options.set_ip(ui->ipFrame->text().toStdString());
	options.set_sync(ui->cbSync->currentIndex());
	options.set_enabled(ui->cbEnabled);

	foreach (ChannelSettings* channel, m_channelsList) {
		options.add_channels()->CopyFrom( channel->getSettings() );
	}

	return options;
}

void DeviceSettings::clearWidget()
{
	foreach (ChannelSettings* channel, m_channelsList) {
		delete channel;
	}

	m_channelsList.clear();
}

void DeviceSettings::setupWidget()
{
	//Setup myself
	ui->leTitle->setText(QString::fromStdString(m_device.title()));
	ui->cbStatus->setEnabled(m_device.status());
	ui->ipFrame->setText(QString::fromStdString( m_device.ip() ));
	ui->cbSync->setCurrentIndex( m_device.sync() );
	ui->cbEnabled->setChecked(m_device.enabled());

	//Setup channels
	for(int i = 0; i < m_device.channels_size(); i++) {
		ChannelSettings* wgt = new ChannelSettings(m_device.channels(i), this);
		ui->channelLayout->addWidget(wgt);
		m_channelsList.append(wgt);

		connect(wgt, SIGNAL(onClose()), this, SLOT(onChannelClose()));
	}
}

void DeviceSettings::onChannelClose()
{
	QObject* obj = sender();
	ChannelSettings* channel = dynamic_cast<ChannelSettings*>(obj);

	if(!channel) {
		return;
	}

	if(m_channelsList.size() <= 1) {
		return;
	}

	m_channelsList.removeAll(channel);
	delete channel;
}

void DeviceSettings::onAddChannel()
{
	RdsProtobuf::ChannelOptions* opt = m_device.add_channels();
	createTmpChannelOptions(opt);

	clearWidget();
	setupWidget();

	emit channelAdded();
}
