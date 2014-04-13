#ifndef RPCCLIENTWRAPPER_H
#define RPCCLIENTWRAPPER_H

#include <QObject>

#include "RpcFlakonClient.h"

class RpcFlakonClientWrapper : public QObject
{
	Q_OBJECT
private:
	quint16 m_port;
	QHostAddress m_address;
	RpcFlakonClient* m_rpcClient;

public:
	explicit RpcFlakonClientWrapper(QObject *parent = 0);
	~RpcFlakonClientWrapper();
	void init(quint16, QHostAddress&);
	void stop();

	void registerReceiver(IRpcListener* listener);
	void deregisterReceiver(IRpcListener* listener);

	void sendMainStationCorrelation(const int id, const QString& value);
	void sendBandwidth(const int id, const float bandwidth);
	void sendShift(const int id, const float shift);
	void recognize(const int id, const int type);
	void sendCorrelation(const int id, const bool enable);
	void sendAvarageSpectrum(const int id, const int avarage);

signals:
	void initSignal();
	void stopSignal();

	void sendMainStationCorrelationSignal(int id, QString value);
	void sendBandwidthSignal(int id, float bandwidth);
	void sendShiftSignal(int id, float shift);
	void recognizeSignal(int id, int type);
	void sendCorrelationSignal(int id, bool enable);
	void sendAvarageSpectrumSignal(int id, int avarage);

private slots:
	void initSlot();
	void stopSlot();

	void sendMainStationCorrelationSlot(int id, QString value);
	void sendBandwidthSlot(int id, float bandwidth);
	void sendShiftSlot(int id, float shift);
	void recognizeSlot(int id, int type);
	void sendCorrelationSlot(int id, bool enable);
	void sendAvarageSpectrumSlot(int id, int avarage);
	
};

#endif // RPCCLIENTWRAPPER_H
