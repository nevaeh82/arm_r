#include "RpcFlakonClientWrapper.h"

RpcFlakonClientWrapper::RpcFlakonClientWrapper(QObject *parent) :
	QObject(parent)
{
	m_rpcClient = NULL;

	connect(this, SIGNAL(initSignal()), this, SLOT(initSlot()));
	connect(this, SIGNAL(stopSignal()), this, SLOT(stopSlot()));

	connect(this, SIGNAL(sendMainStationCorrelationSignal(int, QString )), this, SLOT(sendMainStationCorrelationSlot(int, QString)));
	connect(this, SIGNAL(sendBandwidthSignal(int, float)), this, SLOT(sendBandwidthSlot(int, float)));
	connect(this, SIGNAL(sendShiftSignal(int, float)), this, SLOT(sendShiftSlot(int, float)));
	connect(this, SIGNAL(recognizeSignal(int, int)), this, SLOT(recognizeSlot(int, int)));
	connect(this, SIGNAL(sendCorrelationSignal(int, bool)), this, SLOT(sendCorrelationSlot(int, bool)));
	connect(this, SIGNAL(sendAvarageSpectrumSignal(int, int)), this, SLOT(sendAvarageSpectrumSlot(int, int)));
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

void RpcFlakonClientWrapper::recognizeSlot(int id, int type)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->recognize(id, type);
}

void RpcFlakonClientWrapper::sendCorrelationSlot(int id, bool enable)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendCorrelation(id, enable);
}

void RpcFlakonClientWrapper::sendAvarageSpectrumSlot(int id, int avarage)
{
	if(NULL == m_rpcClient) {
		return;
	}

	m_rpcClient->sendAvarageSpectrum(id, avarage);
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

void RpcFlakonClientWrapper::recognize(const int id, const int type)
{
	emit recognizeSignal(id, type);
}

void RpcFlakonClientWrapper::sendCorrelation(const int id, const bool enable)
{
	emit sendCorrelationSignal(id, enable);
}

void RpcFlakonClientWrapper::sendAvarageSpectrum(const int id, const int avarage)
{
	emit sendAvarageSpectrumSignal(id, avarage);
}
