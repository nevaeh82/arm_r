#include "RpcFlakonClientWrapper.h"

#include <QEventLoop>

RpcFlakonClientWrapper::RpcFlakonClientWrapper(QObject *parent) :
	QObject(parent)
{
	m_rpcClient = NULL;

	connect(this, SIGNAL(initSignal()), this, SLOT(initSlot()));
	connect(this, SIGNAL(stopSignal()), this, SLOT(stopSlot()));

	connect(this, SIGNAL(sendMainStationCorrelationSignal(int, QString )), this, SLOT(sendMainStationCorrelationSlot(int, QString)));
	connect(this, SIGNAL(sendBandwidthSignal(int, float)), this, SLOT(sendBandwidthSlot(int, float)));
	connect(this, SIGNAL(sendShiftSignal(int, float)), this, SLOT(sendShiftSlot(int, float)));
	connect(this, SIGNAL(sendCenterSignal(int,float)), this, SLOT(sendCenterSlot(int,float)));
	connect(this, SIGNAL(recognizeSignal(int, int)), this, SLOT(recognizeSlot(int, int)));
	connect(this, SIGNAL(sendCorrelationSignal(int, float, bool)), this, SLOT(sendCorrelationSlot(int, float, bool)));

	connect(this, SIGNAL(sendAvarageSpectrumSignal(int, int)), this, SLOT(sendAvarageSpectrumSlot(int, int)));
	connect(this, SIGNAL(sendEnableReceiverSignal(int,bool)), this, SLOT(sendEnableReceiverSlot(int, bool)));

	connect(this, SIGNAL(requestFlakonStatusSignal()), this, SLOT(requestFlakonStatusSlot()));

	connect(this, SIGNAL(sendWorkModeSignal(int, bool)), this, SLOT(sendWorkModeSlot(int, bool)));

	connect(this, SIGNAL(sendSolverSetupSettingsSignal(QByteArray)), this, SLOT(sendSolverSetupSettingsSlot(QByteArray)));
	connect(this, SIGNAL(sendRdsProtoSignal(QByteArray)), this, SLOT(sendRdsProtoSlot(QByteArray)));
}

RpcFlakonClientWrapper::~RpcFlakonClientWrapper()
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->stop();
}

void RpcFlakonClientWrapper::init(quint16 port, QHostAddress& address)
{
	m_address = address;
	m_port = port;

	emit initSignal();

	QEventLoop loop;
	connect(this, SIGNAL(initFinishedSignal()), &loop, SLOT(quit()));
	loop.exec();
}


void RpcFlakonClientWrapper::stop()
{
	emit stopSignal();
}

void RpcFlakonClientWrapper::registerReceiver(IRpcListener *listener)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->registerReceiver(listener);
}

void RpcFlakonClientWrapper::deregisterReceiver(IRpcListener *listener)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->deregisterReceiver(listener);
}


void RpcFlakonClientWrapper::initSlot()
{
	m_rpcClient = new RpcFlakonClient(this);
	m_rpcClient->start(m_port, m_address);

	emit initFinishedSignal();
}

void RpcFlakonClientWrapper::stopSlot()
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->stop();
}

void RpcFlakonClientWrapper::sendMainStationCorrelationSlot(int id, QString value)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendMainStationCorrelation(id, value);
}

void RpcFlakonClientWrapper::sendBandwidthSlot(int id, float bandwidth)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendBandwidth(id, bandwidth);
}

void RpcFlakonClientWrapper::sendShiftSlot(int id, float shift)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendShift(id, shift);
}

void RpcFlakonClientWrapper::sendCenterSlot(int id, float center)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendCenter(id, center);
}

void RpcFlakonClientWrapper::recognizeSlot(int id, int type)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->recognize(id, type);
}

void RpcFlakonClientWrapper::sendCorrelationSlot(int id, float frequency, bool enable)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendCorrelation(id, frequency, enable);
}

void RpcFlakonClientWrapper::sendAvarageSpectrumSlot(int id, int avarage)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendAvarageSpectrum(id, avarage);
}

void RpcFlakonClientWrapper::sendEnableReceiverSlot(int id, bool val)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendEnableReceiver(id, val);
}

void RpcFlakonClientWrapper::sendSolverSetupSettingsSlot(QByteArray data)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendSolverSettings( data );
}

void RpcFlakonClientWrapper::sendRdsProtoSlot(QByteArray data)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendRdsProto( data );
}

void RpcFlakonClientWrapper::sendWorkModeSlot(int mode, bool isOn)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendWorkMode(mode, isOn);
}

void RpcFlakonClientWrapper::requestFlakonStatusSlot()
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->requestFlakonStatus();
}


void RpcFlakonClientWrapper::sendMainStationCorrelation(const int id, const QString &value)
{
	emit sendMainStationCorrelationSignal(id, value);
}

void RpcFlakonClientWrapper::sendBandwidth(const int id, const float bandwidth)
{
	emit sendBandwidthSignal(id, bandwidth);
}

void RpcFlakonClientWrapper::sendShift(const int id, const float shift)
{
	emit sendShiftSignal(id, shift);
}

void RpcFlakonClientWrapper::sendCenter(const int id, const float center)
{
	emit sendCenterSignal(id, center);
}

void RpcFlakonClientWrapper::recognize(const int id, const int type)
{
	emit recognizeSignal(id, type);
}

void RpcFlakonClientWrapper::sendCorrelation(const int id, const float frequency, const bool enable)
{
	emit sendCorrelationSignal(id, frequency, enable);
}

void RpcFlakonClientWrapper::sendAvarageSpectrum(const int id, const int avarage)
{
	emit sendAvarageSpectrumSignal(id, avarage);
}

void RpcFlakonClientWrapper::sendEnableReceiver(const int id, const bool val)
{
	emit sendEnableReceiverSignal(id, val);
}

void RpcFlakonClientWrapper::sendWorkMode(const int mode, const bool isOn)
{
	emit sendWorkModeSignal(mode, isOn);
}

void RpcFlakonClientWrapper::sendSolverSetupSettings(const QByteArray &data)
{
	emit sendSolverSetupSettingsSignal( data );
}

void RpcFlakonClientWrapper::sendRdsProto(const QByteArray &data)
{
	emit sendRdsProtoSignal( data );
}

void RpcFlakonClientWrapper::requestFlakonStatus()
{
	emit requestFlakonStatusSignal();
}

void RpcFlakonClientWrapper::clearAllReceiversList()
{
	if(m_rpcClient) {
		m_rpcClient->clearReceiversList();
	}
}
