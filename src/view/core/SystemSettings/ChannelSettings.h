#ifndef CHANNELSETTINGS_H
#define CHANNELSETTINGS_H

#include <QWidget>
#include "RDSExchange.h"

namespace Ui {
class ChannelSettings;
}

class ChannelSettings : public QWidget
{
	Q_OBJECT

public:
	explicit ChannelSettings(RdsProtobuf::ChannelOptions channel, QWidget *parent = 0);
	~ChannelSettings();

	RdsProtobuf::ChannelOptions getSettings();

protected:
	void paintEvent(QPaintEvent *);
private:
	Ui::ChannelSettings *ui;

	RdsProtobuf::ChannelOptions m_device;

	void clearWidget();
	void setupWidget();

signals:
	void onClose();
};

#endif // CHANNELSETTINGS_H
