#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QTimer>

#include <Interfaces/IRpcListener.h>
#include <Interfaces/IRpcControllerBase.h>
#include <TcpDevicesDefines.h>
#include <Rpc/RpcDefines.h>
#include <Tcp/BaseTcpServer.h>

#include "Tcp/TcpDeviceController.h"
#include "Tcp/TcpFlakonController.h"
#include "Tcp/TcpAtlantController.h"
#include "Tcp/TcpPRM300Controller.h"
#include "Tcp/Interfaces/ITcpManager.h"
#include "Tcp/Interfaces/ITcpListener.h"

#include "Flakon/CoordinateCounter.h"

#include "Flakon/Server/PServer.h"
#include "Flakon/Server/ClientServer.h"


class TcpManager : public QObject, public ITcpManager, public ITcpListener, public IRpcListener
{
	Q_OBJECT

private:
	IRpcControllerBase* m_rpcServer;
	ITcpServerController* m_tcpServer;
	QMap< QString, TcpDeviceController* > m_controllersMap;

	TcpFlakonController *m_flakonController;
	QMap<QString, BaseTcpDeviceController*> m_flakonStations;
	QTimer m_timer;

	QTime m_timePoints;
	bool m_isCorrelAfterPoints;


	/**
	 * key = deviceType
	 * Define own deviceType in TcpDevicesDefines
	 *
	 **/

	CoordinateCounter* m_coordinatesCounter;
	PServer*		   m_pServer;
	ClientTcpServer* m_clientTcpServer;

	QMap<float, CoordinateCounter* > m_mapCoordinateCounter;

	float m_currentFrequencyCorrelation;

public:
	explicit TcpManager(QObject* parent = NULL);
	virtual ~TcpManager();

	// ITcpManager interface
	virtual void addTcpDevice(const QString& deviceName, const int& type);
	virtual void removeTcpDevice(const QString& deviceName);
	virtual void setRpcServer(IRpcControllerBase*);
	virtual	void setTcpServer(ITcpServerController*);
	virtual QObject* asQObject();

	/// TODO: add sendDataToDevice from some internal message

	// ITcpListener interface
	virtual void onMessageReceived(const quint32 type, const QString& deviceType, const MessageSP argument);

	// IRpcListener interface
	virtual void onMethodCalled(const QString& method, const QVariant& argument);

private:
	void addStationToFlakon(QString name, BaseTcpDeviceController* controller);
	void addSolver(QByteArray data);

signals:
	void threadTerminateSignal();
	void threadTerminateSignalForMapSolver();

	void onMethodCalledInternalSignal(const QString& method, const QVariant& argument);

private slots:
	void onMethodCalledInternalSlot(const QString &method, const QVariant &argument);
	void emulateBplaPoint(IRpcListener* sender = NULL);

};

#endif // TCPMANAGER_H
