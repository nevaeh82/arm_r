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
#include "Tcp/TcpPRM300Controller.h"
#include "Tcp/TcpRDSController.h"
#include "Tcp/TcpRdsSettingsController.h"
#include "Tcp/Interfaces/ITcpManager.h"
#include "Tcp/Interfaces/ITcpListener.h"
#include "Rpc/RpcConfigReader.h"

#include "Flakon/CoordinateCounter.h"

#include "Flakon/Server/PServer.h"
#include "Flakon/Server/ClientServer.h"

#include "Flakon/Server/SolverClient1.h"


class TcpManager : public QObject, public ITcpManager, public ITcpListener, public IRpcListener
{
	Q_OBJECT

private:
    int m_serverId;
	IRpcControllerBase* m_rpcServer;
	ITcpServerController* m_tcpServer;
	QMap< QString, TcpDeviceController* > m_controllersMap;

	TcpFlakonController *m_flakonController;
	TcpRDSController *m_rdsController;
    TcpRDSSettingsController *m_rdsSettingsController;

	RpcConfigReader* m_rpcConfigReader;

	QMap<QString, BaseTcpDeviceController*> m_flakonStations;
	QTimer m_timer;

	QTime m_timePoints;
	bool m_isCorrelAfterPoints;

	bool m_solverConnectionState;

	QTimer m_sendTimer;


	/**
	 * key = deviceType
	 * Define own deviceType in TcpDevicesDefines
	 *
	 **/

	CoordinateCounter* m_coordinatesCounter;
	PServer*		   m_pServer;
	ClientTcpServer* m_clientTcpServer;
	SolverClient1* m_clientSolver;

	QMap<float, CoordinateCounter* > m_mapCoordinateCounter;

	float m_currentFrequencyCorrelation;

public:
    explicit TcpManager(int serverId, QObject* parent = NULL);
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

	void setConfigReader(RpcConfigReader* reader);

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

	void slotSolverConnectionStatus(int status);
	void slotRpcClientConnected();
	void slotSendSolverStatus();

	void slotNiipData(QString title, bool on);

};

#endif // TCPMANAGER_H
