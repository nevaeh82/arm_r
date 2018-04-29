#pragma once

#include <QObject>
#include <QTimer>
#include <QDir>
#include <QMessageBox>
#include "netpingalarm.h"

#define PATH "C:/ProgramData/Paessler/PRTG Network Monitor/Syslog Database/sensor 2199/"
//#define PATH "C:/a/"

class NetPingDetector : public QObject
{
	Q_OBJECT

public:
	NetPingDetector(QObject* parent = 0);
	~NetPingDetector();

private:
	QTimer* m_readTimer;

	int indexYes;
	int indexNo;

	QString nameLast;

	netpingalarm* alarmW;

private slots:
	void searchForAlarm();

signals:
	void onSendAlarm(QString);
};


