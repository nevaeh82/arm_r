#ifndef TCPPRM300CONTROLLER_H
#define TCPPRM300CONTROLLER_H

#include <QSettings>
#include <QStringList>
#include "BaseClasses/BaseTcpDeviceController.h"
#include "TcpPRM300Coder.h"

#include <Info/Prm300Settings.h>

class TcpPRM300Controller : public BaseTcpDeviceController
{
	Q_OBJECT
public:
	explicit TcpPRM300Controller(QObject* parent = NULL);
	explicit TcpPRM300Controller(const QString& tcpDeviceName, QObject* parent = NULL);
	virtual ~TcpPRM300Controller();

	virtual bool init();

	// ITcpDeviceController interface
public:
	virtual void createTcpDeviceCoder();
	virtual QObject* asQObject();

	virtual QByteArray getFullInfo();

signals:
	void createTcpPRM300CoderInternalSignal();

private slots:
	void createTcpPRM300CoderInternalSlot();
	void slotTcpConnectionStatus(int status);

private:
	Prm300Settings prmSettings;
};

#endif // TCPPRM300CONTROLLER_H
