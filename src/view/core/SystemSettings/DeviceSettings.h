#ifndef DEVICESETTINGS_H
#define DEVICESETTINGS_H

#include <QWidget>

#include "RDSExchange.h"

#include "Common/IpAddressControl.h"
#include "ChannelSettings.h"

namespace Ui {
class DeviceSettings;
}

class DeviceSettings : public QWidget
{
	Q_OBJECT

public:
	explicit DeviceSettings(RdsProtobuf::DeviceOptions device, QWidget *parent = 0);
	~DeviceSettings();

	RdsProtobuf::DeviceOptions getSettings();

private:
	Ui::DeviceSettings *ui;
	QList<ChannelSettings*> m_channelsList;

	RdsProtobuf::DeviceOptions m_device;

	void clearWidget();
	void setupWidget();

private slots:
	void onChannelClose();

public slots:
	void onAddChannel();

signals:
	void onClose();
	void channelAdded();
};

#endif // DEVICESETTINGS_H
