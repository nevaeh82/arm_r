#ifndef TCPATLANTCONTROLLER_H
#define TCPATLANTCONTROLLER_H

#include <QSettings>
#include <QStringList>
#include "BaseClasses/BaseTcpDeviceController.h"
#include "TcpAtlantCoder.h"

#include <Info/AtlantSettings.h>

class TcpAtlantController : public BaseTcpDeviceController
{
	Q_OBJECT
public:
	explicit TcpAtlantController(QObject* parent = NULL);
	explicit TcpAtlantController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpAtlantController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

	virtual bool init();

	virtual QByteArray getFullInfo();


signals:
	void createTcpAtlantCoderInternalSignal();

private slots:
	void createTcpAtlantCoderInternalSlot();

private:
	AtlantSettings m_atlantSettingStruct;
};

#endif // TCPATLANTCONTROLLER_H
