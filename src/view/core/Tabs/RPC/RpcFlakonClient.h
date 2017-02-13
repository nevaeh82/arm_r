#ifndef RPCFLAKONCLIENT_H
#define RPCFLAKONCLIENT_H

#include <Rpc/RpcRoutedClient.h>
#include <QMutex>

#include "Interfaces/IStation.h"
#include "Interfaces/IRpcConfigClient.h"

class RpcFlakonClient : public RpcRoutedClient, public IRpcConfigClient
{
	Q_OBJECT
public:
	explicit RpcFlakonClient(QObject *parent = 0);

	void sendMainStationCorrelation(const int id, const QString& value);
	void sendBandwidth(const int id, const float bandwidth);
	void sendShift(const int id, const float shift);
	void sendCenter(const int id, const float center);
	void recognize(const int id, const int type);
	void sendCorrelation(const int id, const float frequency, const bool enable);
	void sendAvarageSpectrum(const int id, const int avarage);
	void sendEnableReceiver(const int id, const bool val);

	void sendSolverSettings(const QByteArray& data);

	void requestFlakonStatus();

	bool start(quint16 port, QHostAddress ipAddress);

	void clearReceiversList();
	QByteArray testData;
	//void sendWorkMode(const int mode, const bool isOn);
	void sendRdsProto(const QByteArray &data);
    void sendCPPacketProto(const QByteArray &data);
	void sendServerRequestSettings(const int &id);
	QMutex m_receiverMutex;


protected slots:
	void registerRoute();
	void pointsReceived(QByteArray data);
	void bandwidthReceived(QByteArray data);
	void correlationReceived(QByteArray data);
	void flakonStatusReceived(QByteArray data);

	void sloverResultReceived(QByteArray data);
	void sloverResultReceived1(QByteArray data);

signals:
	void signalEnableCorrelation(int, float, bool);
	void connectionEstablishedSignal();

private slots:
	void slotEnableCorrelation(int, float, bool);
	void solverQualityStatusReceived(QByteArray data);
	void solverErrorsReceived(QByteArray data);
	void sloverAnswerReceived1(QByteArray data);

	void sloverConnectState(bool state);

	void slotSettingsFromServer(QByteArray data);

public:
	virtual void requestGetStationList(const QString& filename);
	virtual void requestGetDbConfiguration(const QString& filename);

signals:
	void getStationListSignal(QString);
	//    void connectionEstablishedSignal();

private slots:
	void receivedStationListSlot(QByteArray data);
	void receivedDbConfigurationSlot(QByteArray data);
	void receivedLocSystem(QByteArray data);
	void receivedWorkMode(QByteArray data);

};

#endif // RPCFLAKONCLIENT_H
