#ifndef SYSTEMSETTINGSWIDGET_H
#define SYSTEMSETTINGSWIDGET_H

#include <QWidget>

#include "RDSExchange.h"

#include "ChannelSettings.h"
#include "DeviceSettings.h"

namespace Ui {
class SystemSettingsWidget;
}

class SystemSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SystemSettingsWidget(QWidget *parent = 0);
	~SystemSettingsWidget();

	void setSettings(const RdsProtobuf::System_SystemOptions& options);
	RdsProtobuf::System_SystemOptions getSettings();

private:
	Ui::SystemSettingsWidget *ui;
	QList<DeviceSettings*> m_deviceList;

	RdsProtobuf::System_SystemOptions m_options;

	void clearWidget();
	void setupWidget();

private slots:
	void onDeviceClose();

	void applySettings();
	void addDevice();

public slots:
	void updateAll();

signals:
	void sendRdsData(QByteArray);
};

#endif // SYSTEMSETTINGSWIDGET_H
