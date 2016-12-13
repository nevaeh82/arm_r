#include "SystemSettingsWidget.h"
#include "ui_SystemSettingsWidget.h"

SystemSettingsWidget::SystemSettingsWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SystemSettingsWidget)
{
	ui->setupUi(this);

	connect(ui->pbApply, SIGNAL(clicked(bool)), this, SLOT(applySettings()));

	connect(ui->pbAddDev, SIGNAL(clicked(bool)), this, SLOT(addDevice()));
}

SystemSettingsWidget::~SystemSettingsWidget()
{
	clearWidget();
	delete ui;
}

void SystemSettingsWidget::setSettings(const RdsProtobuf::System_SystemOptions &options)
{
	m_options = options;
	ui->lblSystemTitle->setText(QString::fromStdString(m_options.title()));

	clearWidget();
	setupWidget();
}

RdsProtobuf::System_SystemOptions SystemSettingsWidget::getSettings()
{
	RdsProtobuf::System_SystemOptions settings;
	settings.set_title(ui->lblSystemTitle->text().toStdString());

	foreach (DeviceSettings* device, m_deviceList) {
		settings.add_devices()->CopyFrom(device->getSettings());
	}

	return settings;
}

void SystemSettingsWidget::clearWidget()
{
	foreach (DeviceSettings* wgt, m_deviceList) {
		//ui->deviceLayout->removeWidget(wgt);
		delete wgt;
	}

	m_deviceList.clear();
}

void SystemSettingsWidget::setupWidget()
{
	int tmp =  m_options.devices_size();
	for(int i = 0; i < m_options.devices_size(); i++) {
		DeviceSettings* deviceWidget = new DeviceSettings(m_options.devices(i), this);
		ui->deviceLayout->addWidget(deviceWidget);
		m_deviceList.append(deviceWidget);

		connect(deviceWidget, SIGNAL(onClose()), this, SLOT(onDeviceClose()));
		connect(deviceWidget, SIGNAL(channelAdded()), this, SLOT(updateAll()));
	}
}

void SystemSettingsWidget::onDeviceClose()
{
	if(m_deviceList.size() <= 1) {
		return;
	}

	QObject* obj = sender();
	DeviceSettings* device = dynamic_cast<DeviceSettings*>(obj);

	if(!device) {
		return;
	}

	m_deviceList.removeAll(device);
	delete device;
}

void SystemSettingsWidget::applySettings()
{
	RdsProtobuf::Packet pkt;
	createSetSystemSystemOptions(pkt, getSettings());

	emit sendRdsData( pack(pkt) );
}

void SystemSettingsWidget::addDevice()
{
	RdsProtobuf::DeviceOptions* dev = m_options.add_devices();
	createTmpDeviceOptions(dev);

	clearWidget();
	setupWidget();
}

void SystemSettingsWidget::updateAll()
{
	m_options = getSettings();
	ui->lblSystemTitle->setText(QString::fromStdString(m_options.title()));

	clearWidget();
	setupWidget();
}
