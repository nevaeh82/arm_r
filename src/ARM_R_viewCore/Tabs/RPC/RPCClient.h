#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QxtRPCPeer>
#include <QAbstractSocket>

#include <QTextCodec>
#include <QSettings>
#include <QStringList>

#include <QHostAddress>
#include <QVariant>
#include <QList>

#include "IRPC.h"
#include "Common/IMessage.h"
#include "Common/CommandMessage.h"

#include "Tabs/TabsProperty.h"
#include "Interfaces/IDBManager.h"
#include "Tabs/ITabSpectrum.h"

#include "GraphicData.h"

#include "Tabs/Controls/IControlPRM.h"

#include "UiDefines.h"
#include "Rpc/RpcDefines.h"
#include "Rpc/RpcClientBase.h"

#include "Interfaces/IDbManager.h"

class RPCClient : public RpcClientBase
{
    Q_OBJECT
private:
	IControlPRM*	m_controlPrm;
	IMessage*		m_commandMsg;
	IDbManager*		m_dbManager;
	TabsProperty*	m_tabProperty;
	ITabSpectrum*	m_parentTab;
	GraphicData*	m_grData;

	float*	m_spectrum;
	float*	m_spectrumPeakHold;
	double	m_bandwidth;
	qreal	m_startx;
	bool	m_needSetup;

public:
	RPCClient(TabsProperty *prop, IDbManager *db_manager,
			  ITabSpectrum *parent_tab, GraphicData *gr_data,
			  IControlPRM* control_prm, QObject *parent = 0);
    ~RPCClient();

	void setCommand(IMessage* msg);

	bool start(quint16 port, QHostAddress ipAddress);

private:
	void formCommand(IMessage *msg);
	void recognize();
	void ssCorrelation(bool enable);

	void prmSetFreq(short freq);
	void prmRequestFreq();
	void prmSetAtt1(int att1);
	void prmSetAtt2(int att2);
	void prmSetFilter(int index);

	void flakonSetMainStationCor(int value);
	void flakonSetAvarage(int value);

	void requestSatatus();

signals:
    void signalSetCommand(IMessage *msg);
    void signalFinished();

    void signalStart();
    void signalStop();
    void signalFinishRPC();

	///RPC signals
    void signalSetClientId(int id);
    void signalSetMainStationCor(int id, int station);
    void signalSetBandwidth(int id, float db);
    void signalSetShift(int id, float shift);
    void signalRecognize(int id, int type);
    void signalSSCorrelation(int id, bool enable);
    void signalSetAvarageSpectrum(int id, int avarage);

    void signalPRMSetFreq(int id, short freq);
    void signalPRMRequestFreq(int id);
    void signalPRMSetAtt1(int id, int att1);
    void signalPRMSetAtt2(int id, int att2);
    void signalPRMSetFilter(int id, int index);

    void signalRequestStatus(int id);

    void signalReconnection();

	void signalDataS(float*, float*);
	void signalData(float*, float*);

public slots:
    ///rpc_server
	void rpcSlotGettingPoints(QByteArray points);
	void rpcSlotGettingDetectedBandwidth(QByteArray points);
	void rpcSlotGettingModulation(QString modulation);
	void rpcSlotServerSendCorrelation(uint point1, uint point2, QByteArray points);

	void rpcSlotServerPrmStatus(int prm_freq, int prm_filter, int prm_att1, int prm_att2);

	void rpcSlotServerStatus(bool state);

private slots:
	void slotRpcConnetion();
	//void slotErrorRPCConnection(QAbstractSocket::SocketError socketError);
	void slotSetCommand(IMessage* msg);
};

#endif // RPCCLIENT_H
