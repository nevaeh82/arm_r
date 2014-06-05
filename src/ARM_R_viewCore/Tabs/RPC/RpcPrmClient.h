#ifndef PRMRPCCLIENT_H
#define PRMRPCCLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QAbstractSocket>
#include <QTextCodec>
#include <QSettings>
#include <QStringList>
#include <QHostAddress>
#include <QVariant>
#include <QList>

#include <Interfaces/IRpcControllerBase.h>
#include <Rpc/RpcRoutedClient.h>
#include <Rpc/RpcMessageStruct.h>
#include <Rpc/RpcDefines.h>

#include "Common/IMessage.h"
#include "Common/CommandMessage.h"

#include "Interfaces/IStation.h"
#include "Interfaces/IDBManager.h"

#include "Tabs/ITabSpectrum.h"
#include "Tabs/Controls/IControlPRM.h"

#include "UiDefines.h"

#define RPC_PRM_STATE_CHANGED	"PrmRpcClient::stateChanged"

class RpcPrmClient : public RpcRoutedClient
{
	Q_OBJECT
private:
	IMessage*		m_commandMsg;
	IDbManager*		m_dbManager;
	IStation*		m_station;
	ITabSpectrum*	m_parentTab;

	float*	m_spectrum;
	float*	m_spectrumPeakHold;
	double	m_bandwidth;
	qreal	m_startx;
	bool	m_needSetup;

public:
	RpcPrmClient(IStation *prop, IDbManager *db_manager, IControlPRM *, QObject *parent);
	~RpcPrmClient();

	void setCommand(IMessage* msg);

	bool start(quint16 port, QHostAddress ipAddress);

	void requestFrequency();

private:
	void formCommand(IMessage *msg);
	void recognize();
	void ssCorrelation(bool enable);

	void prmSetFreq(short freq);
	void prmRequestFreq();
	void prmSetAtt1(int att1);
	void prmSetAtt2(int att2);
	void prmSetFilter(int index);

	void flakonSetMainStationCor(QString value);
	void flakonSetAvarage(int value);

	void requestStatus();

signals:
	void signalSetCommand(IMessage *msg);
	void signalFinished();

	void signalStart();
	void signalStop();
	void signalFinishRPC();

	///RPC signals
	void signalPRMSetFreq(QString name, short freq);
	void signalPRMRequestFreq(QString name);
	void signalPRMSetAtt1(QString name, int att1);
	void signalPRMSetAtt2(QString name, int att2);
	void signalPRMSetFilter(QString name, int index);

	void signalRequestStatus(QString name);

	void signalDataS(float*, float*);
	void signalData(float*, float*);

public slots:
	///rpc_server
	void rpcSlotGettingModulation(QString modulation);
    void rpcSlotServerPrmStatus(QByteArray);
	void rpcSlotServerStatus(QByteArray message);
    void rpcSlotPRM300FrequencyChanged(QByteArray message);


private slots:
	void slotRpcConnetion();
	//void slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
	void slotSetCommand(IMessage* msg);
};

#endif // PRMRPCCLIENT_H
