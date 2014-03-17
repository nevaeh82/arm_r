#ifndef TCPFLAKONCONTROLLER_H
#define TCPFLAKONCONTROLLER_H

#include <QSettings>
#include <QStringList>
#include "BaseClasses/BaseTcpDeviceController.h"
#include "TcpFlakonCoder.h"

#include "Info/FlakonSettings.h"

class TcpFlakonController : public BaseTcpDeviceController
{
	Q_OBJECT

public:
	explicit TcpFlakonController(QObject* parent = NULL);
	explicit TcpFlakonController(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpFlakonController();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

	virtual bool init();

signals:
	void createTcpFlakonCoderInternalSignal();

private slots:
	void createTcpFlakonCoderInternalSlot();

private:
	FlakonSettings m_flakonSettingStruct;
};

#endif // TCPFLAKONCONTROLLER_H
